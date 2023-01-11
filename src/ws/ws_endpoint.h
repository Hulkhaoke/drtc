#ifndef _WS_ENDPOINT_H_ 
#define _WS_ENDPOINT_H_ 

#include "websocketpp/config/asio_no_tls_client.hpp"
#include "websocketpp/client.hpp"
#include "websocketpp/common/thread.hpp"
#include "websocketpp/common/memory.hpp"

#include <string>
#include <sstream>

typedef websocketpp::client<websocketpp::config::asio_client> client;

class WsEndpoint
{
public:
    typedef websocketpp::lib::shared_ptr<WsEndpoint> ptr;

    WsEndpoint(websocketpp::connection_hdl hdl, std::string uri);

    void OnOpen(client *c, websocketpp::connection_hdl hdl);

    void OnFail(client *c, websocketpp::connection_hdl hdl);

    void OnClose(client *c, websocketpp::connection_hdl hdl);

    void OnMessage(websocketpp::connection_hdl, client::message_ptr msg);

    void OnPong(websocketpp::connection_hdl, std::string msg);

    void OnPongTimeout(websocketpp::connection_hdl, std::string msg);

    websocketpp::connection_hdl GetHdl() const;

    int GetId() const;

    std::string GetStatus() const;

    friend std::ostream &operator<<(std::ostream &out, WsEndpoint const &data);

private:
    websocketpp::connection_hdl m_hdl;
    std::string m_status;
    std::string m_uri;
    std::string m_server;
    std::string m_error_reason;
    std::vector<std::string> m_messages;
};

#endif