#include "main.hpp"

constexpr bool isUsingSSL = false;
constexpr bool isServer = true;
constexpr int listenPort = 7000;
constexpr std::string_view MARKET_SNAPSHOT_SUB_STR = "market/snapshot";

using WebSocket = uWS::WebSocket<isUsingSSL, isServer, SocketData>;

void socketOpenCallback(WebSocket *webSocket) {
    if (webSocket->subscribe(MARKET_SNAPSHOT_SUB_STR)) {
        std::cout << "Client connected" << std::endl;
    }
}

void socketMessageCallback(WebSocket *webSocket, std::string_view message,
                           uWS::OpCode opCode) {
    std::cout << "Received message: " << message << std::endl;
    webSocket->send(message, opCode);
}

void socketCloseCallback(WebSocket * /*webSocket*/, int /*closeCode*/,
                         std::string_view /*message*/) {
    std::cout << "Client disconnected" << std::endl;
}

void socketListenCallback(us_listen_socket_t *listenSocket, uWS::App *app) {
    static auto timer = std::unique_ptr<us_timer_t, void (*)(us_timer_t *)>(
        nullptr, [](us_timer_t *t) { us_timer_close(t); });
    if (listenSocket) {
        int localPort =
            us_socket_local_port(isUsingSSL, (us_socket_t *)listenSocket);
        std::cout << "WebSocket server listening on port: " << localPort
                  << std::endl;
        struct us_loop_t *loop = (struct us_loop_t *)uWS::Loop::get();
        timer.reset(us_create_timer(loop, 0, sizeof(uWS::App *)));
        *((uWS::App **)us_timer_ext(timer.get())) = app;
        us_timer_set(
            timer.get(),
            [](struct us_timer_t *t) {
                uWS::App *app = *((uWS::App **)us_timer_ext(t));
                app->publish(MARKET_SNAPSHOT_SUB_STR, "SNAPSHOT DATA GOES HERE",
                             uWS::OpCode::TEXT);
            },
            1000, 1000);
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

    application.listen(listenPort, [&application](auto *listenSocket) {
        socketListenCallback(listenSocket, &application);
    });

    application.run();

    return 0;
}
