#include "ws_client.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

WsClient::WsClient()
{
    m_endpoint_.clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint_.clear_error_channels(websocketpp::log::elevel::all);

    m_endpoint_.init_asio();
    m_endpoint_.start_perpetual();

    m_thread_ = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint_);
}

WsClient::~WsClient()
{
    m_endpoint_.stop_perpetual();

    if (metadata_ptr_->GetStatus() != "Open")
    {
        // Only close open connections
        return;
    }

    websocketpp::lib::error_code ec;
    m_endpoint_.close(metadata_ptr_->GetHdl(), websocketpp::close::status::going_away, "", ec);
    if (ec)
    {
        std::cout << "> Error closing connection " << ec.message() << std::endl;
    }

    m_thread_->join();
}

int WsClient::Connect(std::string const &uri)
{
    websocketpp::lib::error_code ec;

    client::connection_ptr con = m_endpoint_.get_connection(uri, ec);

    if (ec)
    {
        std::cout << "> Connect initialization error: " << ec.message() << std::endl;
        return -1;
    }

    metadata_ptr_ = websocketpp::lib::make_shared<WsEndpoint>(con->get_handle(), uri);

    con->set_open_handler(websocketpp::lib::bind(
        &WsEndpoint::OnOpen,
        metadata_ptr_,
        &m_endpoint_,
        websocketpp::lib::placeholders::_1));
    con->set_fail_handler(websocketpp::lib::bind(
        &WsEndpoint::OnFail,
        metadata_ptr_,
        &m_endpoint_,
        websocketpp::lib::placeholders::_1));
    con->set_close_handler(websocketpp::lib::bind(
        &WsEndpoint::OnClose,
        metadata_ptr_,
        &m_endpoint_,
        websocketpp::lib::placeholders::_1));
    con->set_message_handler(websocketpp::lib::bind(
        &WsEndpoint::OnMessage,
        metadata_ptr_,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2));

    // con->set_ping_handler(websocketpp::lib::bind(
    //     &WsEndpoint::on_ping,
    //     metadata_ptr_,
    //     websocketpp::lib::placeholders::_1,
    //     websocketpp::lib::placeholders::_2
    // ));

    con->set_pong_handler(websocketpp::lib::bind(
        &WsEndpoint::OnPong,
        metadata_ptr_,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2));

    con->set_pong_timeout(1000);

    con->set_pong_timeout_handler(websocketpp::lib::bind(
        &WsEndpoint::OnPongTimeout,
        metadata_ptr_,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2));

    m_endpoint_.connect(con);

    return 0;
}

void WsClient::Close(websocketpp::close::status::value code, std::string reason)
{
    websocketpp::lib::error_code ec;

    m_endpoint_.close(metadata_ptr_->GetHdl(), code, reason, ec);
    if (ec)
    {
        std::cout << "> Error initiating close: " << ec.message() << std::endl;
    }
}

void WsClient::Send(std::string message)
{
    websocketpp::lib::error_code ec;

    m_endpoint_.send(metadata_ptr_->GetHdl(), message, websocketpp::frame::opcode::text, ec);
    if (ec)
    {
        std::cout << "> Error sending message: " << ec.message() << std::endl;
        return;
    }
}

void WsClient::Ping()
{
    websocketpp::lib::error_code ec;

    std::string message = "ping";

    m_endpoint_.ping(metadata_ptr_->GetHdl(), message, ec);
    if (ec)
    {
        std::cout << "> Error sending ping " << std::endl;
        return;
    }
}

std::string WsClient::GetStatus()
{
    websocketpp::lib::error_code ec;

    return metadata_ptr_->GetStatus();
}

WsEndpoint::ptr WsClient::GetMetadata() const
{
    return metadata_ptr_;
}