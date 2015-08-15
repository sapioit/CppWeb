#include <thread>
#include <sstream>
#include "dispatcher.h"
#include "Parser.h"
#include "routeutility.h"
#include "global.h"
#include "log.h"
#include "responsemanager.h"

std::map<std::string, std::function<Http::Response(Http::Request)>> Web::Dispatcher::routes;

using namespace Web;

void Dispatcher::Dispatch(IO::Socket& connection)
{
    try {
        auto &&request = Http::Parser(connection)();
        if(request.IsPassable()) {
            auto handler = RouteUtility::GetHandler(request, routes);
            if(handler) {
                PassToUser(request, handler, connection);
            }
            else {
                //No handler defined. what to do?
            }
        }
        else {
            //not passable
        }}
    catch(std::runtime_error &ex) {
        Log::e(ex.what());
    }

}

void Dispatcher::PassToUser(Http::Request request, std::function<Http::Response(Http::Request)> user_handler, IO::Socket& socket)
{
    /* from this point, everything is asynchronous */
    std::thread request_thread([=, &socket]() {
        auto response = user_handler(request);
        ResponseManager::Respond(std::move(request), std::move(response), socket);
    });
    request_thread.detach();
}
