#include "response.h"
#include "Request.h"
#include <json/json.h>
#include "date.h"
#include <iomanip>
#include <iterator>
#include "log.h"
using namespace Http;

int Response::code() const
{
    return _code;
}

void Response::setCode(int code)
{
    _code = code;
}
const Components::ContentType& Response::getContent_type() const
{
    return _content_type;
}

void Response::setContent_type(const Components::ContentType &value)
{
    _content_type = value;
}

std::string Response::str() const
{
    std::ostringstream stream;
    constexpr auto crlf = "\r\n";
    const decltype(Components::content_types)& mime_types = Components::content_types;

    stream << "HTTP/" << std::setprecision(2) << _request.version << " " << code() << " " << Components::status_codes.at(static_cast<Components::StatusCode>(code())) << crlf;
    stream << "Date:" << " " << Date::Now()() << crlf;
    stream << "Connection: " << (should_close() ? "Close" : "Keep-Alive") << crlf;

    if(!is_error()) {
        auto type_str = mime_types.find(getContent_type());
        if(type_str == mime_types.end())
            throw 415;

        if(has_resource()){
            stream << Http::Header::Fields::Content_Type << ": " << type_str->second << crlf;
            stream << Http::Header::Fields::Content_Length << ": " << _resource.content().size()  << crlf;
            //stream << Http::Header::Fields::Cache_Control << ": " << (should_cache() ? "max-age=" + std::to_string(get_expiry()) : "no-cache") << crlf;
            stream << Http::Header::Fields::Transfer_Encoding << ": " << "binary" << crlf;
        }
        else {
            // text response
            if(has_body()) {
                stream << Http::Header::Fields::Content_Type << ": " << type_str->second + "; "  + "charset=utf-8" << crlf;
                stream << Http::Header::Fields::Content_Length << ": " << _text.size() << crlf;
            }
        }

        stream << crlf;
        Log::i(stream.str());
        // start of body
        if(has_body()) {
            if(has_resource())
                std::copy(_resource.content().begin(), _resource.content().end(), std::ostream_iterator<char>(stream));
            else
                std::copy(_text.begin(), _text.end(), std::ostream_iterator<char>(stream));
            stream << crlf;
            stream << crlf;
        }
    }
    else {
        stream << Http::Header::Fields::Content_Length << ": " << 123  << crlf;
        stream << crlf;
    }

    auto response_str = stream.str();
    if(is_error())
        Log::i(response_str);
    return response_str;
}
const Request &Response::getRequest() const
{
    return _request;
}


bool Response::should_cache() const
{
    return false;
}

uint32_t Response::get_expiry() const
{
    return 60;
}

bool Response::should_close() const
{
    auto connection = _request.header.fields.find(Header::Fields::Connection);
    if(connection == _request.header.fields.end())
        return true;

    if(connection->second == "Keep-Alive")
        return false;

    return true;
}

bool Response::has_body() const
{
    return _text.size() || _resource.content().size();
}

bool Response::has_resource() const
{
    return _resource.content().size();
}

bool Response::is_error() const
{
    Components::StatusCode s_code = static_cast<Components::StatusCode>(code());
    if(s_code == Components::StatusCode::BadRequest ||
            s_code == Components::StatusCode::InternalServerError ||
            s_code == Components::StatusCode::NotFound ||
            s_code == Components::StatusCode::UnsupportedMediaType)
        return true;

    return false;
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

