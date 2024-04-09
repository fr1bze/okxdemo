#pragma once
#ifndef OKXTEST_OKXCONNECTOR_H
#define OKXTEST_OKXCONNECTOR_H

#endif //OKXTEST_OKXCONNECTOR_H
#include <iostream>
#include <websocketpp/config/asio_no_tls_client.hpp>
//#include <websocketpp/config/asio_tls_client.hpp>
#include <websocketpp/client.hpp>

namespace OKXConnection {

class OKXConnector {
public:
    OKXConnector();
    void connect(const std::string uri);
    void disconnect();
    void send_message(const std::string& message);
private:
    typedef websocketpp::client<websocketpp::config::asio_client> client_t;
    client_t client;
    websocketpp::connection_hdl connection;
    std::thread client_thread;

};
}