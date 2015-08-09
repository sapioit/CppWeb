//
// Created by vladimir on 09.08.2015.
//

#ifndef SOCKET_HTTPUTILITY_H
#define SOCKET_HTTPUTILITY_H

#include "Socket.h"
#include "Request.h"


namespace Http {
    class HttpUtility {
    public:
        static Request ParseFrom(std::shared_ptr<IO::Socket>);

    };
};

#endif //SOCKET_HTTPUTILITY_H
