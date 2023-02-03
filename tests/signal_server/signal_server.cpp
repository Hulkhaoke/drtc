#include "signal_server.h"
#include <nlohmann/json.hpp>

using nlohmann::json;

SignalServer::SignalServer()
{
    // Set logging settings
    server_.set_error_channels(websocketpp::log::elevel::all);
    server_.set_access_channels(websocketpp::log::alevel::none);

    // Initialize Asio
    server_.init_asio();

    server_.set_open_handler(std::bind(
        &SignalServer::on_open, this,
        std::placeholders::_1));

    server_.set_close_handler(std::bind(
        &SignalServer::on_close, this,
        std::placeholders::_1));

    server_.set_message_handler(std::bind(
        &SignalServer::on_message, this,
        std::placeholders::_1, std::placeholders::_2));

    server_.set_ping_handler(bind(
        &SignalServer::on_ping, this,
        std::placeholders::_1, std::placeholders::_2));

    server_.set_pong_handler(bind(
        &SignalServer::on_pong, this,
        std::placeholders::_1, std::placeholders::_2));
}

SignalServer::~SignalServer()
{

}

bool SignalServer::on_open(websocketpp::connection_hdl hdl)
{
    connections_[hdl] = connection_id_;
    std::cout << "New connection [" << connection_id_++ << "] established" << std::endl;
    return true;
}

bool SignalServer::on_close(websocketpp::connection_hdl hdl)
{
    std::cout << "Connection [" << connections_[hdl] << "] closed" << std::endl;
    connections_.erase(hdl);
    return true;
}

bool SignalServer::on_ping(websocketpp::connection_hdl hdl, std::string s)
{
    /* Do something */
    std::cout << "Receive ping" << std::endl;
    return true;
}

bool SignalServer::on_pong(websocketpp::connection_hdl hdl, std::string s)
{
    /* Do something */
    std::cout << "pong" << std::endl;
    return true;
}

void SignalServer::run()
{
    // Listen on port 9002
    server_.listen(9002);

    // Queues a connection accept operation
    server_.start_accept();

    // Start the Asio io_service run loop
    server_.run();
}

void SignalServer::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    std::string payload = msg->get_payload();
    std::cout << "Receive message: [ " << payload << " ]" << std::endl;
    auto j = json::parse(payload);

    std::string type = j["type"];

    if("offer" == type)
    {
        server_.send(hdl, msg->get_payload(), msg->get_opcode());
    }
    
    // std::string sdp = j["sdp"];

    // std::cout << "Message type: " << type << std::endl;
    // std::cout << "Message body: " << sdp << std::endl;

    // server_.send(hdl, msg->get_payload(), msg->get_opcode());
}