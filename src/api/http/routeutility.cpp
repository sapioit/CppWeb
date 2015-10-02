#include "routeutility.h"
#include "parser.h"
#include <algorithm>
#include <regex>

std::function<Http::Response(Http::Request)> RouteUtility::GetHandler(
    const Http::Request& request,
    const std::map<std::pair<Http::Components::Method, std::string>,
                   std::function<Http::Response(Http::Request)>>& routes) {
  auto strippedRoute = Http::Parser::StripRoute(request.URI);
  auto result = std::find_if(
      routes.begin(), routes.end(),
      [&](const std::pair<std::pair<Http::Components::Method, std::string>,
                          std::function<Http::Response(Http::Request)>>& route)
          -> bool {
            auto& method = route.first.first;
            if (request.method != method) return false;
            auto& pattern = route.first.second;
            std::regex regex(pattern);
            return std::regex_match(strippedRoute, regex);
          });

  if (result != routes.end()) {
    return result->second;
  }

  return nullptr;
}
