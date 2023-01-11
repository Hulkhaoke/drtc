#include "ws_endpoint.h"

#include <cstdlib>
#include <iostream>
#include <map>

WsEndpoint::WsEndpoint(websocketpp::connection_hdl hdl, std::string uri)
    : m_hdl(hdl), m_status("Connecting"), m_uri(uri), m_server("N/A")
{
}

void WsEndpoint::OnOpen(client *c, websocketpp::connection_hdl hdl)
{
    m_status = "Open";

    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");
}

void WsEndpoint::OnFail(client *c, websocketpp::connection_hdl hdl)
{
    m_status = "Failed";

    client::connection_ptr con = c->get_con_from_hdl(hdl);
    m_server = con->get_response_header("Server");
    m_error_reason = con->get_ec().message();
}

void WsEndpoint::OnClose(client *c, websocketpp::connection_hdl hdl)
{
    m_status = "Closed";
    client::connection_ptr con = c->get_con_from_hdl(hdl);
    std::stringstream s;
    s << "close code: " << con->get_remote_close_code() << " ("
      << websocketpp::close::status::get_string(con->get_remote_close_code())
      << "), close reason: " << con->get_remote_close_reason();
    m_error_reason = s.str();
}

void WsEndpoint::OnMessage(websocketpp::connection_hdl, client::message_ptr msg)
{
    std::cout << msg->get_payload() << std::endl;
    if (msg->get_opcode() == websocketpp::frame::opcode::text)
    {
        m_messages.push_back("<< " + msg->get_payload());
    }
    else
    {
        m_messages.push_back("<< " + websocketpp::utility::to_hex(msg->get_payload()));
    }
}

void WsEndpoint::OnPong(websocketpp::connection_hdl, std::string msg)
{
    std::cout << "on_pong" << std::endl;
}

void WsEndpoint::OnPongTimeout(websocketpp::connection_hdl, std::string msg)
{
    std::cout << "timeout" << std::endl;
}

websocketpp::connection_hdl WsEndpoint::GetHdl() const
{
    return m_hdl;
}

std::string WsEndpoint::GetStatus() const
{
    return m_status;
}

std::ostream &operator<<(std::ostream &out, WsEndpoint const &data)
{
    out << "> URI: " << data.m_uri << "\n"
        << "> Status: " << data.m_status << "\n"
        << "> Remote Server: " << (data.m_server.empty() ? "None Specified" : data.m_server) << "\n"
        << "> Error/close reason: " << (data.m_error_reason.empty() ? "N/A" : data.m_error_reason) << "\n";
    out << "> Messages Processed: (" << data.m_messages.size() << ") \n";

    std::vector<std::string>::const_iterator it;
    for (it = data.m_messages.begin(); it != data.m_messages.end(); ++it)
    {
        out << *it << "\n";
    }

    return out;
}