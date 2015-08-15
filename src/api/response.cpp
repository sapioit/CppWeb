#include "response.h"
#include "Request.h"
#include <json/json.h>
#include "date.h"
#include <iomanip>
#include <iterator>
using namespace Http;

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

std::string Response::str()
{
    std::ostringstream stream;
    constexpr auto crlf = "\r\n";

    stream << "HTTP/" << std::setprecision(2) << _request.version << " " << code() << " " << "OK" << crlf;
    stream << "Date:" << " " << Date::Now()() << crlf;
    stream << "Connection:" << " " << "Close" << crlf;

    if(_request.IsResource()){
        stream << Http::Header::Fields::Content_Length << ": " << _resource.content().size()  << crlf;
        stream << Http::Header::Fields::Content_Type << ": " << "image/jpeg" << crlf;
        stream << Http::Header::Fields::Cache_Control << ": " << "no-cache" << crlf;
        stream << Http::Header::Fields::Transfer_Encoding << ": " << "binary" << crlf;
    }
    else {
        stream << Http::Header::Fields::Content_Type << ": " << "text/plain; charset=utf-8" << crlf;
        stream << Http::Header::Fields::Content_Length << ": " << _text.size()   << crlf;
    }

    stream << crlf;

    // START BODY
    if(_request.IsResource()) {
        std::copy(_resource.content().begin(), _resource.content().end(), std::ostream_iterator<char>(stream));
        stream << crlf;
    }

    stream << crlf;

    auto response_str = stream.str();
    return response_str;
}
const Resource& Response::getResource() const
{
    return _resource;
}

void Response::setResource(const Resource &resource)
{
    _resource = resource;
}

Response::Response()
{

}

Response::Response(const Request &request) : _request(request)
{

}

Response::Response(const Request& request, int code) : _request(request), _code(code)
{

}

Response::Response(const Request& request, const std::string &text) : _request(request), _code(200), _text(text)
{

}

Response::Response(const Request& request, int code, const std::string &text) : _request(request), _code(code), _text(text)
{

}

Response::Response(const Request& request, const Resource & resource) :_request(request), _resource(resource), _code(200)
{

}
