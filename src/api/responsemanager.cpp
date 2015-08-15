#include "responsemanager.h"
#include <sstream>

void ResponseManager::Respond(const Http::Request& request, Http::Response response, IO::Socket& socket)
{
    /*TODO:
     * determine response type
     * serialize
     * write to socket
     */

    auto raw_response = response.str();
    try {
        socket.Write(std::move(raw_response));
    }
    catch(std::runtime_error &ex) {
        //log
    }
}
