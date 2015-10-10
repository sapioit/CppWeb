#include <server/server.h>
#include <server/dispatcher.h>
#include <http/response.h>
#include <http/request.h>
#include <json/json.h>

#include <iostream>
int main() {
    try {
        Web::Server s(1234);
        Web::Dispatcher::routes.insert(
                    std::make_pair(std::make_pair(Http::Components::Method::Get,
                                                  "^\\/adsaf\\/json\\/(\\d+)$"),
                                   [](Http::Request req) -> Http::Response {
                        // /adsaf/json/<int>

                        Json::Value root(Json::arrayValue);
                        Json::Value records(Json::arrayValue);

                        Json::Value val;
                        val["this"] = "that";

                        records.append(val);

                        Json::Value a1(Json::arrayValue);
                        decltype(a1) a2(Json::arrayValue);

                        a1.append("1");
                        a1.append("2");

                        a2.append(req.uri_components().at(2));
                        a2.append("2");

                        records.append(a1);
                        records.append(a2);

                        root.append(records);

                        return {req, root};
                    }));

        Web::Dispatcher::routes.insert(std::make_pair(
                                           std::make_pair(Http::Components::Method::Get, "^\\/adsaf\\/json\\/$"),
                                           [](Http::Request req) -> Http::Response {
                                           // /adsaf/json/
                                           Json::Value root(Json::arrayValue);
                                           Json::Value records(Json::arrayValue);

                                           Json::Value a1(Json::arrayValue);
                                           decltype(a1) a2(Json::arrayValue);

                                           a1.append({"1"});
                                           a1.append({"2"});

                                           a2.append({"3"});
                                           a2.append({"4"});

                                           records.append(a1);
                                           records.append(a2);

                                           root.append(records);

                                           return {req, root};
                                       }));
        Settings settings;
#ifndef __arm__
        settings.root_path =
                ".";
#else
        settings.root_path = ".";
#endif
        settings.max_connections = 1000;
        s.setSettings(settings);
        s.run();
    } catch (std::exception& ex) {
        std::cerr << ex.what();
    }
    return 0;
}
