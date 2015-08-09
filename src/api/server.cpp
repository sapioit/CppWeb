#include <map>
#include "server.h"
#include "Watcher.h"
#include "HttpUtility.h"
#include "dispatcher.h"
#include <utility>
#define USE_GLOBAL_DEFINES
#include "global.h"

using namespace Web;

std::shared_ptr<Application> MatchApplication(const std::string& URI) {
    return std::make_shared<Application>(nullptr);
}

Server::Server(int port) : _port(port)
{

}

void Server::add(std::shared_ptr<Application> application_ptr)
{
    _applications.push_back(application_ptr);
}

void Server::run()
{
    if(_port == -1)
        throw std::runtime_error("Port number not set");
    try {
        _masterSocket = IO::Socket::start_socket(_port, _maxPending);
        IO::Watcher _master_listener(_masterSocket);
        _master_listener.Start([](std::vector<std::shared_ptr<IO::Socket>> sockets) {
            /*TODO:
             * parse the request on each socket/connection
             * map each socket to application
             * pass the diagram to the dispatcher
             */
            std::map<std::shared_ptr<Application>, std::vector<std::shared_ptr<IO::Socket>>> diagram;
            for(auto &socket : sockets) {
                //TODO parse the request
                //TODO identify the application
                //TODO at one point, optimize this to only parse the URI, instead of parsing the header too
                auto firstLine = (*socket).ReadUntil("\r\n", true);
                auto URI = Http::HttpUtility::GetURI(firstLine);
                auto application = MatchApplication(URI);
                diagram[application].push_back(socket);
            }
            Dispatcher::Dispatch(std::move(diagram));
        });
    }
    catch(std::exception& ex) {
            throw;
    }
}
int Server::maxPending() const
{
    return _maxPending;
}

void Server::setMaxPending(int maxPending)
{
    _maxPending = maxPending;
}


