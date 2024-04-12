#include <iostream>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>
#include <thread>
#include <mutex>
//#include <nlohmann/json.hpp>
typedef websocketpp::client<websocketpp::config::asio_tls_client> client_t;

namespace OKXConnection {

    class OKXConnector {
    public:
        OKXConnector();
        void connect(const std::string uri);
        void disconnect();
        void send_message(const std::string& message);
        std::thread& getClientThread();
        bool stop_client_thread = false;
    private:
        client_t client;
        websocketpp::connection_hdl connection;
        std::thread client_thread;
        std::mutex mutex_;
        websocketpp::lib::shared_ptr<boost::asio::ssl::context> on_tls_init(const std::string& uri, websocketpp::connection_hdl);
        void on_message(websocketpp::connection_hdl hdl, client_t::message_ptr msg);
    };
}
