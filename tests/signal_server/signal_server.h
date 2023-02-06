#ifndef _SIGNAL_SERVER_H_
#define _SIGNAL_SERVER_H_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <functional>
#include <map>
#include <string>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef unsigned int connection_id;
typedef std::string room_id;

class SignalServer {
public:
    SignalServer();
    ~SignalServer();

    bool on_open(websocketpp::connection_hdl hdl);

    bool on_close(websocketpp::connection_hdl hdl);

    bool on_ping(websocketpp::connection_hdl hdl, std::string s);

    bool on_pong(websocketpp::connection_hdl hdl, std::string s);

    void run();

    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);

private:
    server server_;
    std::map<websocketpp::connection_hdl, connection_id,
        std::owner_less<websocketpp::connection_hdl>> connections_;
    std::map<room_id, connection_id> rooms_;
    unsigned int connection_id_ = 0;
    std::string transport_id_ = "000000";
};


#endif