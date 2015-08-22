#include "response.h"
#include "Request.h"
#include "date.h"
#include <iomanip>
#include <iterator>
#include "log.h"
#include "DFA.h"
#include <sstream>
#include <utility>
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

enum class states { StatusLine, GeneralHeader, ResponseHeader, Body, CRLFHeader, CRLFBody, End };
enum class transitions { EndStatusLine, EndGeneralHeader, EndResponseHeader, EndBody, CRLFEnd, Error };

constexpr auto start = states::StatusLine;
constexpr auto end = states::End;

DFA<states, transitions> make_machine() {
    DFA<states, transitions> machine(start, end);
    machine.add(std::make_pair(states::StatusLine, transitions::EndStatusLine), states::GeneralHeader);
    machine.add(std::make_pair(states::GeneralHeader, transitions::Error), states::CRLFBody);
    machine.add(std::make_pair(states::GeneralHeader, transitions::EndGeneralHeader), states::ResponseHeader);
    machine.add(std::make_pair(states::ResponseHeader, transitions::EndResponseHeader), states::CRLFHeader );
    machine.add(std::make_pair(states::CRLFHeader, transitions::CRLFEnd), states::Body);
    machine.add(std::make_pair(states::Body, transitions::EndBody), states::CRLFBody);
    machine.add(std::make_pair(states::CRLFBody, transitions::CRLFEnd), states::End);
    return machine;
}


std::string Response::str() const {
    std::ostringstream stream;

    auto machine = make_machine();
    constexpr auto crlf = "\r\n";
    const decltype(Components::content_types)& mime_types = Components::content_types;

    while(machine.currentState() != end) {
        switch(machine.currentState()) {
        case states::StatusLine:
        {
            stream << "HTTP/" << std::setprecision(2) << 1.1;
            stream << " " << code() << " ";
            stream << Components::status_codes.at(static_cast<Components::StatusCode>(code())) << crlf;
            machine.transition(transitions::EndStatusLine);
            break;
        }
        case states::GeneralHeader:
        {
            stream << "Date:" << " " << Date::Now()() << crlf;
            stream << "Connection: " << (should_close() ? "Close" : "Keep-Alive") << crlf;
            //            if(is_error())
            //                machine.transition(transitions::Error);
            //            else
            machine.transition(transitions::EndGeneralHeader);
            break;
        }
        case states::ResponseHeader:
        {
            auto type_str_it =  mime_types.find(getContent_type());
            if(type_str_it == mime_types.end())
                throw 415;
            std::string type_str(type_str_it->second);
            stream << Http::Header::Fields::Content_Type << ": " << type_str << crlf;
            stream << Http::Header::Fields::Content_Length << ": ";
                      if(has_resource())
                        stream << _resource.content().size();
                      else stream << _text.size();
            stream << crlf;
            stream << Http::Header::Fields::Cache_Control << ": " << (should_cache() ? "max-age=" + std::to_string(get_expiry()) : "no-cache") << crlf;
            if(has_resource()) {
                if(getContent_type() == Components::ContentType::TextHtml ||
                        getContent_type() == Components::ContentType::TextPlain) {
                    //stream << Http::Header::Fields::Transfer_Encoding << ": " << "8bit";
                }
                else {
                    stream << Http::Header::Fields::Transfer_Encoding << ": " << "binary" << crlf;
                }
            }
            else {
                //TODO
            }
            machine.transition(transitions::EndResponseHeader);
            break;
        }
        case states::CRLFHeader:
        {
            stream << crlf;
            machine.transition(transitions::CRLFEnd);
            break;
        }
        case states::Body:
        {
            if(has_resource()) {
                std::copy(_resource.content().begin(), _resource.content().end(), std::ostream_iterator<char>(stream));
            }
            else {
                stream << _text;
            }
            stream << crlf;
            machine.transition(transitions::EndBody);
            break;
        }
        case states::CRLFBody:
        {
            stream << crlf;
            machine.transition(transitions::CRLFEnd);
            break;
        }
        default:
        {
            break;
        }
        }
    }

    return stream.str();
}

const Request &Response::getRequest() const
{
    return _request;
}
std::string Response::getText() const
{
    return _text;
}

void Response::setText(const std::string &text)
{
    _text = text;
}



bool Response::should_cache() const
{
    if(has_resource())
        return true;
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

Response::Response(const Request& request, const Json::Value &json) : _request(request), _code(200), _text(json.toStyledString()), _content_type(Components::ContentType::ApplicationJson)
{

}

