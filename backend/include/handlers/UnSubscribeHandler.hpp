#pragma once

#include "include/WebSocketServer.hpp"
#include "nlohmann/json.hpp"

void UnSubscribeHandler(WebSocketServer *server, WebSocketServer::WebSocket *ws,
                        WebSocketServer::SocketData *user,
                        const nlohmann::json &msg);
