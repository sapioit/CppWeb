#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <map>
#include <memory>
#include "Socket.h"
#include "routeutility.h"
#include <functional>
#define USE_GLOBAL_DEFINES
#include "global.h"
namespace Web {
class Dispatcher
{
public:
    static std::map<std::string, std::function<Http::Response(Http::Request)>> routes;
    static bool Dispatch(IO::Socket& connection);
    static void PassToUser(Http::Request request, std::function<Http::Response(Http::Request)> user_handler, IO::Socket &socket);
};
}

#endif // DISPATCHER_H
