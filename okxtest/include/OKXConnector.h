#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <boost/asio/ssl.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef websocketpp::client<websocketpp::config::asio_tls_client> client;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;
typedef std::shared_ptr<boost::asio::ssl::context> context_ptr;

class OKXConnector {
public:
    OKXConnector();
    void connect();
    void disconnect();
    ~OKXConnector() = default;
    OKXConnector(const OKXConnector& other) {
        this->m_con = other.m_con;
    }

    OKXConnector& operator=(const OKXConnector& other) {
        if (this != &other) {
            this->m_con = other.m_con;
        }
        return *this;
    }

private:
    client m_client;
    websocketpp::connection_hdl m_con;
    static context_ptr on_tls_init();
    static void on_open(client* c, websocketpp::connection_hdl hdl);
    void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
};
