#include "ComplexCalculator.h"

template <typename T>
ComplexCalculator<T>::ComplexCalculator(std::vector<CalcType> types) {
    this->m_types = types;
    std::cout << "Calculation object has been created!" << std::endl;
}

template <typename T>
void ComplexCalculator<T>::setMatrix(std::vector<std::vector<T>> transpositionMatrix) {
    this->m_transpositionMatrix = transpositionMatrix;
}

template <typename T>
std::vector<std::vector<T>> ComplexCalculator<T>::gaussCalc(std::vector<std::vector<T>> A, std::vector<std::vector<T>> B) {
    std::cout << "Gauss method executed" << std::endl;
    int n = A.size();
    int m = B[0].size();
    std::vector<std::vector<T>> X(n, std::vector<T>(m));

    for (int i = 0; i < n; ++i) {
        int maxRow = i;
        for (int k = i + 1; k < n; ++k) {
            if (std::abs(A[k][i]) > std::abs(A[maxRow][i])) {
                maxRow = k;
            }
        }
        if (maxRow != i) {
            std::swap(A[i], A[maxRow]);
            std::swap(B[i], B[maxRow]);
        }
        for (int k = i + 1; k < n; ++k) {
            T factor = A[k][i] / A[i][i];
            for (int j = i; j < n; ++j) {
                A[k][j] -= factor * A[i][j];
            }
            for (int j = 0; j < m; ++j) {
                B[k][j] -= factor * B[i][j];
            }
        }
    }

    for (int i = n - 1; i >=0 ; --i) {
        for (int j = 0; j < m; ++j) {
            T sum = 0;
            for (int k = i + 1; k < n; ++k) {
                sum += A[i][k] * X[k][j];
            }

            X[i][j] = (B[i][j] - sum) / A[i][j];
        }
    }

    return X;
}

template <typename T>
void ComplexCalculator<T>::transposition() {
    std::cout << "Transposition method executed" << std::endl;
    int rows = this->m_transpositionMatrix.size();
    int cols = this->m_transpositionMatrix[0].size();

    for (int i = 0; i < rows; ++i) {
        for (int j = i; j < cols; ++j) {
            std::swap(this->m_transpositionMatrix[i][j], this->m_transpositionMatrix[j][i]);
        }
    }
}

template <typename T>
T ComplexCalculator<T>::integration(double a, double b, std::function<double(double)> function) {
    std::cout << "Trapezoid method executed" << std::endl;
    int n = 1000;
    double h = (b - a) / n;

    double result = 0.5 * (function(a) + function(b));

    for (int i = 0; i < n; ++i) {
        double x = a + i * h;
        result += function(x);
    }

    result *= h;
    this->m_integrationResult = result;
    return result;
}

template <typename T>
Result<T> ComplexCalculator<T>::calculate(std::vector<std::vector<T>> A, std::vector<std::vector<T>> B, double a, double b, const std::function<double(double)>& function) {
    Result<T> results;
    for (auto type : this->m_types) {
        if (type == CalcType::GAUSS) {
            auto resultGauss = this->gaussCalc(A, B);
            results.gaussResult = resultGauss;
        } else if (type == CalcType::INTEGRATION) {
            double resultIntegration = this->integration(a, b, function);
            results.integrationResult = resultIntegration;
        } else if (type == CalcType::TRANSPOSITION){
            this->transposition();
            results.transpositionResult = this->m_transpositionMatrix;
        }
    }
    return results;
}

template class ComplexCalculator<double>;