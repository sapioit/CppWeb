#include <map>
#include "server.h"
#include "Watcher.h"
#include "Parser.h"
#include "dispatcher.h"
#include "storage.h"
#include <utility>
#include <algorithm>
#include <functional>
#include <iostream>
#include <thread>
#include <future>
#include <regex>
#define USE_GLOBAL_DEFINES
#include "global.h"

using namespace Web;

std::mutex Log::mLock;
std::string Log::_fn;
Server::Server(int port, int maxConcurrent) : _port(port), _maxPending(maxConcurrent)
{
    Log::Init("log_file.txt");        
    Log::SetEnabled(false);
    Log::i("Started logging");
}


void Server::run()
{
    if(_port == -1)
        throw std::runtime_error("Port number not set");
    try {
        _masterSocket = IO::Socket::start_socket(_port, _maxPending);
        IO::Watcher _master_listener(_masterSocket, _maxPending);

        Storage::InitializeOutputScheduler(_maxPending);
        IO::OutputScheduler &output_scheduler = Storage::output_scheduler();
        std::thread output_thread(&IO::OutputScheduler::Run, std::ref(output_scheduler));

        output_thread.detach();
        _master_listener.Start([&](std::vector<std::shared_ptr<IO::Socket>> sockets) {
            for(auto& sock: sockets) {
                    Log::i("Will dispatch " + std::to_string(sockets.size()) + " connections");
                    bool should_close = Dispatcher::Dispatch((*sock));
                    if(should_close)
                        _master_listener.Close(sock);
            }
        });

        output_thread.detach();
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



