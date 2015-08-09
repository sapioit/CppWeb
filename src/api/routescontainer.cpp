#include "routescontainer.h"

RoutesContainer::RoutesContainer()
{

}

void RoutesContainer::add(const std::string& regex, std::function<Http::Response(Http::Request)> action)
{
    _routes[regex] = action;
}

const std::map<std::string, std::function<Http::Response (Http::Request)> > &RoutesContainer::get() const
{
    return _routes;
}
