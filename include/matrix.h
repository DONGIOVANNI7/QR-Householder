#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <random>

class Matrix {
public:
    // Constructors
    Matrix(int rows, int cols, double init_val = 0.0);
    Matrix(const std::vector<std::vector<double>>& data);
    
    // Accessors
    double& operator()(int i, int j);
    const double& operator()(int i, int j) const;
    int rows() const noexcept { return m_rows; }
    int cols() const noexcept { return m_cols; }

    // Core operations
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix transpose() const;
    Matrix inverse() const;  // For square matrices only
    
    // Norm calculations
    double normInf() const noexcept;  // Infinity norm (max row sum)

    // Factory methods
    static Matrix identity(int n);
    static Matrix random(int rows, int cols, double min = -1.0, double max = 1.0);
    static Matrix loadFromFile(const std::string& path);

    // Utility
    void print(const std::string& label = "") const;

private:
    int m_rows, m_cols;
    std::vector<double> m_data;  // Row-major contiguous storage

    // Helper for inverse()
    void lu_decompose(std::vector<int>& perm, int& sign) const;
};