//
// Created by vladimir on 08.08.2015.
//

#ifndef SOCKET_REQUEST_H
#define SOCKET_REQUEST_H

#include "Header.h"
#include "Socket.h"
#include <components.h>
#include <string>
#include <map>

namespace Http {

    class Request {
    public:
        Components::Method method;
        Header header;
        std::string URI, body;
        float version;
        Request() = default;
        Request(const Request&) = default;
        Request(Request&&) = default;
        Request& operator=(const Request&) = default;
        bool operator==(const Request&);
        bool IsPassable() const;
        bool IsResource() const;

    };
}


#endif //SOCKET_REQUEST_H
