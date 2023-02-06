#include "signal_server.h"
#include "log.h"
#include <nlohmann/json.hpp>

using nlohmann::json;

static const std::map<std::string, unsigned int> siganl_types{
    {"create_transport", 1},
    {"offer", 2}};

std::string gen_random_6() {
    static const char alphanum[] =
        "0123456789";
    std::string tmp_s;
    tmp_s.reserve(6);

    for (int i = 0; i < 6; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

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
    LOG_INFO("New connection [{}] established", connection_id_++);

    json message = {
        {"type", "connection_id"},
        {"connection_id", connection_id_}
    };
    server_.send(hdl, message.dump(), websocketpp::frame::opcode::text);

    return true;
}

bool SignalServer::on_close(websocketpp::connection_hdl hdl)
{
    LOG_INFO("Connection [{}] closed", connection_id_++);
    connections_.erase(hdl);
    return true;
}

bool SignalServer::on_ping(websocketpp::connection_hdl hdl, std::string s)
{
    /* Do something */
    LOG_INFO("Receive ping");
    return true;
}

bool SignalServer::on_pong(websocketpp::connection_hdl hdl, std::string s)
{
    /* Do something */
    LOG_INFO("pong");
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

    auto j = json::parse(payload);

    std::string type = j["type"];
    auto itr = siganl_types.find(type);
    if (itr != siganl_types.end())
    {
        LOG_INFO("msg type :{}", itr->second);
        switch (itr->second)
        {
        case 1:
            transport_id_ = gen_random_6();
            LOG_INFO("Generate transport_id [{}]", transport_id_);
            break;
        case 2:
            // server_.send(hdl, msg->get_payload(), msg->get_opcode());
            // LOG_INFO("Message body:\n{}", sdp);
            break;
        default:
            break;
        }
    }
    
    // std::string sdp = j["sdp"];

    // LOG_INFO("Message type: {}", type);
    // LOG_INFO("Message body: {}", sdp);

    // server_.send(hdl, msg->get_payload(), msg->get_opcode());
}