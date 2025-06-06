#pragma once
#include "matrix.h"

class ErrorMetrics {
public:
    // ||A - QR||∞
    static double a_minus_qr(const Matrix& A, const Matrix& Q, const Matrix& R);
    
    // ||QᵀQ - I||∞
    static double qtq_minus_i(const Matrix& Q);
    
    // ||AR⁻¹ - Q||∞
    static double arinv_minus_q(const Matrix& A, const Matrix& Q, const Matrix& R);
    
    // cond(R) = ||R||∞ * ||R⁻¹||∞
    static double condition_number(const Matrix& R);
    
private:
    // Helper for triangular matrix inversion
    static Matrix invert_upper_triangular(const Matrix& R);
};