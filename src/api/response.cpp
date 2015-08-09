#include "response.h"
#include <json/json.h>

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

std::ostream& operator<<(std::ostream &o, const Response &r)
{
       return o;
}

