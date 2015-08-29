#include <server/dispatcher.h>
#include <http/parser.h>
#include <http/routeutility.h>
#include <http/parser.h>
#include <http/components.h>
#include <http/cachemanager.h>
#include <http/responsemanager.h>
#include <misc/log.h>
#include <misc/storage.h>

#include <thread>
#include <sstream>

std::map<std::string, std::function<Http::Response(Http::Request)> >
    Web::Dispatcher::routes;

using namespace Web;

bool Dispatcher::Dispatch(IO::Socket& connection) {
  try {
    auto&& request = Http::Parser(connection)();
    if (request.IsPassable()) {
      if (!request.IsResource()) {
        auto handler = RouteUtility::GetHandler(request, routes);
        if (handler) {
          request.setUri_components(
              Http::Parser::Split(Http::Parser::StripRoute(request.URI), '/'));
          auto response = handler(request);
          ResponseManager::Respond(response, connection);
          return response.should_close();
        } else {
          // no user-defined handler, return not found
          ResponseManager::Respond({ request, 404 }, connection);
          return true;
        }
      } else {
        // it's a resource
        try {
          auto&& resource = CacheManager::GetResource(request.URI.c_str());
          Http::Response resp{ request, resource };
          resp.setContent_type(
              Http::Parser::GetMimeTypeByExtension(request.URI));
          ResponseManager::Respond(resp, connection);
          return resp.should_close();
        } catch (int code) {
          ResponseManager::Respond({ request, code }, connection);
          return false;
        }
      }
    } else {
      // not passable
    }
  } catch (std::runtime_error& ex) {
    Log::e(ex.what());
  }
  return true;
}

void Dispatcher::PassToUser(
    Http::Request request,
    std::function<Http::Response(Http::Request)> user_handler,
    IO::Socket& socket) {
  auto response = user_handler(request);
  ResponseManager::Respond(std::move(response), socket);
}
