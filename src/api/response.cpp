#include "response.h"
#include "Request.h"
#include <json/json.h>
#include "date.h"
using namespace Http;
Response::Response()
{

}

Response::Response(int code) : _code(code)
{

}

Response::Response(const std::string &text) : _code(200), _text(text)
{

}

Response::Response(int code, const std::string &text) : _code(code), _text(text)
{

}
int Response::code() const
{
    return _code;
}

void Response::setCode(int code)
{
    _code = code;
}
Header::MIME_Type Response::getContent_type() const
{
    return content_type;
}

void Response::setContent_type(const Header::MIME_Type &value)
{
    content_type = value;
}

std::string Response::str() const
{
    std::ostringstream stream;
    constexpr auto crlf = "\r\n";

    stream << "HTTP/1.0" << " " << code() << " OK" << crlf;
    stream << "Date:" << " " << Date::Now()() << crlf;
    stream << "Connection" << " " << "Close" << crlf;
    stream << Http::Header::Fields::Content_Type << ": " << "text/plain; charset=utf-8" << crlf;
    stream << Http::Header::Fields::Content_Length << ": " << 2 << crlf;

    stream << crlf;

    // START BODY

    stream << "ab" << crlf;

    stream << crlf;

    return stream.str();
}

//std::ostream& operator<<( std::ostream& stream, const Response& response) {

//    stream << "HTTP/1.1" << " " << response.code() << "OK" << "\r\n";


//    return stream;
//}
