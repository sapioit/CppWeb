#include "routesmanager.h"

bool RoutesManager::IsPassable(const Http::Request &request)
{
    //TODO check the method of the request, if it's GET, POST, PUT, DELETE, etc, it is passable
    return true;
}

std::function<Http::Response (Http::Request)> RoutesManager::GetHandler(const Http::Request &request, const RoutesContainer &routes)
{
    //TODO try to apply all the regular expression in the routes container on the request URI
    return nullptr;
}
