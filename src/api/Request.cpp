//
// Created by vladimir on 08.08.2015.
//

#include <ostream>
#include "Request.h"
#include "components.h"
#include <regex>

using namespace Http;

const std::vector<Components::ContentType> &Request::accepted() const
{
    return _accepted;
}

void Request::setAccepted(const std::vector<Components::ContentType> &accepted)
{
    _accepted = accepted;
}
bool Http::Request::operator==(const Http::Request & other)
{
    bool bmethod = method == other.method;
    bool buri = URI == other.URI;
    bool bversion = version == other.version;
    bool bheader = header == other.header;
    bool bbody = body == other.body;
    return (bmethod && buri && bversion && bheader && bbody);
}

bool Http::Request::IsPassable() const
{
    switch(method) {
    case Components::Method::Get:
        return true;
    case Components::Method::Post:
        return true;
    case Components::Method::Put:
        return true;
    case Components::Method::Delete:
        return true;
    case Components::Method::Head:
        return true;
    default:
        return false;
    }
}

bool Http::Request::IsResource() const
{
    std::regex extensions(".*\\.(jpg|jpeg|png|gif|zip|pdf)$", std::regex::ECMAScript|std::regex::icase);
    bool match = std::regex_match(URI, extensions);
    return match;
}

