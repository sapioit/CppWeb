#ifndef OSCHEDULER_H
#define OSCHEDULER_H
#include <queue>
#include <io/socket.h>
#include <io/scheduled_write.h>
#include <sys/epoll.h>
#include <mutex>
#include <memory>
#include <misc/log.h>
#include <string.h>

namespace IO {
class OutputScheduler {


    std::vector<epoll_event> _events;
    std::vector<scheduled_write> _schedule;
    std::mutex _scheduling_mutex;

    void dump_schedule();
    void add_event(const scheduled_write& sched_item);
    void remove_socket(IO::Socket&, int, epoll_event* ev);
    void remove_socket(int);
    void loop();
    int get_events();
    void write(const epoll_event& event, IO::scheduled_write& scheduled_write);

    int _efd, _maxEv;
    bool _stopRequested = true;

    static std::unique_ptr<OutputScheduler> _instance;

public:

    OutputScheduler(int max_events);
    template <typename... Args>
    void ScheduleWrite(const Socket& sock, Args... packets)
    {
        try {
            Log::i("scheduling write on fd = " + std::to_string(sock.get_fd()));
            volatile std::lock_guard<std::mutex> schedule_lock(_scheduling_mutex);
            std::shared_ptr<IO::Socket> new_sock(new Socket(sock));
            Log::i("socket with fd = " + std::to_string(sock.get_fd()) + " was copied to fd = " + std::to_string(new_sock.get()->get_fd()));
            scheduled_write sw(new_sock, std::forward<Args>(packets)...);
            _schedule.push_back(std::move(sw));
            add_event(_schedule.front());
        }
        catch (std::exception& ex) {
            throw;
        }
    }
    void Run();
    void Stop() { _stopRequested = true; }
    static OutputScheduler& get();
};
};

#endif // OSCHEDULER_H
