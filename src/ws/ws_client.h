#ifndef _WS_CLIENT_H_ 
#define _WS_CLIENT_H_ 

#include "ws_endpoint.h"

#include <map>
#include <string>

class WsClient
{
public:
    WsClient();

    ~WsClient();

    int Connect(std::string const &uri);

    void Close(websocketpp::close::status::value code, std::string reason);

    void Send(std::string message);

    void Ping();

    std::string GetStatus();

    WsEndpoint::ptr GetMetadata() const;

private:
    client m_endpoint_;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread_;
    WsEndpoint::ptr metadata_ptr_;
};

#endif