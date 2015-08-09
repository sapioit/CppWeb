#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <memory>
#include "Socket.h"
#include "application.h"

namespace Web{
class Server
{
public:
    Server(int);
    void add(std::shared_ptr<Application>);
    void run();

    int maxPending() const;
    void setMaxPending(int maxPending);

private:
    int _port = -1;
    int _maxPending = 100;
    std::vector<std::shared_ptr<Application>> _applications;
    std::shared_ptr<IO::Socket> _masterSocket;
};
};

#endif // SERVER_H
