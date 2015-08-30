#include <io/outputscheduler.h>
#include <misc/storage.h>

#include <algorithm>
#include <cassert>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>

void IO::OutputScheduler::dump_schedule()
{
    std::ostringstream dump;
    dump << "Dumping schedule items:" << std::endl;
    for (scheduled_write& item : _schedule) {
        dump << item.dump();
    }
    Log::i(dump.str());
}

void IO::OutputScheduler::add_event(const IO::scheduled_write& sched_item)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.data.fd = sched_item.sock()->get_fd();
    ev.events = EPOLLOUT | EPOLLET;
    auto add_result = epoll_ctl(_efd, EPOLL_CTL_ADD, ev.data.fd, &ev);
    if (-1 == add_result)
        throw std::runtime_error("Could not add event with fd:" + std::to_string(sched_item.sock()->get_fd()));
}

void IO::OutputScheduler::remove_socket(IO::Socket& sock, int position, epoll_event* ev)
{
    Log::i("Will remove socket with fd (1)= " + std::to_string(sock.get_fd()));
    epoll_ctl(_efd, EPOLL_CTL_DEL, sock.get_fd(), ev);
    _schedule.erase(_schedule.begin() + position);
}

void IO::OutputScheduler::remove_socket(int sockfd)
{
    Log::i("Will remove socket with fd = " + std::to_string(sockfd));
    IO::Socket* socket = nullptr;
    int socket_position = -1;
    for (auto index = 0u; index < _schedule.size(); ++index) {
        if (_schedule[index].sock()->get_fd() == sockfd) {
            socket = &((*_schedule[index].sock()));
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
    }
    else {
        Log::e("could not find socket with sockfd = " + std::to_string(sockfd) + " in order to remove it");
    }
}

IO::OutputScheduler::OutputScheduler(int max_events)
    : _maxEv(max_events)
{
    _efd = epoll_create1(0);
    _events.resize(_maxEv);
}

bool is_error(const std::uint32_t& ev)
{
    if ((ev & EPOLLERR) || (ev & EPOLLHUP) || (ev & EPOLLRDHUP) || (!(ev & EPOLLOUT)))
        return true;
    return false;
}

void log_error(int efd)
{
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

int IO::OutputScheduler::get_events()
{
    Log::i("get_events invoked. schedule: " + std::to_string(_schedule.size()) + " items. events: " + std::to_string(_events.size()));
    dump_schedule();
    auto dump_events = [&]() {
        std::ostringstream dump;
        for(auto index = 0u; index < _schedule.size(); ++index) {
            for(auto index2 = 0u; index2 < _events.size(); ++index2) {
                if(_events[index2].data.fd == _schedule[index].sock().get()->get_fd()) {
                    dump << "fd = " + std::to_string(_events[index2].data.fd) + " \n" + _schedule[index].dump() << std::endl;
                }
            }
        }
        Log::i("Dumping events: " + dump.str());
    };

    dump_events();
    int ev_no = -1;
    do {
        ev_no = epoll_wait(_efd, &_events.front(), _events.size(), -1);
        if (ev_no == -1 && errno != EINTR)
            throw std::runtime_error("Epoll wait error, errno = " + std::to_string(errno));
    } while (ev_no == -1);
    Log::i("got " + std::to_string(ev_no) + " events (writing)");
    return ev_no;
}
void IO::OutputScheduler::write(const epoll_event& event, IO::scheduled_write& scheduled_write)
{
    DataPacket& packet = scheduled_write.current_packet();

    const auto& type = typeid(packet);

    std::size_t written = static_cast<decltype(written)>(-1);

    if (type.hash_code() == typeid(IO::File).hash_code()) {
        IO::File& file = dynamic_cast<IO::File&>(packet);
        written = scheduled_write.sock()->Write(file);
        if (written < file.left()) {
            Log::i("on fd = " + std::to_string(event.data.fd) + ", wrote " + std::to_string(written) + " remaining = " + std::to_string(file.size() - written));
            scheduled_write.current_packet_state().transition(IO::scheduled_write::WriteResult::NotFinalized);
        }
        else {
            scheduled_write.current_packet_state().transition(IO::scheduled_write::WriteResult::Finalized);
        }
    }
    else if (type.hash_code() == typeid(IO::MemoryPacket<std::string>).hash_code()) {
        IO::MemoryPacket<std::string>& mem_packet = dynamic_cast<IO::MemoryPacket<std::string>&>(packet);
        written = scheduled_write.sock()->Write(mem_packet.rep);
        auto left = mem_packet.left();
        if (written < left) {
            auto oldSize = mem_packet.left();
            std::string(mem_packet.rep.begin() + written, mem_packet.rep.end()).swap(mem_packet.rep);
            auto newSize = mem_packet.left();
            assert(oldSize - written == newSize);
            scheduled_write.current_packet_state().transition(IO::scheduled_write::WriteResult::NotFinalized);
        }
        else {
            scheduled_write.current_packet_state().transition(IO::scheduled_write::WriteResult::Finalized);
        }
    }

    Log::i("on fd = " + std::to_string(event.data.fd) + ", wrote " + std::to_string(written) + " remaining = " + std::to_string(packet.left() - written));
}

void IO::OutputScheduler::Run()
{
    try {
        _stopRequested = false;
        while (!_stopRequested) {
            int events_number = get_events();
            for (auto index = 0; index < events_number; ++index) {
                if (is_error(_events[index].events)) {
                    Log::i("events received errors");
                    //log_error(_efd);
                    remove_socket(_events[index].data.fd);
                }
                else {
                    volatile std::lock_guard<std::mutex> schedule_lock(_scheduling_mutex);

                    std::size_t err_pos = static_cast<std::size_t>(-1);
                    std::size_t scheduled_item_pos = err_pos;
                    for (decltype(scheduled_item_pos) index2 = 0;
                         index2 < _schedule.size(); ++index2) {
                        if (_schedule[index2].sock()->get_fd() == _events[index].data.fd) {
                            scheduled_item_pos = index2;
                        }
                    }

                    scheduled_write& sched_item = _schedule[scheduled_item_pos];
                    if (scheduled_item_pos != err_pos) {
                        scheduled_write& sched_item = _schedule[scheduled_item_pos];
                        auto& machine = sched_item.current_packet_state();
                        write(_events[index], sched_item);
                        if (machine.currentState() == machine.finalState()) {
                            Log::i(sched_item.dump());
                            Log::i("popping scheduled item");
                            sched_item.pop();
                            Log::i("items left = " + std::to_string(sched_item.packets_left()));
                            if (sched_item.packets_left() == 0) {
                                Log::i("will remove socket with fd = " + std::to_string(sched_item.sock()->get_fd()));
                                remove_socket(sched_item.sock()->get_fd());
                            }
                        }
                    }
                }
            }
        }
    }
    catch (std::exception& ex) {
        throw;
    }
}

std::unique_ptr<IO::OutputScheduler> IO::OutputScheduler::_instance;

IO::OutputScheduler& IO::OutputScheduler::get()
{
    if (_instance.get() == nullptr)
        _instance.reset(new OutputScheduler(Storage::settings().max_connections));
    return (*_instance);
}
