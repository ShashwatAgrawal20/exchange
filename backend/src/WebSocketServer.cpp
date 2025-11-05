#include "include/WebSocketServer.hpp"
#include "include/MarketData.hpp"
#include <nlohmann/json.hpp>
#include <print>

namespace {
constexpr static std::string_view MARKET_SNAPSHOT_SUB_STR = "market/snapshot";

struct BroadcastData {
    uWS::SSLApp *app;
    size_t broadcast_left;
};

void broadcast_callback(us_timer_t *t) {
    if (t) {
        BroadcastData *data = static_cast<BroadcastData *>(us_timer_ext(t));
        if (data->broadcast_left > 0) {
            std::string snapshot = create_market_snapshot();
            data->app->publish(MARKET_SNAPSHOT_SUB_STR, snapshot,
                               uWS::OpCode::TEXT);

            data->broadcast_left--;
            std::println("Broadcast sent. {} remaining", data->broadcast_left);
        }
    }
};
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;
} // namespace

WebSocketServer::WebSocketServer(int port, const std::string &key_file,
                                 const std::string &cert_file)
    : key_file_(key_file), cert_file_(cert_file),
      app_({.key_file_name = key_file_.c_str(),
            .cert_file_name = cert_file_.c_str()}),
      port_(port), broadcast_timer_(nullptr,
                                    [](us_timer_t *t) {
                                        if (t)
                                            us_timer_close(t);
                                    }),
      auth_{} {
    setup_routes();
};

WebSocketServer::~WebSocketServer() { stop_broadcast_timer(); }

void WebSocketServer::setup_routes(void) {
    uWS::SSLApp::WebSocketBehavior<SocketData> ws_behaviour = {
        .open = [this](WebSocket *ws) { this->on_open(ws); },
        .message =
            [this](WebSocket *ws, std::string_view message,
                   uWS::OpCode op_code) {
                this->on_message(ws, message, op_code);
            },
        .close =
            [this](WebSocket *ws, int code, std::string_view message) {
                this->on_close(ws, code, message);
            }};
    app_.ws<SocketData>("/*", std::move(ws_behaviour));
}

void WebSocketServer::start_broadcast_timer(void) {
    if (broadcast_timer_ || app_.numSubscribers(MARKET_SNAPSHOT_SUB_STR) == 0)
        return;

    us_loop_t *loop = reinterpret_cast<us_loop_t *>(uWS::Loop::get());
    broadcast_timer_.reset(us_create_timer(loop, 0, sizeof(BroadcastData)));
    auto *data =
        static_cast<BroadcastData *>(us_timer_ext(broadcast_timer_.get()));
    data->app = &app_;
    data->broadcast_left = 10;
    us_timer_set(broadcast_timer_.get(), broadcast_callback, 1000, 1000);
}

void WebSocketServer::stop_broadcast_timer(void) {
    if (!broadcast_timer_)
        return;

    std::println("No subscribers left. Stopping broadcast timer.");
    broadcast_timer_.reset(nullptr);
}

void WebSocketServer::run(void) {
    app_.listen(
            port_,
            [this](us_listen_socket_t *listen_socket) {
                if (!listen_socket) {
                    std::println("webSocket server failed to listen on port {}",
                                 port_);
                    return;
                }
                std::println("WebSocket server listening on port: {}", port_);
            })
        .run();
    std::println("Server shutdown.");
}

void WebSocketServer::on_open(WebSocket *ws) {
    std::println("Client Connected");
}

/*
 * yeah this does way too much rn, but it works. don’t touch it till it breaks.
 */
void WebSocketServer::on_message(WebSocket *ws, std::string_view message,
                                 uWS::OpCode op_code) {
    SocketData *user = ws->getUserData();

    try {
        json msg = json::parse(message);
        std::string type = msg.value("type", "");
        if (type == "login") {
            std::string username = msg.value("username", "");
            std::string password = msg.value("password", "");

            if (auth_.authenticate(username, password)) {
                user->is_authenticated = true;
                user->username = username;
                ws->send(R"({"type":"login_response","status":"success"})",
                         uWS::OpCode::TEXT);
                std::println("[Auth] {} logged in successfully", username);

            } else {
                ws->send(R"({"type":"login_response","status":"failed"})",
                         uWS::OpCode::TEXT);
                std::println("[Auth Failed] Invalid credentials for {}",
                             username);
            }
            return;
        }

        if (!user->is_authenticated) {
            ws->send(R"({"type":"error","message":"not_authenticated"})",
                     uWS::OpCode::TEXT);
            return;
        }

        if (type == "subscribe") {
            std::string channel = msg.value("channel", "");
            if (channel == MARKET_SNAPSHOT_SUB_STR) {
                ws->subscribe(MARKET_SNAPSHOT_SUB_STR);
                start_broadcast_timer();
                std::println("[Sub] {} subscribed to {}", user->username,
                             channel);
                ordered_json response = {{"type", "subscribe_response"},
                                         {"status", "ok"},
                                         {"channel", MARKET_SNAPSHOT_SUB_STR}};
                ws->send(response.dump(), uWS::OpCode::TEXT);
            } else {
                ws->send(R"({"type":"error","message":"unknown_channel"})",
                         uWS::OpCode::TEXT);
            }
            return;
        }

        if (type == "unsubscribe") {
            std::string channel = msg.value("channel", "");
            if (channel == MARKET_SNAPSHOT_SUB_STR) {
                ws->unsubscribe(MARKET_SNAPSHOT_SUB_STR);
                if (app_.numSubscribers(MARKET_SNAPSHOT_SUB_STR) == 0) {
                    stop_broadcast_timer();
                }
                ordered_json response = {{"type", "unsubscribe_response"},
                                         {"status", "ok"},
                                         {"channel", MARKET_SNAPSHOT_SUB_STR}};
                ws->send(response.dump(), uWS::OpCode::TEXT);
            } else {
                ws->send(R"({"type":"error","message":"unknown_channel"})",
                         uWS::OpCode::TEXT);
            }
            return;
        }
        ws->send(R"({"type":"error","message":"unknown_command"})",
                 uWS::OpCode::TEXT);
    } catch (const std::exception &e) {
        std::println("[Error] Invalid JSON:{}", e.what());
        ws->send(R"({"type":"error","message":"invalid_json"})",
                 uWS::OpCode::TEXT);
    }
}

void WebSocketServer::on_close(WebSocket * /*ws*/, int /*code*/,
                               std::string_view /*message*/) {
    std::println("Client disconnected.");
    if (app_.numSubscribers(MARKET_SNAPSHOT_SUB_STR) == 0) {
        std::println("Last client disconnected. Stopping broadcast timer.");
        broadcast_timer_.reset(nullptr);
    }
}
