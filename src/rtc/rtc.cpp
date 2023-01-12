#include "rtc.h"
#include "ice_agent.h"
#include "ws_client.h"
#include "peer_connection.h"

#include <iostream>

class WsSender:public WsClient
{
public:
    WsSender(){}
    ~WsSender(){}

    void OnReceiveMessage(const std::string &msg)
    {
        std::cout << "Receive msg: " << msg << std::endl;
    }
};

static WsSender *ws_client;

int rtc()
{
    ws_client = new WsSender();
    return 0;
}

int CreateWsClient(const char *uri)
{
    ws_client->Connect(uri);

    return 0;
}

int WsSendMsg(const char *message)
{
    ws_client->Send(message);

    return 0;
}

ws_status GetWsStatus()
{
    std::string ws_status = ws_client->GetStatus();

    if ("Connecting" == ws_status)
    {
        return ws_status::WS_CONNECTING;
    }
    else if ("Open" == ws_status)
    {
        return ws_status::WS_OPEN;
    }
    else if ("Failed" == ws_status)
    {
        return ws_status::WS_FAILED;
    }
    else if ("Closed" == ws_status)
    {
        return ws_status::WS_CLOSED;
    }
    else
    {
        return ws_status::WS_UNKNOWN;
    }
}