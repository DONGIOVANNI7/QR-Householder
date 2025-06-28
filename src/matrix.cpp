#include "matrix.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <random>
#include <stdexcept>
#include <functional>  

// Constructors
Matrix::Matrix(int rows, int cols, double init_val) 
    : m_rows(rows), m_cols(cols), m_data(rows * cols, init_val) 
{
    if (rows <= 0 || cols <= 0) 
        throw std::invalid_argument("Matrix dimensions must be positive");
}

Matrix::Matrix(const std::vector<std::vector<double>>& data) {
    if (data.empty() || data[0].empty())
        throw std::invalid_argument("Invalid 2D vector");
    
    m_rows = data.size();
    m_cols = data[0].size();
    m_data.resize(m_rows * m_cols);
    
    for (int i = 0; i < m_rows; ++i) {
        if (static_cast<int>(data[i].size()) != m_cols)
            throw std::invalid_argument("Inconsistent row size");
        std::copy(data[i].begin(), data[i].end(), m_data.begin() + i * m_cols);
    }
}

// Accessors
double& Matrix::operator()(int i, int j) {
    if (i < 0 || i >= m_rows || j < 0 || j >= m_cols)
        throw std::out_of_range("Matrix index out of bounds");
    return m_data[i * m_cols + j];
}

const double& Matrix::operator()(int i, int j) const {
    if (i < 0 || i >= m_rows || j < 0 || j >= m_cols)
        throw std::out_of_range("Matrix index out of bounds");
    return m_data[i * m_cols + j];
}

// Matrix subtraction
Matrix Matrix::operator-(const Matrix& other) const {
    if (m_rows != other.m_rows || m_cols != other.m_cols)
        throw std::invalid_argument("Matrix dimensions mismatch");
    
    Matrix result(m_rows, m_cols);
    for (int i = 0; i < m_rows * m_cols; ++i)
        result.m_data[i] = m_data[i] - other.m_data[i];
    return result;
}

// Matrix multiplication
Matrix Matrix::operator*(const Matrix& other) const {
    if (m_cols != other.m_rows)
        throw std::invalid_argument("Matrix dimensions mismatch");
    
    Matrix result(m_rows, other.m_cols, 0.0);
    for (int i = 0; i < m_rows; ++i) {
        for (int k = 0; k < m_cols; ++k) {
            const double tmp = m_data[i * m_cols + k];
            for (int j = 0; j < other.m_cols; ++j) {
                result.m_data[i * other.m_cols + j] += 
                    tmp * other.m_data[k * other.m_cols + j];
            }
        }
    }
    return result;
}

// Transpose
Matrix Matrix::transpose() const {
    Matrix result(m_cols, m_rows);
    for (int i = 0; i < m_rows; ++i)
        for (int j = 0; j < m_cols; ++j)
            result(j, i) = (*this)(i, j);
    return result;
}

// Infinity norm (max row sum)
double Matrix::normInf() const noexcept {
    double max_sum = 0.0;
    for (int i = 0; i < m_rows; ++i) {
        double row_sum = 0.0;
        for (int j = 0; j < m_cols; ++j)
            row_sum += std::fabs((*this)(i, j));
        if (row_sum > max_sum) max_sum = row_sum;
    }
    return max_sum;
}

// Identity matrix
Matrix Matrix::identity(int n) {
    Matrix I(n, n, 0.0);
    for (int i = 0; i < n; ++i)
        I(i, i) = 1.0;
    return I;
}

// Random matrix generator
Matrix Matrix::random(int rows, int cols, double min, double max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min, max);
    
    Matrix mat(rows, cols);
    for (int i = 0; i < rows * cols; ++i)
        mat.m_data[i] = dist(gen);
    return mat;
}

// File I/O - Load matrix from text file
Matrix Matrix::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Cannot open file: " + path);
    
    std::vector<std::vector<double>> data;
    std::string line;
    
    while (std::getline(file, line)) {
        std::vector<double> row;
        std::istringstream iss(line);
        double value;
        while (iss >> value)
            row.push_back(value);
        if (!row.empty())
            data.push_back(row);
    }
    
    return Matrix(data);
}

