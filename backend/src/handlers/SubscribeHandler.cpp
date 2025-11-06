#include "include/handlers/SubscribeHandler.hpp"
#include <print>

void SubscribeHandler(WebSocketServer *server, WebSocketServer::WebSocket *ws,
                      WebSocketServer::SocketData *user,
                      const nlohmann::json &msg) {
    using ordered_json = nlohmann::ordered_json;
    std::string channel = msg.value("channel", "");
    if (channel == WebSocketServer::MARKET_SNAPSHOT_SUB_STR) {
        ws->subscribe(WebSocketServer::MARKET_SNAPSHOT_SUB_STR);
        server->start_broadcast_timer();
        std::println("[Sub] {} subscribed to {}", user->username, channel);
        ordered_json response = {
            {"type", "subscribe_response"},
            {"status", "ok"},
            {"channel", WebSocketServer::MARKET_SNAPSHOT_SUB_STR}};
        ws->send(response.dump(), uWS::OpCode::TEXT);
    } else {
        ws->send(R"({"type":"error","message":"unknown_channel"})",
                 uWS::OpCode::TEXT);
    }
    return;
}
