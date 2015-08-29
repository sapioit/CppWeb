#ifndef OSCHEDULER_H
#define OSCHEDULER_H
#include <queue>
#include "socket.h"
#include <sys/epoll.h>
#include <mutex>
#include <memory>

namespace IO {
class OutputScheduler {
  struct scheduled_write {
    std::shared_ptr<Socket> sock;
    std::vector<char> data;
    scheduled_write(std::shared_ptr<Socket> s, const std::string& d)
        : sock(s), data(d.begin(), d.end()) {}
    ~scheduled_write() = default;
    scheduled_write(scheduled_write&&) = default;
    scheduled_write(const scheduled_write&) = default;
    scheduled_write& operator=(const scheduled_write&) = default;
  };

  std::vector<epoll_event> _events;
  std::vector<scheduled_write> _schedule;
  std::mutex _scheduling_mutex;

  void add_socket(const IO::Socket&, const std::string&);
  void remove_socket(IO::Socket&, int, epoll_event* ev);
  void remove_socket(int);
  void Loop();
  int GetEvents();

  int _efd, _maxEv;
  bool _stopRequested = true;
  static std::unique_ptr<OutputScheduler> _instance;
 public:
  OutputScheduler(int max_events);
  void ScheduleWrite(const Socket&, const std::string&);
  void Run();
  void Stop() { _stopRequested = true; }
  static OutputScheduler& get();
};
};

#endif  // OSCHEDULER_H
