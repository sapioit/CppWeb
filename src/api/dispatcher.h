#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <map>
#include <memory>
#include "application.h"
#include "Socket.h"
#define USE_GLOBAL_DEFINES
#include "global.h"
namespace Web {
class Dispatcher
{
public:
    static void Dispatch(std::map<std::shared_ptr<Application>, std::vector<std::shared_ptr<IO::Socket>>>);
    static void PassToUser(Http::Request, std::function<Http::Response(Http::Request)>, std::shared_ptr<IO::Socket>);
};
}

#endif // DISPATCHER_H
