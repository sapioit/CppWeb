//
// Created by vladimir on 09.08.2015.
//

#ifndef SOCKET_HTTPUTILITY_H
#define SOCKET_HTTPUTILITY_H

#include "Socket.h"
#include "Request.h"


namespace Http {
class Parser {
    IO::Socket& _sock;
public:
    Parser(IO::Socket& sock) : _sock(sock) {}
    Request operator()();
    Request Init();
    Components::Method GetMethod(const std::string& str);
    Header GetHeader();
    //static std::string GetURI(const std::string& line);
};
};

#endif //SOCKET_HTTPUTILITY_H
