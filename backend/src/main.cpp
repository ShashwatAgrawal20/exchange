#include "include/WebSocketServer.hpp"
#include <iostream>

int main() {
    constexpr int listen_port = 7000;

    try {
        WebSocketServer server(listen_port, "../certs/key.pem",
                               "../certs/cert.pem");
        server.run();
    } catch (const std::exception &e) {
        std::cerr << "An unhandled exception occurred: " << e.what()
                  << std::endl;
        return 1;
    }

    return 0;
}
