#include "signal_server.h"

SignalServer::SignalServer()
{
    // Set logging settings
    m_endpoint_.set_error_channels(websocketpp::log::elevel::all);
    m_endpoint_.set_access_channels(websocketpp::log::alevel::none);

    // Initialize Asio
    m_endpoint_.init_asio();

    // Set the default message handler to the echo handler
    m_endpoint_.set_message_handler(std::bind(
        &SignalServer::on_message, this,
        std::placeholders::_1, std::placeholders::_2));

    m_endpoint_.set_ping_handler(bind(
        &SignalServer::on_ping, this,
        std::placeholders::_1, std::placeholders::_2));

    m_endpoint_.set_pong_handler(bind(
        &SignalServer::on_pong, this,
        std::placeholders::_1, std::placeholders::_2));
}

SignalServer::~SignalServer()
{

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
    m_endpoint_.listen(9002);

    // Queues a connection accept operation
    m_endpoint_.start_accept();

    // Start the Asio io_service run loop
    m_endpoint_.run();
}

void SignalServer::on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    // write a new message
    // std::cout << "Receive message: [ " << msg->get_payload() << " ]" << std::endl;
    // m_endpoint_.send(hdl, msg->get_payload(), msg->get_opcode());
    message_proc_.Process(msg->get_payload());
}