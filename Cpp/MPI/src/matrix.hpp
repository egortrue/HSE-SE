#pragma once

#include "common.hpp"

class Matrix {
   public:
    int size;
    int rows;
    int cols;
    double* data;
    double* data_transposed;

    enum type_t {
        ZERO,
        RANDOM,
    };

    Matrix() {
        size = 0;
        rows = 0;
        cols = 0;
        data = nullptr;
        data_transposed = nullptr;
    }

    Matrix(const int rows, const int cols, const type_t type = ZERO) {
        this->size = rows * cols;
        this->rows = rows;
        this->cols = cols;
        this->data = new double[size];
        this->data_transposed = new double[size];

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                double value = type == ZERO ? 0 : MIN_VALUE + (rand() % static_cast<int>(MAX_VALUE - MIN_VALUE + 1));
                (*this)(row, col) = value;
                (*this)(row, col, true) = value;
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
        this->data = new double[size];
        this->data_transposed = new double[size];

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                double value = other(row, col);
                (*this)(row, col) = value;
                (*this)(row, col, true) = value;
            }
        }
    }

    void writeTransposedToData() {
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < cols; ++col)
                (*this)(row, col) = (*this)(row, col, true);
    }

    void writeDataToTransposed() {
        for (int row = 0; row < rows; ++row)
            for (int col = 0; col < cols; ++col)
                (*this)(row, col, true) = (*this)(row, col);
    }

    // Копирование элементов из исходной матрицы в подматрицу
    Matrix slice(int start_row, int start_col, int count_row, int count_col) {
        Matrix submatrix = Matrix(count_row, count_col);
        for (int i = start_row; i < start_row + count_row; i++) {
            for (int j = start_col; j < start_col + count_col; j++) {
                submatrix(i - start_row, j - start_col) = (*this)(i, j);
                submatrix(i - start_row, j - start_col, true) = (*this)(i, j, true);
            }
        }
        return submatrix;
    }

    double operator()(int row, int col, bool transposed = false) const {
        return transposed ? data_transposed[row + col * rows] : data[col + row * cols];
    }

    double& operator()(int row, int col, bool transposed = false) {
        return transposed ? data_transposed[row + col * rows] : data[col + row * cols];
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            clearData();

            this->size = other.size;
            this->rows = other.rows;
            this->cols = other.cols;
            this->data = new double[size];
            this->data_transposed = new double[size];

            for (int row = 0; row < rows; ++row) {
                for (int col = 0; col < cols; ++col) {
                    double value = other(row, col);
                    (*this)(row, col) = value;
                    (*this)(row, col, true) = value;
                }
            }
        }
        return *this;
    }

    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
        }
        Matrix result(rows, other.cols);
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < other.cols; ++col) {
                for (int k = 0; k < cols; k++) {
                    double value = (*this)(row, k) * other(k, col);
                    result(row, col) += value;
                    result(row, col, true) += value;
                }
            }
        }
        return result;
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
        if (data != nullptr) delete[] data;
        if (data_transposed != nullptr) delete[] data_transposed;
    }
};
