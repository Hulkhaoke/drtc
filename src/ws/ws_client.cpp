#include "ws_client.h"

WsClient::WsClient()
{
}

WsClient::~WsClient()
{
}

void WsClient::OnReceiveMessage(const std::string &msg)
{
    std::cout << "Receive msg: " << msg << std::endl;
}