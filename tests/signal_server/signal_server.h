#ifndef _SIGNAL_SERVER_H_
#define _SIGNAL_SERVER_H_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <functional>
#include "message_proc.h"

typedef websocketpp::server<websocketpp::config::asio> server;

class SignalServer {
public:
    SignalServer();
    ~SignalServer();

    bool on_ping(websocketpp::connection_hdl hdl, std::string s);

    bool on_pong(websocketpp::connection_hdl hdl, std::string s);

    void run();

    void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);

private:
    server m_endpoint_;
    MessageProc message_proc_;
};

#endif