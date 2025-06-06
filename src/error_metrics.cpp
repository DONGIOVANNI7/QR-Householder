#include "error_metrics.h"
#include <stdexcept>

// ||A - QR||∞ computation
double ErrorMetrics::a_minus_qr(const Matrix& A, const Matrix& Q, const Matrix& R) {
    if (A.rows() != Q.rows() || A.rows() != R.rows() || 
        A.cols() != R.cols() || Q.cols() != R.rows()) {
        throw std::invalid_argument("Matrix dimension mismatch in a_minus_qr");
    }
    
    Matrix QR = Q * R;
    Matrix diff = A - QR;
    return diff.normInf();
}

// ||QᵀQ - I||∞ computation
double ErrorMetrics::qtq_minus_i(const Matrix& Q) {
    if (Q.rows() != Q.cols()) 
        throw std::invalid_argument("Q must be square in qtq_minus_i");
    
    Matrix QT = Q.transpose();
    Matrix QTQ = QT * Q;
    Matrix I = Matrix::identity(Q.rows());
    Matrix diff = QTQ - I;
    return diff.normInf();
}

// Invert upper triangular matrix (efficient back substitution)
Matrix ErrorMetrics::invert_upper_triangular(const Matrix& R) {
    if (R.rows() != R.cols()) 
        throw std::invalid_argument("R must be square for inversion");
    
    const int n = R.rows();
    Matrix inv(n, n, 0.0);
    
    // Backward substitution for each column
    for (int j = n-1; j >= 0; --j) {
        inv(j, j) = 1.0 / R(j, j);
        for (int i = j-1; i >= 0; --i) {
            double sum = 0.0;
            for (int k = i+1; k <= j; ++k) {
                sum += R(i, k) * inv(k, j);
            }
            inv(i, j) = -sum / R(i, i);
        }
    }
    return inv;
}

// ||AR⁻¹ - Q||∞ computation
double ErrorMetrics::arinv_minus_q(const Matrix& A, const Matrix& Q, const Matrix& R) {
    if (A.rows() != Q.rows() || A.cols() != R.rows() ||
        Q.rows() != Q.cols() || R.rows() != R.cols()) {
        throw std::invalid_argument("Matrix dimension mismatch in arinv_minus_q");
    }
    
    Matrix R_inv = invert_upper_triangular(R);
    Matrix AR_inv = A * R_inv;
    Matrix diff = AR_inv - Q;
    return diff.normInf();
}

// Condition number computation cond(R) = ||R||∞ * ||R⁻¹||∞
double ErrorMetrics::condition_number(const Matrix& R) {
    if (R.rows() != R.cols()) 
        throw std::invalid_argument("R must be square for condition number");
    
    Matrix R_inv = invert_upper_triangular(R);
    return R.normInf() * R_inv.normInf();
}