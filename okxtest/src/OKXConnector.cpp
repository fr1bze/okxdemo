#include "OKXConnector.h"

using namespace OKXConnection;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

OKXConnector::OKXConnector() {
    std::cout << "Created object connector" << std::endl;
}

void OKXConnector::on_message(websocketpp::connection_hdl hdl, client_t::message_ptr msg) {
    std::cout << "Received message: " << msg->get_payload() << " " << msg->get_header() << " " << msg->get_raw_payload() << std::endl;
}

void OKXConnector::connect(const std::string uri) {
    try {
        client.set_access_channels(websocketpp::log::alevel::all);
        client.clear_access_channels(websocketpp::log::alevel::frame_payload);
        client.set_error_channels(websocketpp::log::elevel::all);

        client.init_asio();

        client.set_tls_init_handler([&](websocketpp::connection_hdl hdl) {
            return on_tls_init(uri, hdl);
        });

        client.set_message_handler(std::bind(&OKXConnector::on_message, this, ::_1, ::_2));

        websocketpp::lib::error_code ec;
        client_t::connection_ptr con = client.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return;
        }

        client.connect(con);

        client.get_alog().write(websocketpp::log::alevel::app, "Connecting to " + uri);

//        try {
//            client_thread = std::thread([&]() {
                client.run();
         //   });
           // this->send_message("hello!");

//        } catch (websocketpp::exception const & e) {
//            std::cout << e.what() << std::endl;
//        }
} catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}

std::thread& OKXConnector::getClientThread() {
    return this->client_thread;
}

websocketpp::lib::shared_ptr<boost::asio::ssl::context> OKXConnector::on_tls_init(const std::string& uri, websocketpp::connection_hdl) {
    websocketpp::lib::shared_ptr<boost::asio::ssl::context> ctx = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    try {
        //ssl/tls params

        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);

        ctx->set_verify_mode(boost::asio::ssl::context::verify_none);
        // ctx->set_verify_mode(boost::asio::ssl::context::verify_peer);
//        ctx->set_verify_callback(websocketpp::lib::bind(&OKXConnector::verify_certificate, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));

    } catch (std::exception& e) {
        std::cout << "Error initializing TLS context: " << e.what() << std::endl;
    }
    return ctx;
}

void OKXConnector::send_message(const std::string &message) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!connection.expired()) {
        websocketpp::lib::error_code ec;
        client.send(connection, message, websocketpp::frame::opcode::text, ec);
        if (ec) {
            std::cout << "Error sending message: " << ec.message() << std::endl;
        }
    } else {
        std::cout << "Not connected to WebSocket server" << std::endl;
    }
}

void OKXConnector::disconnect() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!connection.expired()) {
        websocketpp::lib::error_code ec;
        auto conn = connection.lock();
        if (conn) {
            client.close(conn, websocketpp::close::status::going_away, "Closing connection", ec); // Вызов close() с корректным указателем
            if (ec) {
                std::cout << "Error closing connection: " << ec.message() << std::endl;
            }

            client_thread.join();
        }
    }
}