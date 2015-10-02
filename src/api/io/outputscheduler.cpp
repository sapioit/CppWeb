#include <io/outputscheduler.h>
#include <misc/log.h>
#include <misc/storage.h>

#include <algorithm>
#include <cassert>
#include <string.h>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>

inline void IO::OutputScheduler::add_socket(const IO::Socket& sock,
                                            const std::string& data) {
    Log::i("socket with fd = " + std::to_string(sock.get_fd()) +
           " will be copied to fd = ");
    std::unique_ptr<IO::Socket> new_sock(new Socket(sock));

    scheduled_write sw(std::move(new_sock), data);

    _schedule.push_back(std::move(sw));

    struct epoll_event ev;
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.data.fd = (*_schedule.back().sock).get_fd();
    ev.events = EPOLLOUT | EPOLLET;

    Log::i(std::to_string((*_schedule.back().sock).get_fd()));

    if (-1 == epoll_ctl(_efd, EPOLL_CTL_ADD, ev.data.fd, &ev))
        throw std::runtime_error("Could not add event with fd:" +
                                 std::to_string(sock.get_fd()));
}

void IO::OutputScheduler::ScheduleWrite(const IO::Socket& sock,
                                        const std::string& data) {
    Log::i("scheduling write on fd = " + std::to_string(sock.get_fd()));
    volatile std::lock_guard<std::mutex> schedule_lock(_scheduling_mutex);
    add_socket(sock, data);
}

void IO::OutputScheduler::remove_socket(IO::Socket& sock, int position,
                                        epoll_event* ev) {
    epoll_ctl(_efd, EPOLL_CTL_DEL, sock.get_fd(), ev);
    _schedule.erase(_schedule.begin() + position);
}

void IO::OutputScheduler::remove_socket(int sockfd) {
    IO::Socket* socket = nullptr;
    int socket_position = -1;
    unsigned int index = 0;
    for (index = 0; index < _schedule.size(); ++index) {
        if ((*_schedule[index].sock).get_fd() == sockfd) {
            socket = &((*_schedule[index].sock));
            socket_position = index;
            break;
        }
    }

    assert(socket != nullptr);

    if (socket != nullptr) {
        struct epoll_event* ev = nullptr;
        for (auto& event : _events) {
            if ((event.data.fd) == socket->get_fd())
                ev = &(event);
        }

        remove_socket(*socket, socket_position, ev);
    } else {
        Log::e("could not find socket with sockfd = " + std::to_string(sockfd) +
               " in order to remove it");
    }
}

IO::OutputScheduler::OutputScheduler(int max_events) : _maxEv(max_events) {
    _efd = epoll_create1(0);
    _events.resize(_maxEv);
}

bool is_error(const std::uint32_t& ev) {
    if ((ev & EPOLLERR) || (ev & EPOLLHUP) || (ev & EPOLLRDHUP) ||
            (!(ev & EPOLLOUT)))
        return true;
    return false;
}

void log_error(int efd) {
    std::ostringstream error;
    int ierror = 0;
    socklen_t errlen = sizeof(ierror);
    if (!getsockopt(efd, SOL_SOCKET, SO_ERROR, (void*)&ierror, &errlen)) {
        error << "getsockopt SO_ERROR = " << ierror
              << " strerror: " << strerror(ierror);
    }
    std::cerr << error.str();
    Log::e(error.str());
}

int IO::OutputScheduler::GetEvents() {
    int ev_no = -1;
    do {
        ev_no = epoll_wait(_efd, &_events.front(), _events.size(), -1);
        if (ev_no == -1 && errno != EINTR)
            throw std::runtime_error("Epoll wait error, errno = " +
                                     std::to_string(errno));
    } while (ev_no == -1);
    return ev_no;
}

void IO::OutputScheduler::Run() {
    try {
        _stopRequested = false;
        while (!_stopRequested) {
            int events_number = GetEvents();
            for (auto index = 0; index < events_number; ++index) {
                volatile std::lock_guard<std::mutex> schedule_lock(_scheduling_mutex);
                Log::i("got " + std::to_string(events_number) + " events (writing)");
                if (is_error(_events[index].events)) {
                    log_error(_efd);
                    remove_socket(_events[index].data.fd);
                } else {

                    std::size_t err_pos = static_cast<std::size_t>(-1);
                    std::size_t scheduled_item_pos = err_pos;
                    for (decltype(scheduled_item_pos) index2 = 0;
                         index2 < _schedule.size(); ++index2) {
                        if (_schedule[index2].sock->get_fd() == _events[index].data.fd) {
                            scheduled_item_pos = index2;
                        }
                    }

                    if (scheduled_item_pos != err_pos) {
                        if (_schedule[scheduled_item_pos].data.size() == 0)
                            remove_socket(_schedule[scheduled_item_pos].sock->get_fd());

                        auto written = static_cast<std::size_t>(
                                    _schedule[scheduled_item_pos].sock->Write(
                                        _schedule[scheduled_item_pos].data));
                        if (written < _schedule[scheduled_item_pos].data.size()) {
                            auto oldSize = _schedule[scheduled_item_pos].data.size();
                            Log::i("on fd = " + std::to_string(_events[index].data.fd) +
                                   ", wrote " + std::to_string(written) + " remaining = " +
                                   std::to_string(_schedule[scheduled_item_pos].data.size() -
                                                  written));
                            std::vector<decltype(
                                        _schedule[scheduled_item_pos].data)::value_type>(
                                        _schedule[scheduled_item_pos].data.begin() + written,
                                        _schedule[scheduled_item_pos].data.end())
                                    .swap(_schedule[scheduled_item_pos].data);
                            auto newSize = _schedule[scheduled_item_pos].data.size();
                            assert(oldSize - written == newSize);
                        } else {
                            _schedule[scheduled_item_pos].data = {};
                        }
                    }
                }
            }
        }
    } catch (std::exception& ex) {
        throw;
    }
}

std::unique_ptr<IO::OutputScheduler> IO::OutputScheduler::_instance;

IO::OutputScheduler& IO::OutputScheduler::get() {
    if (_instance.get() == nullptr)
        _instance.reset(new OutputScheduler(Storage::settings().max_connections));
    return (*_instance);
}
