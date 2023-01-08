#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <functional>

typedef websocketpp::server<websocketpp::config::asio> server;

class SignalServer {
public:
    SignalServer() {
         // Set logging settings
        m_endpoint.set_error_channels(websocketpp::log::elevel::all);
        m_endpoint.set_access_channels(websocketpp::log::alevel::all ^ websocketpp::log::alevel::frame_payload);

        // Initialize Asio
        m_endpoint.init_asio();

        // Set the default message handler to the echo handler
        m_endpoint.set_message_handler(std::bind(
            &SignalServer::echo_handler, this,
            std::placeholders::_1, std::placeholders::_2
        ));

        m_endpoint.set_ping_handler(bind(
            &SignalServer::on_ping, this,
            std::placeholders::_1,std::placeholders::_2
        ));
        
        m_endpoint.set_pong_handler(bind(
            &SignalServer::on_pong, this,
            std::placeholders::_1,std::placeholders::_2
        ));
    }

    void echo_handler(websocketpp::connection_hdl hdl, server::message_ptr msg) {
        // write a new message
        std::cout << msg->get_payload() << " " << msg->get_opcode() << std::endl;
        // m_endpoint.send(hdl, msg->get_payload(), msg->get_opcode());
    }

    bool on_ping(websocketpp::connection_hdl hdl, std::string s)
    {
        /* Do something */
        std::cout << "ping" << std::endl;
        return true;
    }

    bool on_pong(websocketpp::connection_hdl hdl, std::string s)
    {
        /* Do something */
        std::cout << "pong" << std::endl;
        return true;
    }

    void run() {
        // Listen on port 9002
        m_endpoint.listen(9002);

        // Queues a connection accept operation
        m_endpoint.start_accept();

        // Start the Asio io_service run loop
        m_endpoint.run();
    }
private:
    server m_endpoint;
};

int main() {
    SignalServer s;
    // connect ws://localhost:9002
    s.run();
    return 0;
}
