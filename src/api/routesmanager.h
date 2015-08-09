#ifndef ROUTESMANAGER_H
#define ROUTESMANAGER_H

#include "Request.h"
#include "routescontainer.h"


class RoutesManager
{
public:
    static bool IsPassable(const Http::Request& request);
    static std::function<Http::Response(Http::Request)> GetHandler(const Http::Request& request, const RoutesContainer &routes);
};

#endif // ROUTESMANAGER_H
