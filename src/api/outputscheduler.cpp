#include "outputscheduler.h"
#include "log.h"
#include <algorithm>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <cassert>
#include <sys/socket.h>


inline void IO::OutputScheduler::add_socket(const IO::Socket &sock, const std::string& data)
{
    Log::i("socket with fd = " + std::to_string(sock.get_fd()) + " will be copied to fd = ");
    std::unique_ptr<IO::Socket> new_sock(new Socket(sock));

    scheduled_write sw(std::move(new_sock), data);

    _schedule.push_back(std::move(sw));

    struct epoll_event ev;
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.data.fd = (*_schedule.back().sock).get_fd();
    ev.events = EPOLLOUT | EPOLLET;

    Log::i(std::to_string((*_schedule.back().sock).get_fd()));

    if(-1 == epoll_ctl(_efd, EPOLL_CTL_ADD, ev.data.fd, &ev))
        throw std::runtime_error("Could not add event with fd:" + std::to_string(sock.get_fd()));
}

void IO::OutputScheduler::ScheduleWrite(const IO::Socket& sock, const std::string& data)
{
    Log::i("scheduling write on fd = " + std::to_string(sock.get_fd()));
    volatile std::lock_guard<std::mutex> schedule_lock(_scheduling_mutex);
    add_socket(sock, data);
}

void IO::OutputScheduler::remove_socket(IO::Socket &sock, int position, epoll_event* ev)
{
    epoll_ctl(_efd, EPOLL_CTL_DEL, sock.get_fd(), ev);
    _schedule.erase(_schedule.begin() + position);
}

void IO::OutputScheduler::remove_socket(int sockfd)
{
    IO::Socket* socket = nullptr;
    int socket_position = -1;
    struct epoll_event *ev = nullptr;
    unsigned int index = 0;
    for(index = 0; index < _schedule.size(); ++index) {
        if((*_schedule[index].sock).get_fd() == sockfd) {
            socket = &((*_schedule[index].sock));
            socket_position = index;
            break;
        }
    }

    assert(socket != nullptr);

    if(socket != nullptr) {
        for(auto& event : _events) {
            if((event.data.fd) == socket->get_fd())
                ev = &(event);
        }

        remove_socket(*socket, socket_position, ev);
    }
    else {
        Log::e("could not find socket with sockfd = " + std::to_string(sockfd) + " in order to remove it");
    }
}

IO::OutputScheduler::OutputScheduler(int max_events) : _maxEv(max_events)
{
    _efd = epoll_create1(0);
    _events.resize(_maxEv);
}



bool is_error(const std::uint32_t& ev) {
    if ((ev & EPOLLERR) ||
            (ev & EPOLLHUP) ||
            (ev & EPOLLRDHUP) ||
            (!(ev & EPOLLOUT)))
        return true;
    return false;
}

void log_error(int efd) {
    std::ostringstream error;
    int ierror = 0;
    socklen_t errlen = sizeof(ierror);
    if (!getsockopt(efd, SOL_SOCKET, SO_ERROR, (void *)&ierror, &errlen))
    {
        error << "getsockopt SO_ERROR = " << ierror << " strerror: " << strerror(ierror);
    }
    std::cerr << error.str();
    Log::e(error.str());

}

void IO::OutputScheduler::Run()
{
    _stopRequested = false;
    while(!_stopRequested) {
        int events_number;
        Log::i("Will wait until a fd is available for writing");
        do {
            events_number = epoll_wait(_efd, _events.data(), _maxEv, -1);
            if (events_number == -1 && errno != EINTR)
                throw std::runtime_error("Epoll wait error, errno = " + std::to_string(errno));
        }
        while (events_number == -1);

        for(auto index = 0; index < events_number; ++index) {
            Log::i("got " + std::to_string(events_number) + " events (writing)");
            if (is_error(_events[index].events)) {
                log_error(_efd);
                remove_socket(_events[index].data.fd);
            }
            else {
                auto scheduled_item_it  = std::find_if(_schedule.begin(), _schedule.end(), [&](const scheduled_write& item) {
                    if((*item.sock).get_fd() == _events[index].data.fd)
                        return true;
                    return false;
                });

                if(scheduled_item_it != _schedule.end()) {
                    scheduled_write& swr = *scheduled_item_it;

                    ssize_t bytes_written = (*(*scheduled_item_it).sock).Write(swr.data);
                    auto written = static_cast<std::size_t>(bytes_written);

                    if(written < (*scheduled_item_it).data.size())
                    {
                        auto oldSize = (*scheduled_item_it).data.size();
                        Log::i("on fd = " + std::to_string(_events[index].data.fd) + ", wrote " + std::to_string(written) + " remaining = " + std::to_string((*scheduled_item_it).data.size() - written));
                        (*scheduled_item_it).data.erase(0, written);
                        assert((oldSize - written) == (*scheduled_item_it).data.size());
                    }
                    else {
                        remove_socket((*swr.sock).get_fd());
                    }
                }
            }
        }
    }
}


