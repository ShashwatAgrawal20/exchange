#pragma once

#include "include/WebSocketServer.hpp"
#include "nlohmann/json.hpp"

void SubscribeHandler(WebSocketServer *server, WebSocketServer::WebSocket *ws,
                      WebSocketServer::SocketData *user,
                      const nlohmann::json &msg);
