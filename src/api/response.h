#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <iostream>
#include "Header.h"
namespace Http{
class Response
{
public:
    Response();
    Response(int);
    Response(const std::string&);
    Response(int, const std::string&);
    Response(const Response&) = default;
    Response(Response&&) = default;
    Response& operator=(const Response&) = default;
    ~Response() = default;
private:
    Header::MIME_Type content_type = Header::MIME_Type::ApplicationJson;
    std::string _text;
    int _code;
    friend std::ostream& operator<<(std::ostream&, const Response&);
};
};

#endif // RESPONSE_H
