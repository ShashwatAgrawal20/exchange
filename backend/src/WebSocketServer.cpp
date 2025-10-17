#include "include/WebSocketServer.hpp"
#include "include/MarketData.hpp"

#include <iostream>

namespace {
constexpr static std::string_view MARKET_SNAPSHOT_SUB_STR = "market/snapshot";

void broadcast_callback(us_timer_t *t) {
    if (t) {
        uWS::SSLApp *app = *((uWS::SSLApp **)us_timer_ext(t));
        std::string snapshot = create_market_snapshot();
        app->publish(MARKET_SNAPSHOT_SUB_STR, snapshot, uWS::OpCode::TEXT);
    }
}

} // namespace

WebSocketServer::WebSocketServer(int port, const std::string &key_file,
                                 const std::string &cert_file)
    : key_file_(key_file), cert_file_(cert_file),
      app_({.key_file_name = key_file_.c_str(),
            .cert_file_name = cert_file_.c_str()}),
      port_(port), broadcast_timer_(nullptr, [](us_timer_t *t) {
          if (t)
              us_timer_close(t);
      }) {
    setup_routes();
};

void WebSocketServer::setup_routes(void) {
    uWS::SSLApp::WebSocketBehavior<SocketData> ws_behaviour = {
        .open = on_open,
        .message = on_message,
        .close = on_close,
    };
    app_.ws<SocketData>("/*", std::move(ws_behaviour));
}

void WebSocketServer::run(void) {
    app_.listen(port_,
                [this](us_listen_socket_t *listen_socket) {
                    this->on_listen(listen_socket);
                })
        .run();
    std::cout << "Server shutdown." << std::endl;
}

void WebSocketServer::on_listen(us_listen_socket_t *listen_socket) {
    if (!listen_socket) {
        std::cerr << "webSocket server failed to listen on port " << port_
                  << std::endl;
        return;
    }
    std::cout << "WebSocket server listening on port: " << port_ << std::endl;

    us_loop_t *loop = (us_loop_t *)uWS::Loop::get();
    broadcast_timer_.reset(us_create_timer(loop, 0, sizeof(uWS::App *)));
    *((uWS::SSLApp **)us_timer_ext(broadcast_timer_.get())) = &app_;
    us_timer_set(broadcast_timer_.get(), broadcast_callback, 1000, 1000);
}

void WebSocketServer::on_open(WebSocket *ws) {
    std::cout << "Client Connected" << std::endl;
    ws->subscribe(MARKET_SNAPSHOT_SUB_STR);
}

void WebSocketServer::on_message(WebSocket *ws, std::string_view message,
                                 uWS::OpCode op_code) {
    std::cout << "Received message: " << message << std::endl;
    ws->send(message, op_code);
}

void WebSocketServer::on_close(WebSocket * /*ws*/, int /*code*/,
                               std::string_view /*message*/) {
    std::cout << "Client disconnected." << std::endl;
}
