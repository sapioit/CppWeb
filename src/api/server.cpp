#include <map>
#include "server.h"
#include "Watcher.h"
#include "Parser.h"
#include "dispatcher.h"
#include <utility>
#include <algorithm>
#include <iostream>
#include <regex>
#define USE_GLOBAL_DEFINES
#include "global.h"

using namespace Web;

std::mutex Log::mLock;
std::string Log::_fn;
Server::Server(int port) : _port(port)
{
    Log::Init("log_file.txt");
    Log::i("Started logging");
}


void Server::run()
{
    if(_port == -1)
        throw std::runtime_error("Port number not set");
    try {
        _masterSocket = IO::Socket::start_socket(_port, _maxPending);
        IO::Watcher _master_listener(_masterSocket);
        _master_listener.Start([&](std::vector<std::shared_ptr<IO::Socket>> sockets) {
            for(auto& sock: sockets) {
                if((*sock).WasShutDown())
                    _master_listener.Close(sock);
                else
                    Dispatcher::Dispatch((*sock));
            }
        });
    }
    catch(std::exception& ex) {
        Log::e(std::string("Server error: ").append(ex.what()));
        auto msg = std::string("Server error. Please see the log file. Last exception: ");
        msg += ex.what();
        throw std::runtime_error(msg);
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



