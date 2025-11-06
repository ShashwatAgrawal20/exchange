#include "include/handlers/LoginHandler.hpp"
#include <print>

void LoginHandler(WebSocketServer *server, WebSocketServer::WebSocket *ws,
                  WebSocketServer::SocketData *user,
                  const nlohmann::json &msg) {
    using json = nlohmann::json;

    std::string username = msg.value("username", "");
    std::string password = msg.value("password", "");

    if (user->is_authenticated) {
        std::println("[Auth] {} attempted to log in again, ignoring.",
                     user->username);
        ws->send(R"({"type":"login_response","status":"already_logged_in"})",
                 uWS::OpCode::TEXT);
        return;
    }
    if (server->auth().authenticate(username, password)) {
        user->is_authenticated = true;
        user->username = username;

        ws->send(R"({"type":"login_response","status":"success"})",
                 uWS::OpCode::TEXT);
        std::println("[Auth] {} logged in successfully", username);
    } else {
        ws->send(R"({"type":"login_response","status":"failed"})",
                 uWS::OpCode::TEXT);
        std::println("[Auth Failed] Invalid credentials for {}", username);
    }
}
