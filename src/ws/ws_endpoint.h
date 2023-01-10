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

    WsEndpoint(int id, websocketpp::connection_hdl hdl, std::string uri);

    void on_open(client *c, websocketpp::connection_hdl hdl);

    void on_fail(client *c, websocketpp::connection_hdl hdl);

    void on_close(client *c, websocketpp::connection_hdl hdl);

    void on_message(websocketpp::connection_hdl, client::message_ptr msg);

    void on_pong(websocketpp::connection_hdl, std::string msg);

    void on_pong_timeout(websocketpp::connection_hdl, std::string msg);

    websocketpp::connection_hdl get_hdl() const;

    int get_id() const;

    std::string get_status() const;

    void record_sent_message(std::string message);

    friend std::ostream &operator<<(std::ostream &out, WsEndpoint const &data);

private:
    int m_id;
    websocketpp::connection_hdl m_hdl;
    std::string m_status;
    std::string m_uri;
    std::string m_server;
    std::string m_error_reason;
    std::vector<std::string> m_messages;
};