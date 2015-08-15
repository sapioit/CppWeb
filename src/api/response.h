#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <iostream>
#include "Header.h"
#include "resource.h"
#include "Request.h"

namespace Http{
class Response
{
public:
    Response();
    Response(const Request&);
    Response(const Request&, int);
    Response(const Request&, const std::string&);
    Response(const Request&, int, const std::string&);
    Response(const Request&, const Resource&);
    Response(const Response&) = default;
    Response(Response&&) = default;
    Response& operator=(const Response&) = default;
    ~Response() = default;

    int code() const;
    void setCode(int code);

    Header::MIME_Type getContent_type() const;
    void setContent_type(const Header::MIME_Type &value);

    std::string str();

    const Resource &getResource() const;
    void setResource(const Resource &resource);

private:
    Request _request;
    int _code;
    std::string _text;
    Resource _resource;
    Header::MIME_Type content_type = Header::MIME_Type::ApplicationJson;

    // Or, more generally:
};
//std::ostream& operator<<( std::ostream& o, const Response& r);
};

#endif // RESPONSE_H
