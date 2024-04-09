#include "OKXConnector.h"

using namespace OKXConnection;

OKXConnector::OKXConnector() {
    std::cout << "Created object connector" << std::endl;
}

void OKXConnector::connect(const std::string uri) {
    websocketpp::lib::error_code ec;

    websocketpp::client<websocketpp::config::asio_client>::connection_ptr connection_ptr = client.get_connection(uri, ec);
    if (ec) {
        std::cout << "Error connecting: " << ec.message() << std::endl;
        return;
    }

    client.set_open_handler([&](websocketpp::connection_hdl hdl) {
        std::cout << "Connected to WebSocket server" << std::endl;
        connection = hdl;
    });

    client.connect(connection_ptr);
    if (ec) {
        std::cout << "Error connecting: " << ec.message() << std::endl;
        return;
    }

    client_thread = std::thread([&]() {
        client.run();
    });
}

void OKXConnector::send_message(const std::string &message) {
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
    if (!connection.expired()) {
        websocketpp::lib::error_code ec;
        client.close(connection, websocketpp::close::status::going_away, "Closing connection", ec);
        if (ec) {
            std::cout << "Error closing connection: " << ec.message() << std::endl;
        }

        client_thread.join();
    }
}
