#pragma once

#include "common.hpp"

class Matrix {
   public:
    int size;
    int rows;
    int cols;
    double** data;
    double** data_transposed;

    enum type_t {
        ZERO,
        RANDOM,
    };

    Matrix() {
        size = 0;
        rows = 0;
        cols = 0;
        data = nullptr;
    }

    Matrix(const int rows, const int cols, const type_t type = ZERO) {
        this->size = rows * cols;
        this->rows = rows;
        this->cols = cols;

        this->data = new double*[rows];
        for (int row = 0; row < rows; ++row) {
            this->data[row] = new double[cols];
        }

        this->data_transposed = new double*[cols];
        for (int col = 0; col < cols; ++col) {
            this->data_transposed[col] = new double[rows];
        }

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                double value = type == ZERO ? 0 : MIN_VALUE + (rand() % static_cast<int>(MAX_VALUE - MIN_VALUE + 1));
                this->data[row][col] = value;
                this->data_transposed[col][row] = value;
            }
        }
    }

    ~Matrix() {
        clearData();
    }

    Matrix(const Matrix& other) {
        this->size = rows * cols;
        this->rows = other.rows;
        this->cols = other.cols;

        this->data = new double*[rows];
        for (int row = 0; row < rows; ++row) {
            this->data[row] = new double[cols];
        }

        this->data_transposed = new double*[cols];
        for (int col = 0; col < cols; ++col) {
            this->data_transposed[col] = new double[rows];
        }

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                double value = other(row, col);
                this->data[row][col] = value;
                this->data_transposed[col][row] = value;
            }
        }
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            this->size = other.size;
            this->rows = other.rows;
            this->cols = other.cols;

            clearData();
            this->data = new double*[rows];
            for (int row = 0; row < rows; ++row) {
                this->data[row] = new double[cols];
            }

            this->data_transposed = new double*[cols];
            for (int col = 0; col < cols; ++col) {
                this->data_transposed[col] = new double[rows];
            }

            for (int row = 0; row < rows; ++row) {
                for (int col = 0; col < cols; ++col) {
                    double value = other(row, col);
                    this->data[row][col] = value;
                    this->data_transposed[col][row] = value;
                }
            }
        }
        return *this;
    }

    double operator()(int row, int col, bool transposed = false) const {
        return transposed ? data_transposed[col][row] : data[row][col];
    }

    double& operator()(int row, int col, bool transposed = false) {
        return transposed ? data_transposed[col][row] : data[row][col];
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        os << "[" << std::endl;
        for (int i = 0; i < matrix.rows; ++i) {
            for (int j = 0; j < matrix.cols; ++j) {
                os << std::fixed << std::setw(5) << matrix(i, j) << " ";
            }
            os << std::endl;
        }
        os << "]" << std::endl;
        return os;
    }

   private:
    void clearData() {
        if (data != nullptr) {
            for (int row = 0; row < rows; ++row)
                if (data[row] != nullptr) delete[] data[row];
            delete[] data;
        }
        if (data_transposed != nullptr) {
            for (int col = 0; col < cols; ++col)
                if (data_transposed[col] != nullptr) delete[] data_transposed[col];
            delete[] data_transposed;
        }
    }
};
