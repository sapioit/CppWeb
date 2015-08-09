#include "responsemanager.h"
#include <sstream>

void ResponseManager::Respond(const Http::Request& request, Http::Response response, std::shared_ptr<IO::Socket> socket)
{
    /*TODO:
     * determine response type
     * serialize
     * write to socket
     */

    std::ostringstream stream;
    stream << response;
    std::string raw_response = stream.str();
    (*socket).Write(std::move(raw_response));
}
