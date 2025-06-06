#include "benchmark.h"
#include "qr_householder.h"
#include "error_metrics.h"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <functional>

// Measure CPU time for a function
double Benchmark::measure_cpu_time(std::function<void()> func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

// Test a specific matrix dimension
void Benchmark::test_dimension(int n) {
    Matrix A = Matrix::random(n, n);
    
    // Initialize matrices with proper dimensions
    Matrix Q = Matrix::identity(n);  // Initialize as identity matrix
    Matrix R = A;                    // Initialize as copy of A
    
    // Measure QR decomposition time
    double time = measure_cpu_time([&]() {
        QRResult result = HouseholderQR::decompose(A);
        Q = result.Q;  // Assign new values
        R = result.R;
    });
    
    // Compute error metrics
    double err_a = ErrorMetrics::a_minus_qr(A, Q, R);
    double err_b = ErrorMetrics::qtq_minus_i(Q);
    double err_c = ErrorMetrics::arinv_minus_q(A, Q, R);
    double cond_r = ErrorMetrics::condition_number(R);
    
    // Print results in table format
    std::cout << "| " << std::setw(4) << n << " | "
              << std::scientific << std::setprecision(2) << err_a << " | "
              << err_b << " | "
              << err_c << " | "
              << std::fixed << std::setprecision(2) << cond_r << " | "
              << std::setw(8) << std::fixed << std::setprecision(3) << time << " |\n";
}

// Main benchmark runner
void Benchmark::run() {
    std::vector<int> sizes = {100, 500, 1000};
    
    // Print table header
    std::cout << "\n| Dimension | α-Error (A-QR) | β-Error (QᵀQ-I) | γ-Error (AR⁻¹-Q) | cond(R) | Time (s) |\n";
    std::cout << "|----------|----------------|-----------------|------------------|---------|----------|\n";
    
    // Run tests for each size
    for (int n : sizes) {
        test_dimension(n);
    }
    
    std::cout << "\nBenchmark complete.\n\n";
    
}