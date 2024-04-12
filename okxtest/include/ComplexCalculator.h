#ifndef OKXTEST_COMPLEXCALCULATOR_H
#define OKXTEST_COMPLEXCALCULATOR_H

#endif //OKXTEST_COMPLEXCALCULATOR_H
#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include <chrono>

enum class CalcType {
    GAUSS,
    INTEGRATION,
    TRANSPOSITION
};

template <typename T>
struct Result {
    std::vector<std::vector<T>> gaussResult;
    double integrationResult;
    std::vector<std::vector<T>> transpositionResult;
};

template <typename T>
class ComplexCalculator {
public:
    explicit ComplexCalculator(std::vector<CalcType> types);
    std::vector<std::vector<T>> gaussCalc(std::vector<std::vector<T>> A, std::vector<std::vector<T>> B);
    T integration(double a, double b, std::function<double(double)> function);
    void transposition();
    Result<T> calculate(std::vector<std::vector<T>> A, std::vector<std::vector<T>> B, double a, double b,const std::function<double(double)>& function);
    void setMatrix(std::vector<std::vector<T>>);
private:
    std::vector<CalcType> m_types;
    double m_integrationResult{};
    std::vector<std::vector<T>> m_transpositionMatrix;
};

