#ifndef ROUTES_H
#define ROUTES_H

#include <map>
#include "response.h"
#include <Request.h>

class RoutesContainer
{
    std::map<std::string, std::function<Http::Response(Http::Request)>> _routes;
public:
    RoutesContainer();
    void add(const std::string&, std::function<Http::Response(Http::Request)> action);
    const std::map<std::string, std::function<Http::Response (Http::Request)> > &get() const;
};

#endif // ROUTES_H
