#include "Socket.h"
#include "Watcher.h"
#include "Request.h"
#include "Parser.h"
#include "server.h"
#include "dispatcher.h"
#include "response.h"
#include <iostream>

int main() {
    try {
        Web::Server s(1234);
        Web::Dispatcher::routes.insert(std::make_pair("^\\/adsaf\\/json\\/(\\d+)$",[](Http::Request req) -> Http::Response {
                                           // /adsaf/json/<int>
                                           Http::Response response(req, 200);
                                           response.setContent_type(Http::Components::ContentType::ApplicationJson);
                                           response.setText("{\"records\": [{\"Name\" : \"" + req.uri_components()[2] + "\",\"City\" : \"Mega-Bla\", \"Country\" : \"Blaaaa!\"}]}");

                                           return response;
                                       }));
        Web::Dispatcher::routes.insert(std::make_pair("^\\/adsaf\\/json\\/$",[](Http::Request req) -> Http::Response {
                                           // /adsaf/json/
                                           Http::Response response(req, 200);
                                           response.setContent_type(Http::Components::ContentType::ApplicationJson);
                                           response.setText("{\"records\": [{\"Name\" : \"asdf\",\"City\" : \"Mega-Bla\", \"Country\" : \"Blaaaa!\"}]}");

                                           return response;
                                       }));
        s.run();
    }
    catch(std::exception &ex) {
        std::cerr << ex.what();
    }
    return 0;
}
