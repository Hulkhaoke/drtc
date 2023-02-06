#include "ws_client.h"
#include "log.h"

WsClient::WsClient()
{
}

WsClient::~WsClient()
{
}

void WsClient::OnReceiveMessage(const std::string &msg)
{
    LOG_INFO("Receive msg: {}", msg);
}