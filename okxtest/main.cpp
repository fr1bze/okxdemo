#include <iostream>
#include "OKXConnector.h"
#include "ComplexCalculator.h"
#include <cmath>
#include <future>
#include <thread>

using namespace OKXConnection;

double cosine_function(double x) {
    return std::cos(0.5 * x);
};

int main() {
    std::string uri = "wss://ws.okx.com:8443/ws/v5/public";
    std::vector<CalcType> types = {CalcType::INTEGRATION, CalcType::GAUSS, CalcType::TRANSPOSITION};
    std::vector<std::vector<double>> matrix = {
            {1,2,3},
            {4,5,6},
            {7,8,9}
    };
    auto* complexCalc = new ComplexCalculator<double>(types);
    complexCalc->setMatrix(matrix);
    std::vector<std::vector<double>> A = {
            {2.0, 1.0, -1.0},
            {-3.0, -1.0, 2.0},
            {-2.0, 1.0, 2.0}
    };
    std::vector<std::vector<double>> B = {
            {8.0},
            {-11.0},
            {-3.0}
    };
    Result<double> results;
    double a = 0;
    double b = M_PI;

    std::future<Result<double>> calculation_result = std::async(std::launch::async, [=]() {
        return complexCalc->calculate(A, B, a, b, cosine_function);
    });


    std::vector<std::future<void>> websocket_futures;

    websocket_futures.reserve(10);
    for (int i = 0; i < 10; ++i) {
        websocket_futures[i] = (std::async(std::launch::async, [=]() {
            OKXConnector connector;
            connector.connect(uri);
            connector.send_message("Hello, WebSocket!");
            std::this_thread::sleep_for(std::chrono::seconds(5));
            connector.disconnect();
        }));
    }

    for (auto& future : websocket_futures) {
        future.wait();
    }

    Result<double> results_values = calculation_result.get();

    return 0;
}
