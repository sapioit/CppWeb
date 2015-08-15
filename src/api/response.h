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
    int code() const;
    void setCode(int code);

    Header::MIME_Type getContent_type() const;
    void setContent_type(const Header::MIME_Type &value);
    //friend std::ostream& operator<<( std::ostream&, const Response&);

    std::string str() const;

private:
    int _code;
    std::string _text;
    Header::MIME_Type content_type = Header::MIME_Type::ApplicationJson;

    // Or, more generally:
};
//std::ostream& operator<<( std::ostream& o, const Response& r);
};

#endif // RESPONSE_H
