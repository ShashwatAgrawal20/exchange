#pragma once

#include <include/main.hpp>

class WebSocketServer {
  public:
    explicit WebSocketServer(int port, const std::string &key_file,
                             const std::string &cert_file);
    void run(void);

    constexpr static bool IS_USING_SSL = true;
    constexpr static bool IS_SERVER = true;

  private:
    struct SocketData {};
    using WebSocket = uWS::WebSocket<IS_USING_SSL, IS_SERVER, SocketData>;

    void setup_routes(void);
    void on_listen(us_listen_socket_t *listen_socket);

    static void on_open(WebSocket *ws);
    static void on_message(WebSocket *ws, std::string_view message,
                           uWS::OpCode op_code);
    static void on_close(WebSocket *ws, int code, std::string_view message);

    uWS::SSLApp app_;
    int port_;
    std::unique_ptr<us_timer_t, void (*)(us_timer_t *)> broadcast_timer_;
};
