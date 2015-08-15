#include "Socket.h"
#include "Watcher.h"
#include "Request.h"
#include "Parser.h"
#include <iostream>

int main() {
    try {
        auto socket = std::make_shared<IO::Socket>(1234);
        (*socket).Bind();
        (*socket).MakeNonBlocking();
        (*socket).Listen(30);
        IO::Watcher watcher(socket);
        watcher.Start([](std::shared_ptr<IO::Socket> sock) {
            try {
                auto &&req = Http::Parser((*sock))();
                std::cout << "got request" << std::endl;

            }
            catch (std::exception &ex) {
                std::cout << ex.what() << std::endl;
            }


        });
    }
    catch (std::runtime_error &ex) {
        std::cout << ex.what() << std::endl;
    }
    return 0;
}
