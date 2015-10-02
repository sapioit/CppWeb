#ifndef ROUTESMANAGER_H
#define ROUTESMANAGER_H

#include <http/request.h>
#include <http/response.h>
#include <map>

class RouteUtility {
 public:
  static std::function<Http::Response(Http::Request)> GetHandler(
      const Http::Request& request,
      const std::map<std::pair<Http::Components::Method, std::string>,
                     std::function<Http::Response(Http::Request)> >& routes);
};

#endif  // ROUTESMANAGER_H
