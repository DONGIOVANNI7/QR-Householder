#pragma once
#include "matrix.h"

struct QRResult {
    Matrix Q;
    Matrix R;
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