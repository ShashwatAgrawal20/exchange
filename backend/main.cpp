#include "main.hpp"

constexpr bool isUsingSSL = false;
constexpr bool isServer = true;
typedef uWS::WebSocket<isUsingSSL, isServer, SocketData> WebSocket;

void socketOpenCallback(WebSocket *webSocket) {
    std::cout << "Client connected" << std::endl;
}
void socketMessageCallback(WebSocket *webSocket, std::string_view message,
                           uWS::OpCode opCode) {
    std::cout << "Received message: " << message << std::endl;
    webSocket->send(message, opCode);
}
void socketCloseCallback(WebSocket *webSocket, int closeCode,
                         std::string_view message) {
    std::cout << "Client disconnected" << std::endl;
}
void socketListenCallback(us_listen_socket_t *listenSocket) {
    if (listenSocket) {
        int localPort =
            us_socket_local_port(isUsingSSL, (us_socket_t *)listenSocket);
        std::cout << "WebSocket server listening on port: " << localPort
                  << std::endl;
    } else {
        std::cout << "WebSocket server failed to listen" << std::endl;
    }
}
int main() {
    uWS::App application = uWS::App();

    uWS::App::WebSocketBehavior<SocketData> webSocketBehaviour = {};
    webSocketBehaviour.open = &socketOpenCallback;
    webSocketBehaviour.message = &socketMessageCallback;
    webSocketBehaviour.close = &socketCloseCallback;

    application.ws<SocketData>("/*", std::move(webSocketBehaviour));

    constexpr int listenPort = 7000;
    application.listen(listenPort, &socketListenCallback);
    application.run();

    return 0;
}
