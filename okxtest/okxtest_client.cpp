#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include "OKXConnector.h"
#include "ComplexCalculator.h"

void disconnect_connectors(std::vector<OKXConnector>& connectors) {
    std::this_thread::sleep_for(std::chrono::seconds(20));
    for (auto& connector : connectors) {
        connector.disconnect();
    }
}

int main() {
    const int num_connectors = 10;
    const int size = 100;

    std::vector<OKXConnector> connectors(num_connectors);

    std::vector<CalcType> types = {CalcType::INTEGRATION, CalcType::GAUSS, CalcType::TRANSPOSITION};
    std::vector<ComplexCalculator<double>> calcs(num_connectors, ComplexCalculator<double>(types));

    std::vector<std::vector<double>> A(size, std::vector<double>(size));
    std::vector<std::vector<double>> B(size, std::vector<double>(size));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            A[i][j] = dis(gen);
            B[i][j] = dis(gen);
        }
    }

    std::vector<std::thread> connector_threads;
    for (int i = 0; i < num_connectors; ++i) {
        connector_threads.emplace_back([&connectors, i]() {
            connectors[i].connect();
        });
    }

    std::vector<std::thread> calculation_threads;
    for (int i = 0; i < num_connectors; ++i) {
        calculation_threads.emplace_back([&calcs, &A, &B, i, size]() {
            double a = 0.0, b = 1.0;
            std::function<double(double)> function = [](double x) { return std::cos(0.5 * x); };
            calcs[i].calculate(A, B, a, b, function);
        });
    }

    std::thread disconnect_thread(disconnect_connectors, std::ref(connectors));

    for (auto& thread : connector_threads) {
        thread.join();
    }

    for (auto& thread : calculation_threads) {
        thread.join();
    }

    std::this_thread::sleep_for(std::chrono::seconds(20));

    disconnect_thread.join();
    return 0;
}
