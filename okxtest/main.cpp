#include "OKXConnector.h"
#include "ComplexCalculator.h"
#include <thread>
#include <cmath>

double cosine_function(double x) {
    return std::cos(0.5 * x);
}


int main() {
    OKXConnection::OKXConnector connector;
    std::vector<CalcType> types = {CalcType::INTEGRATION, CalcType::GAUSS, CalcType::TRANSPOSITION};
    double a = 0;
    double b = M_PI * static_cast<double>(3.0 / 2);
    std::vector<std::vector<double>> B = {
            {8.0},
            {-11.0},
            {-3.0}
    };
    std::vector<std::vector<double>> A = { {1,2,3},
                                           {4,5,6},
                                           {7,8,9}
                                           };

    std::thread connectorThread([&]() {
        std::string uri = "wss://ws.okx.com:8443/ws/v5/public";
        connector.connect(uri);
        std::this_thread::sleep_for(std::chrono::seconds(5));
        for (int i = 0; i < 10; ++i) {
            connector.send_message("My message");
        }
    });
    std::thread thread([&]() {
        ComplexCalculator<double> complexCalc(types);
        complexCalc.calculate(A, B, a, b, cosine_function);

    });
    std::this_thread::sleep_for(std::chrono::seconds(5));
    for (int i = 0; i < 10; ++i) {
        connector.send_message("My message");
    }
    connectorThread.join();
    thread.join();
    connector.send_message("Hello, world");
    connector.disconnect();
    return 0;
}