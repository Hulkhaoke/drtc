#include "ws_client.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

int main()
{
    bool done = false;
    std::string input;
    WsClient ws_client;

    std::cout << "connect ws://localhost:9002" << std::endl;
    int id = ws_client.connect("ws://localhost:9002");
    if (id != -1)
    {
        std::cout << "Created connection with id " << id << std::endl;
    }

    std::string status1 = ws_client.get_status(id);

    while("Open" != status1)
    {
        status1 = ws_client.get_status(id);
    }
    
    std::cout << "Connect successfully!" << std::endl;

    std::cout << "Show conneciton[" << id << "] info" << std::endl;
    WsEndpoint::ptr metadata = ws_client.get_metadata(id);
    if (metadata)
    {
        std::cout << *metadata << std::endl;
    }
    else
    {
        std::cout << "> Unknown connection id " << id << std::endl;
    }

    std::cout << "Send message [Hello] to conneciton[" << id << "]" << std::endl;
    ws_client.send(id, "Hello");

    std::cout << "Send ping to conneciton[" << id << "]" << std::endl;
    ws_client.ping(id);

    std::cout << "Close conneciton[" << id << "]" << std::endl;
    int close_code = websocketpp::close::status::normal;
    std::string reason = "User Close";
    ws_client.close(id, close_code, reason);

    getchar();
    return 0;
}