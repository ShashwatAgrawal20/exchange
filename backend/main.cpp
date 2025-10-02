#include <external/uwebsockets/src/App.h>
#include <iostream>
#include <string_view>

int main() {
    /* ws->getUserData returns one of these */
    struct PerSocketData {
        /* Fill with user data */
    };

    constexpr int port = 7000;
    uWS::App()
        .ws<PerSocketData>(
            "/*",
            {.open = [](auto * /*ws*/) { std::cout << "Client connected.\n"; },
             .message =
                 [](auto *ws, std::string_view message, uWS::OpCode opCode) {
                     std::cout << "Received message: " << message << "\n";
                     ws->send(message, opCode);
                 },

             .close =
                 [](auto * /*ws*/, int /*code*/, std::string_view /*message*/) {
                     std::cout << "Client disconnected.\n";
                 }})
        .listen(port,
                [](auto *listen_socket) {
                    if (listen_socket) {
                        std::cout << "WebSocket server listening on port "
                                  << port << "\n";
                    } else {
                        std::cerr << "Failed to listen on port " << port
                                  << "\n";
                    }
                })
        .run();

    return 0;
}
