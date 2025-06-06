#pragma once
#include "matrix.h"

struct QRResult {
    Matrix Q;
    Matrix R;
    
    // No default constructor needed
    QRResult(Matrix Q_mat, Matrix R_mat) : Q(std::move(Q_mat)), R(std::move(R_mat)) {}
};

class HouseholderQR {
public:
    static QRResult decompose(const Matrix& A);
    
private:
    static void apply_householder(
        Matrix& R, 
        Matrix& Q, 
        const std::vector<double>& v, 
        double beta, 
        int k
    );
};