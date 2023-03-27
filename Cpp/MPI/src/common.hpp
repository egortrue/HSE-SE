#pragma once

// #define DEBUG
#define MIN_VALUE 1
#define MAX_VALUE 5

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include "matrix.hpp"
#include "mpi.h"
#include "mpi.hpp"

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    for (int i = 0; i < v.size(); ++i) {
        os << std::fixed << std::setw(4) << v[i];
        if (i != v.size() - 1)
            os << " ";
    }
    os << "]";
    return os;
}

double operator*(std::vector<double>& a, std::vector<double>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors dimensions do not match for multiplication.");
    }
    double result = 0;
    for (int index = 0; index < a.size(); ++index)
        result += a[index] * b[index];
    return result;
}