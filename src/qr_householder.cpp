#include "qr_householder.h"
#include <cmath>
#include <vector>
#include <iostream>

// Helper: Infinity norm of vector
static double vector_norm_inf(const std::vector<double>& v) {
    double max = 0.0;
    for (double x : v) {
        double abs_val = std::fabs(x);
        if (abs_val > max) max = abs_val;
    }
    return max;
}

QRResult HouseholderQR::decompose(const Matrix& A) {
    const int m = A.rows();
    const int n = A.cols();
    const int t = std::min(m, n);
    
    Matrix Q = Matrix::identity(m);
    Matrix R = A;

    for (int k = 0; k < t; ++k) {
        // Extract column k from diagonal downward
        std::vector<double> x;
        for (int i = k; i < m; ++i) {
            x.push_back(R(i, k));
        }

        // Skip if zero column
        if (vector_norm_inf(x) < 1e-12) continue;

        // Compute norm and sign
        double norm_x = 0.0;
        for (double val : x) norm_x += val * val;
        norm_x = std::sqrt(norm_x);
        
        const double sign = (x[0] >= 0) ? 1.0 : -1.0;
        const double sigma = -sign * norm_x;
        
        // Construct Householder vector
        std::vector<double> v = x;
        v[0] = x[0] - sigma;  // v = x - sigma*e1
        
        // Compute beta = 2/(v^T v)
        double vtv = 0.0;
        for (double val : v) vtv += val * val;
        const double beta = 2.0 / vtv;

        // Apply transformation to R and Q
        apply_householder(R, Q, v, beta, k);
        
        // Store R's diagonal element
        R(k, k) = sigma;
        
        // Zero subdiagonal elements
        for (int i = k + 1; i < m; ++i) {
            R(i, k) = 0.0;
        }
    }
    
    // Use constructor instead of brace initialization
    return QRResult(Q, R);
}

void HouseholderQR::apply_householder(
    Matrix& R, 
    Matrix& Q, 
    const std::vector<double>& v, 
    double beta, 
    int k
) {
    const int m = R.rows();
    const int n = R.cols();
    const int v_size = v.size();
    
    // Apply to R: R = (I - beta*v*v^T) * R
    for (int j = k; j < n; ++j) {
        // Compute v^T * R_col
        double dot = 0.0;
        for (int i = 0; i < v_size; ++i) {
            dot += v[i] * R(k + i, j);
        }
        
        // Update column
        for (int i = 0; i < v_size; ++i) {
            R(k + i, j) -= beta * dot * v[i];
        }
    }
    
    // Apply to Q: Q = Q * (I - beta*v*v^T)
    for (int i = 0; i < m; ++i) {
        // Compute Q_row * v
        double dot = 0.0;
        for (int j = 0; j < v_size; ++j) {
            dot += Q(i, k + j) * v[j];
        }
        
        // Update row
        for (int j = 0; j < v_size; ++j) {
            Q(i, k + j) -= beta * dot * v[j];
        }
    }
}