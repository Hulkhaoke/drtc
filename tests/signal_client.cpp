#include "ws_client.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

int main()
{
    bool done = false;
    std::string input;
    WsClient endpoint;

    std::cout << "connect ws://localhost:9002" << std::endl;
    int id = endpoint.connect("ws://localhost:9002");
    if (id != -1)
    {
        std::cout << "> Created connection with id " << id << std::endl;
    }

    std::string status1 = endpoint.get_status(id);
    // std::cout << "Connect status:" << status1 << std::end;

    Sleep(1000);

    std::string status2 = endpoint.get_status(id);
    // std::cout << "Connect status:" << status2 << std::end;

    std::cout << "Show conneciton[" << id << "] info" << std::endl;
    WsEndpoint::ptr metadata = endpoint.get_metadata(id);
    if (metadata)
    {
        std::cout << *metadata << std::endl;
    }
    else
    {
        std::cout << "> Unknown connection id " << id << std::endl;
    }

    std::cout << "Send message [Hello] to conneciton[" << id << "]" << std::endl;
    endpoint.send(id, "Hello");

    std::cout << "Send ping to conneciton[" << id << "]" << std::endl;
    endpoint.ping(id);

    std::cout << "Close conneciton[" << id << "]" << std::endl;
    int close_code = websocketpp::close::status::normal;
    std::string reason = "User Close";
    endpoint.close(id, close_code, reason);

    getchar();
    return 0;
}