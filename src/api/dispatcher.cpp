#include <thread>
#include <sstream>
#include "dispatcher.h"
#include "HttpUtility.h"
#include "routesmanager.h"
#include "responsemanager.h"
using namespace Web;

void Dispatcher::Dispatch(std::map<std::shared_ptr<Application>, std::vector<std::shared_ptr<IO::Socket>>> diagram)
{
    for(auto &application : diagram) {
        if(application.first.get() != nullptr) {
            for(auto &connection : application.second) {
                auto request = Http::HttpUtility::ParseFrom(connection);
                if(RoutesManager::IsPassable(request)) {
                    auto handler = RoutesManager::GetHandler(request, (*application.first).route());
                    if(handler) {
                        //TODO pass to user
                        PassToUser(std::move(request), std::move(handler), connection);
                    }
                    //TODO see what you can do if there's no handler defined
                }
            }
        }
        else {
            //TODO return 404
        }
    }
}

void Dispatcher::PassToUser(Http::Request request, std::function<Http::Response(Http::Request)> user_handler, std::shared_ptr<IO::Socket> socket)
{
    /* from this point, everything is asynchronous */
    std::thread request_thread([&]() {
        auto response = user_handler(request);
        ResponseManager::Respond(request, std::move(response), socket);
    });
    request_thread.detach();
}
