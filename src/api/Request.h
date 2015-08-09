//
// Created by vladimir on 08.08.2015.
//

#ifndef SOCKET_REQUEST_H
#define SOCKET_REQUEST_H

#include "Header.h"
#include "Socket.h"
#include <string>
#include <map>

namespace Http {
    struct Request {
        Request() = default;
        Request(const Request&) = default;
        Request(Request&&) = default;
        Request& operator=(const Request&) = default;
        bool operator==(const Request&);
        enum class Method;
        Method method;
        std::string URI;
        std::string version;
        Header header;
        std::string body;
        int _content_size = 0;
        static const std::map<std::string, Method> _methods;

        enum class Method {
            Delete,
            Get,
            Head,
            Post,
            Put,
            Connect,
            Options,
            Trace,
            Copy,
            Lock,
            Mkcol,
            Move,
            Propfind,
            Proppatch,
            Unlock,
            Report,
            Mkactivity,
            Checkout,
            Merge,
            M_Search,
            Notify,
            Subscribe,
            Unsubscribe
        };
        enum class Encoding {
            ApplicationJson
        };
    };
}


#endif //SOCKET_REQUEST_H
