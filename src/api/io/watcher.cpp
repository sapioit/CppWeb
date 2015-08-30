//
// Created by Vladimir on 8/2/2015.
//

#include <io/watcher.h>
#include <misc/log.h>

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <string.h>
#include <sstream>

using namespace IO;

Watcher::Watcher(std::shared_ptr<Socket> socket, int maxEvents)
    : _socket(socket), _maxEvents(maxEvents) {
  _efd = epoll_create1(0);
  if (_efd == -1) {
    throw std::runtime_error("Epoll create failed");
  }
  try {
    AddSocket(socket);
  } catch (std::runtime_error& ex) {
    throw;
  }

  _events.resize(_maxEvents);
}

void Watcher::Stop() { _stopRequested = true; }

void Watcher::RemoveSocket(int fd) {
  struct epoll_event* ev = nullptr;
  for (auto& event : _events) {
    if (event.data.fd == fd) {
      ev = &event;
      break;
    }
  }
  auto result = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, ev);

  if (result == -1) {
    // error
  }

  for (std::size_t index = 0; index < _to_observe.size(); ++index) {
    if ((*_to_observe[index]).get_fd() == fd) {
      _to_observe.erase(_to_observe.begin() + index);
      break;
    }
  }
}

void Watcher::RemoveSocket(std::shared_ptr<Socket> sock) {
  Watcher::RemoveSocket((*sock).get_fd());
}

Watcher::~Watcher() {
  for (auto& event : _events)
    ::close(event.data.fd);
}
bool Watcher::stopRequested() const { return _stopRequested; }

void Watcher::setStopRequested(bool stopRequested) {
  _stopRequested = stopRequested;
}

std::vector<std::shared_ptr<Socket> > Watcher::Watch() {
  int events_number;
  std::vector<std::shared_ptr<Socket> > result;
  _events.resize(_maxEvents);
  do {
    events_number = epoll_wait(_efd, _events.data(), _maxEvents, -1);
    if (events_number == -1 && errno != EINTR)
      throw std::runtime_error("Epoll wait error, errno = " +
                               std::to_string(errno));
  } while (events_number == -1);
  _events.resize(events_number);
  for (int index = 0; index < events_number; ++index) {
    if ((_events[index].events & EPOLLERR) ||
        (_events[index].events & EPOLLHUP) ||
        (!(_events[index].events & EPOLLIN))) {
      std::ostringstream error;
      int ierror = 0;
      socklen_t errlen = sizeof(ierror);
      if (!getsockopt(_efd, SOL_SOCKET, SO_ERROR, (void*)&ierror, &errlen)) {
        error << "getsockopt SO_ERROR = " << ierror
              << " strerror: " << strerror(ierror);
      }

      Log::e(error.str());

      // TODO call the member Close
      ::close(_events[index].data.fd);
      ::close((*_to_observe[index]).get_fd());
      _to_observe.erase(_to_observe.begin() + index);
      continue;
    }

    if (_events[index].events & EPOLLRDHUP)
      RemoveSocket(_events[index].data.fd);

    if ((*_socket).get_fd() == _events[index].data.fd) {
      /* the listening socket received something,
       * that means we'll accept a new connection */
      try {
        while (true) {
          auto new_connection = (*_socket).Accept();
          if ((*new_connection).get_fd() == -1) {
            Log::e("error on accepting, errno = " + std::to_string(errno));
            break;
          }
          (*new_connection).MakeNonBlocking();
          AddSocket(new_connection);
        }
      } catch (std::exception& ex) {
        Log::e(ex.what());
        throw;
      }
    } else {
      /* a connection socket received something, we'll add it
       * to the list of active socekts */
      auto connection_it =
          std::find_if(_to_observe.begin(), _to_observe.end(),
                       [&](std::shared_ptr<Socket> ptr) {
                         return (*ptr).get_fd() == _events[index].data.fd;
                       });
      if (connection_it != _to_observe.end()) {
        result.push_back(std::shared_ptr<Socket>(*connection_it));
      } else
        throw std::runtime_error(
            "Error when polling. An inactive socket was marked as active.");
    }
  }
  return result;
}

void Watcher::AddSocket(std::shared_ptr<Socket> socket) {
  _to_observe.push_back(socket);
  int fd = (*socket).get_fd();
  struct epoll_event ev;
  memset(&ev, 0, sizeof(struct epoll_event));
  ev.data.fd = fd;
  ev.events = EPOLLIN | EPOLLET;
  int result = epoll_ctl(_efd, EPOLL_CTL_ADD, ev.data.fd, &ev);
  if (result == -1)
    throw std::runtime_error("Could not add event with fd " +
                             std::to_string(fd));
}

void Watcher::Start(std::function<void(std::shared_ptr<Socket>)> callback) {
  try {
    while (!stopRequested()) {
      auto sockets_with_activity = Watch();
      for (auto& socket : sockets_with_activity) {
        callback(socket);
      }
    }
  } catch (std::runtime_error& ex) {
    throw;
  }
}

void Watcher::Start(
    std::function<void(std::vector<std::shared_ptr<Socket> >)> callback) {
  try {
    while (!stopRequested()) {
      auto sockets_with_activity = Watch();
      if (sockets_with_activity.size() > 0) {
        callback(sockets_with_activity);
      }
    }
  } catch (std::runtime_error& ex) {
    throw;
  }
}
