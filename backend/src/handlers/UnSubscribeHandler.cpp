#include "include/handlers/UnSubscribeHandler.hpp"

void UnSubscribeHandler(WebSocketServer *server, WebSocketServer::WebSocket *ws,
                        WebSocketServer::SocketData *user,
                        const nlohmann::json &msg) {

    using ordered_json = nlohmann::ordered_json;
    std::string channel = msg.value("channel", "");
    if (channel == WebSocketServer::MARKET_SNAPSHOT_SUB_STR) {
        ws->unsubscribe(WebSocketServer::MARKET_SNAPSHOT_SUB_STR);
        if (server->app().numSubscribers(
                WebSocketServer::MARKET_SNAPSHOT_SUB_STR) == 0) {
            server->stop_broadcast_timer();
        }
        ordered_json response = {
            {"type", "unsubscribe_response"},
            {"status", "ok"},
            {"channel", WebSocketServer::MARKET_SNAPSHOT_SUB_STR}};
        ws->send(response.dump(), uWS::OpCode::TEXT);
    } else {
        ws->send(R"({"type":"error","message":"unknown_channel"})",
                 uWS::OpCode::TEXT);
    }
    return;
}
