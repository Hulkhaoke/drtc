#include "ws_client.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

class WsReceiver:public WsClient
{
public:
    WsReceiver(){}
    ~WsReceiver(){}

    void OnReceiveMessage(const std::string &msg)
    {
        std::cout << "Receive msg: " << msg << std::endl;
    }
};

int main()
{
    bool done = false;
    std::string input;
    WsReceiver ws_client;

    std::cout << "connect ws://localhost:9002" << std::endl;
    ws_client.Connect("ws://localhost:9002");

    std::string status1 = ws_client.GetStatus();

    while("Open" != status1)
    {
        status1 = ws_client.GetStatus();
    }
    
    std::cout << "Connect successfully!" << std::endl;

    std::cout << "Send message [Hello]" << std::endl;
    ws_client.Send("Hello");

    std::cout << "Send ping" << std::endl;
    ws_client.Ping();

    std::cout << "Close conneciton" << std::endl;
    int close_code = websocketpp::close::status::normal;
    std::string reason = "User Close";
    ws_client.Close(close_code, reason);

    getchar();
    return 0;
}