// LU Decomposition helper for inverse
void Matrix::lu_decompose(std::vector<int>& perm, int& sign) {
    if (m_rows != m_cols)
        throw std::logic_error("LU decomposition requires square matrix");
    
    const int n = m_rows;
    perm.resize(n);
    std::vector<double> row_scales(n);
    sign = 1;

    // Initialize permutation and scaling
    for (int i = 0; i < n; ++i) {
        perm[i] = i;
        double max_val = 0.0;
        for (int j = 0; j < n; ++j) {
            const double abs_val = std::fabs((*this)(i, j));
            if (abs_val > max_val) max_val = abs_val;
        }
        if (max_val == 0.0) throw std::runtime_error("Matrix is singular");
        row_scales[i] = 1.0 / max_val;
    }

    // Crout's algorithm
    for (int j = 0; j < n; ++j) {
        // Compute elements of U
        for (int i = 0; i < j; ++i) {
            double sum = (*this)(i, j);
            for (int k = 0; k < i; ++k)
                sum -= (*this)(i, k) * (*this)(k, j);
            (*this)(i, j) = sum;
        }
        
        // Find pivot
        int pivot_row = j;
        double max_val = 0.0;
        for (int i = j; i < n; ++i) {
            double sum = (*this)(i, j);
            for (int k = 0; k < j; ++k)
                sum -= (*this)(i, k) * (*this)(k, j);
            (*this)(i, j) = sum;
            
            const double scaled_val = row_scales[i] * std::fabs(sum);
            if (scaled_val >= max_val) {
                max_val = scaled_val;
                pivot_row = i;
            }
        }
        
        // Swap rows if needed
        if (j != pivot_row) {
            for (int k = 0; k < n; ++k) {
                using std::swap;
                swap((*this)(pivot_row, k), (*this)(j, k));
            }
            sign = -sign;
            row_scales[pivot_row] = row_scales[j];
        }
        perm[j] = pivot_row;
        
        // Check singularity
        if (std::fabs((*this)(j, j)) < 1e-12)
            throw std::runtime_error("Matrix is singular");
            
        // Compute elements of L
        if (j != n-1) {
            const double denom = 1.0 / (*this)(j, j);
            for (int i = j+1; i < n; ++i)
                (*this)(i, j) *= denom;
        }
    }
}

// Matrix inverse using LU decomposition
Matrix Matrix::inverse() const {
    if (m_rows != m_cols)
        throw std::logic_error("Inverse requires square matrix");
    
    const int n = m_rows;
    Matrix LU = *this;  // Copy for LU decomposition
    std::vector<int> perm;
    int sign;
    LU.lu_decompose(perm, sign);
    
    Matrix inv(n, n, 0.0);
    
    // Solve LUx = e_k for each column k
    for (int k = 0; k < n; ++k) {
        // Forward substitution (Ly = e_k)
        std::vector<double> col(n, 0.0);
        col[k] = 1.0;
        
        for (int i = 0; i < n; ++i) {
            const int pi = perm[i];
            double sum = col[pi];
            col[pi] = col[i];
            for (int j = 0; j < i; ++j)
                sum -= LU(i, j) * col[j];
            col[i] = sum;
        }
        
        // Backward substitution (Ux = y)
        for (int i = n-1; i >= 0; --i) {
            double sum = col[i];
            for (int j = i+1; j < n; ++j)
                sum -= LU(i, j) * col[j];
            col[i] = sum / LU(i, i);
        }
        
        // Set column in inverse matrix
        for (int i = 0; i < n; ++i)
            inv(i, k) = col[i];
    }
    return inv;
}

// Matrix printing
void Matrix::print(const std::string& label) const {
    if (!label.empty()) std::cout << label << ":\n";
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j)
            std::cout << std::setw(12) << (*this)(i, j) << " ";
        std::cout << "\n";
    }
}