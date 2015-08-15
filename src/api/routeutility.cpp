#include "routeutility.h"
#include <algorithm>
#include <regex>


std::function<Http::Response (Http::Request)> RouteUtility::GetHandler(const Http::Request &request, const std::map<std::string, std::function<Http::Response (Http::Request)>> &routes)
{
    auto result = std::find_if(routes.begin(), routes.end(), [&](const std::pair<std::string, std::function<Http::Response(Http::Request)>>& route) -> bool {
//        std::regex reg(route.first);
//        auto begin = std::sregex_iterator(request.URI.begin(), request.URI.end(), reg);
//        auto end = std::sregex_iterator();

//        if(std::distance(begin, end) == 0)
//            return false;
        if(route.first == request.URI)
            return true;
        return false;
    });

    if(result != routes.end()) {
        return result->second;
    }

    return nullptr;
}
