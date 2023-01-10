#include "ws_client.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

WsClient::WsClient() : m_next_id(0)
{
    m_endpoint.clear_access_channels(websocketpp::log::alevel::all);
    m_endpoint.clear_error_channels(websocketpp::log::elevel::all);

    m_endpoint.init_asio();
    m_endpoint.start_perpetual();

    m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_endpoint);
}

WsClient::~WsClient()
{
    m_endpoint.stop_perpetual();

    for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it)
    {
        if (it->second->get_status() != "Open")
        {
            // Only close open connections
            continue;
        }

        std::cout << "> Closing connection " << it->second->get_id() << std::endl;

        websocketpp::lib::error_code ec;
        m_endpoint.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", ec);
        if (ec)
        {
            std::cout << "> Error closing connection " << it->second->get_id() << ": "
                      << ec.message() << std::endl;
        }
    }

    m_thread->join();
}

int WsClient::connect(std::string const &uri)
{
    websocketpp::lib::error_code ec;

    client::connection_ptr con = m_endpoint.get_connection(uri, ec);

    if (ec)
    {
        std::cout << "> Connect initialization error: " << ec.message() << std::endl;
        return -1;
    }

    int new_id = m_next_id++;
    WsEndpoint::ptr metadata_ptr = websocketpp::lib::make_shared<WsEndpoint>(new_id, con->get_handle(), uri);
    m_connection_list[new_id] = metadata_ptr;

    con->set_open_handler(websocketpp::lib::bind(
        &WsEndpoint::on_open,
        metadata_ptr,
        &m_endpoint,
        websocketpp::lib::placeholders::_1));
    con->set_fail_handler(websocketpp::lib::bind(
        &WsEndpoint::on_fail,
        metadata_ptr,
        &m_endpoint,
        websocketpp::lib::placeholders::_1));
    con->set_close_handler(websocketpp::lib::bind(
        &WsEndpoint::on_close,
        metadata_ptr,
        &m_endpoint,
        websocketpp::lib::placeholders::_1));
    con->set_message_handler(websocketpp::lib::bind(
        &WsEndpoint::on_message,
        metadata_ptr,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2));

    // con->set_ping_handler(websocketpp::lib::bind(
    //     &WsEndpoint::on_ping,
    //     metadata_ptr,
    //     websocketpp::lib::placeholders::_1,
    //     websocketpp::lib::placeholders::_2
    // ));

    con->set_pong_handler(websocketpp::lib::bind(
        &WsEndpoint::on_pong,
        metadata_ptr,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2));

    con->set_pong_timeout(1000);

    con->set_pong_timeout_handler(websocketpp::lib::bind(
        &WsEndpoint::on_pong_timeout,
        metadata_ptr,
        websocketpp::lib::placeholders::_1,
        websocketpp::lib::placeholders::_2));

    m_endpoint.connect(con);

    return new_id;
}

void WsClient::close(int id, websocketpp::close::status::value code, std::string reason)
{
    websocketpp::lib::error_code ec;

    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end())
    {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }

    m_endpoint.close(metadata_it->second->get_hdl(), code, reason, ec);
    if (ec)
    {
        std::cout << "> Error initiating close: " << ec.message() << std::endl;
    }
}

void WsClient::send(int id, std::string message)
{
    websocketpp::lib::error_code ec;

    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end())
    {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }

    m_endpoint.send(metadata_it->second->get_hdl(), message, websocketpp::frame::opcode::text, ec);
    if (ec)
    {
        std::cout << "> Error sending message: " << ec.message() << std::endl;
        return;
    }

    metadata_it->second->record_sent_message(message);
}

void WsClient::ping(int id)
{
    websocketpp::lib::error_code ec;

    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end())
    {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
    }

    std::string message = "ping";

    m_endpoint.ping(metadata_it->second->get_hdl(), message, ec);
    if (ec)
    {
        std::cout << "> Error sending ping " << std::endl;
        return;
    }
}

std::string WsClient::get_status(int id) const
{
    websocketpp::lib::error_code ec;

    con_list::iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end())
    {
        return std::string("No connection found with id");
    }

    return metadata_it->second->get_status();
}

WsEndpoint::ptr WsClient::get_metadata(int id) const
{
    con_list::const_iterator metadata_it = m_connection_list.find(id);
    if (metadata_it == m_connection_list.end())
    {
        return WsEndpoint::ptr();
    }
    else
    {
        return metadata_it->second;
    }
}