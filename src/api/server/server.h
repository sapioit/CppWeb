#ifndef SERVER_H
#define SERVER_H

#include <io/socket.h>
#include <misc/settings.h>
#include <misc/log.h>

#include <vector>
#include <memory>
#include <fstream>
namespace Web {
class Server {
 public:
  Server(int);
  void run();
  void setSettings(const Settings&);

  int maxPending() const;
  void setMaxPending(int maxPending);

 private:
  int _port = -1;
  int _maxPending;
  std::shared_ptr<IO::Socket> _masterSocket;
};
};

#endif  // SERVER_H
