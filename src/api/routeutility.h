#ifndef ROUTESMANAGER_H
#define ROUTESMANAGER_H

#include "Request.h"
#include "response.h"
#include "global.h"
#include <map>


class RouteUtility
{
public:
    static std::function<Http::Response(Http::Request)> GetHandler(const Http::Request& request, const std::map<std::string, std::function<Http::Response(Http::Request)>> &routes);
};

#endif // ROUTESMANAGER_H
