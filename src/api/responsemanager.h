#ifndef RESPONSEMANAGER_H
#define RESPONSEMANAGER_H

#include "response.h"
#include "Request.h"
#include "Socket.h"

class ResponseManager
{
public:
    static void Respond(const Http::Request&, Http::Response, std::shared_ptr<IO::Socket>);
};

#endif // RESPONSEMANAGER_H
