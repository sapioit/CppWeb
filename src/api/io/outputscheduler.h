#ifndef OSCHEDULER_H
#define OSCHEDULER_H
#include <queue>
#include <io/socket.h>
#include <io/scheduled_write.h>
#include <sys/epoll.h>
#include <mutex>
#include <memory>

namespace IO {
class OutputScheduler {


  void Write(const epoll_event& event, std::size_t scheduled_item_pos);

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
  void Write(auto index, std::size_t scheduled_item_pos);
};
};

#endif // OSCHEDULER_H
