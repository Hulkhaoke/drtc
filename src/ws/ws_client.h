#include "ws_endpoint.h"

#include <map>
#include <string>

class WsClient
{
public:
    WsClient();

    ~WsClient();

    int connect(std::string const &uri);

    void close(int id, websocketpp::close::status::value code, std::string reason);

    void send(int id, std::string message);

    void ping(int id);

    std::string get_status(int id);

    WsEndpoint::ptr get_metadata(int id) const;

private:
    typedef std::map<int, WsEndpoint::ptr> con_list;

    client m_endpoint;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;

    con_list m_connection_list;
    int m_next_id;
};