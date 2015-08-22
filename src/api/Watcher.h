//
// Created by Vladimir on 8/2/2015.
//

#ifndef SOCKET_OBSERVER_H
#define SOCKET_OBSERVER_H

#include "Socket.h"
#include <sys/epoll.h>
#include <memory>
#include <vector>

namespace IO {
class Watcher {
 public:
  Watcher(std::shared_ptr<Socket>, int);
  ~Watcher();
  void Stop();
  void Close(int);
  void Close(std::shared_ptr<IO::Socket>);
  void Start(std::function<void(std::shared_ptr<Socket>)>);
  void Start(std::function<void(std::vector<std::shared_ptr<Socket>>)>);

 private:
  std::shared_ptr<Socket> _socket;
  int _maxEvents;
  std::vector<std::shared_ptr<Socket>> _to_observe;
  int _efd;
  std::vector<epoll_event> _events;
  bool _stopRequested = false;
  std::vector<std::shared_ptr<Socket>> Watch();
  void AddSocket(std::shared_ptr<Socket> socket);

  bool stopRequested() const;
  void setStopRequested(bool stopRequested);
};
};

#endif  // SOCKET_OBSERVER_H
