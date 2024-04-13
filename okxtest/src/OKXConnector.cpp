#include "OKXConnector.h"
OKXConnector::OKXConnector() {}
void OKXConnector::connect() {
    std::string uri = "wss://ws.okx.com:8443/ws/v5/public";
    try {
        m_client.set_access_channels(websocketpp::log::alevel::all);
        m_client.clear_access_channels(websocketpp::log::alevel::frame_payload);
        m_client.init_asio();
        m_client.set_tls_init_handler(std::bind(&OKXConnector::on_tls_init));
        m_client.set_open_handler(std::bind(&OKXConnector::on_open, &m_client, _1));
        m_client.set_message_handler(std::bind(&OKXConnector::on_message, this, _1, _2));
        websocketpp::lib::error_code ec;
        auto con = m_client.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
        } else {
            m_con = con->get_handle();
            m_client.connect(con);
            m_client.run();
        }
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}

context_ptr OKXConnector::on_tls_init() {
    context_ptr ctx = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    try {
        ctx->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);
        ctx->set_verify_mode(boost::asio::ssl::context::verify_none);
    } catch (std::exception& e) {
        std::cout << "Error initializing TLS context: " << e.what() << std::endl;
    }
    return ctx;
}

void OKXConnector::on_open(client* c, websocketpp::connection_hdl hdl) {
    std::string msg = R"({"op": "subscribe", "args": [{"channel": "mark-price", "instId": "BTC-USDT"}]})";
    c->send(hdl, msg, websocketpp::frame::opcode::text);
}

void OKXConnector::on_message(websocketpp::connection_hdl hdl, message_ptr msg) {
    try {
        json data = json::parse(msg->get_payload());
        if (data.contains("event")) {
            std::string ev = data["event"].get<std::string>();
            if (!ev.empty()) {
                std::cout << "---- event " << ev << " = " << data["arg"] << " -----" << std::endl;
            }
        } else if (data.contains("data") && data["data"].is_array() && !data["data"].empty()) {
            json symbol = data["data"][0]["instId"];
            double price = 0.0;
            if (data["data"][0]["markPx"].is_number()) {
                price = data["data"][0]["markPx"].get<double>();
            } else if (data["data"][0]["markPx"].is_string()) {
                price = std::stod(data["data"][0]["markPx"].get<std::string>());
            }
            std::cout << symbol << " = " << price << std::endl;
        }
    } catch (const json::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
    }
}

void OKXConnector::disconnect() {
    if (!m_con.expired()) {
        m_client.stop();
        m_client.stop_perpetual();
    }

}