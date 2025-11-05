#pragma once

#include "external/uwebsockets/src/App.h"
#include "include/AuthManager.hpp"

#include <memory>
#include <string>
#include <string_view>

class WebSocketServer {
  public:
    explicit WebSocketServer(int port, const std::string &key_file,
                             const std::string &cert_file);
    void run(void);

    constexpr static bool IS_USING_SSL = true;
    constexpr static bool IS_SERVER = true;
    ~WebSocketServer();

  private:
    struct SocketData {
        bool is_authenticated = false;
        std::string username;
    };
    using WebSocket = uWS::WebSocket<IS_USING_SSL, IS_SERVER, SocketData>;

    void setup_routes(void);

    void on_open(WebSocket *ws);
    void on_message(WebSocket *ws, std::string_view message,
                    uWS::OpCode op_code);
    void on_close(WebSocket *ws, int code, std::string_view message);

    void start_broadcast_timer(void);
    void stop_broadcast_timer(void);

    AuthManager auth_;
    std::string key_file_;
    std::string cert_file_;
    uWS::SSLApp app_;
    int port_;
    std::unique_ptr<us_timer_t, void (*)(us_timer_t *)> broadcast_timer_;
};
