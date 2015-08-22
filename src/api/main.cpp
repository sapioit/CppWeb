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
        Web::Dispatcher::routes.insert(std::make_pair("/adsaf",[](Http::Request req) -> Http::Response {

                                           return {req, 200};
                                       }));
        s.run();
    }
    catch(std::exception &ex) {
        std::cerr << ex.what();
    }
    return 0;
}
