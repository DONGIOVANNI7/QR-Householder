#include "matrix.h"
#include "qr_householder.h"
#include "error_metrics.h"
#include "benchmark.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    // Command-line handling for benchmark
    if (argc > 1 && std::string(argv[1]) == "bench") {
        Benchmark::run();
        return 0;
    }
    
    // Existing menu code
    std::cout << "QR Householder Factorization\n"
              << "============================\n\n";
    
    int choice;
    std::cout << "1. Manual matrix input\n"
              << "2. Generate random matrix\n"
              << "3. Load matrix from file\n"
              << "4. Run benchmark (n=100,500,1000)\n"
              << "5. Exit\n"
              << "Choice: ";
    std::cin >> choice;
    
    Matrix A(0, 0);
    try {
        switch(choice) {
            case 1: {
                int m, n;
                std::cout << "Enter matrix dimensions (rows cols): ";
                std::cin >> m >> n;
                A = Matrix(m, n);
                std::cout << "Enter matrix elements row-wise:\n";
                for (int i = 0; i < m; ++i) {
                    for (int j = 0; j < n; ++j) {
                        std::cin >> A(i, j);
                    }
                }
                break;
            }
            case 2: {
                int m, n;
                std::cout << "Enter matrix dimensions (rows cols): ";
                std::cin >> m >> n;
                A = Matrix::random(m, n);
                std::cout << "Generated random matrix (" << m << "x" << n << ")\n";
                break;
            }
            case 3: {
                std::string path;
                std::cout << "Enter file path: ";
                std::cin >> path;
                A = Matrix::loadFromFile(path);
                std::cout << "Loaded matrix (" << A.rows() << "x" << A.cols() << ")\n";
                break;
            }
            case 4:
                Benchmark::run();
                return 0;
            case 5:
                return 0;
            default:
                std::cout << "Invalid choice\n";
                return 1;
        }
        
        // Perform QR decomposition
        QRResult result = HouseholderQR::decompose(A);
        
        // Compute and display metrics
        std::cout << "\nResults:\n";
        std::cout << "||A - QR||∞: " << ErrorMetrics::a_minus_qr(A, result.Q, result.R) << "\n";
        std::cout << "||QᵀQ - I||∞: " << ErrorMetrics::qtq_minus_i(result.Q) << "\n";
        std::cout << "||AR⁻¹ - Q||∞: " << ErrorMetrics::arinv_minus_q(A, result.Q, result.R) << "\n";
        std::cout << "cond(R): " << ErrorMetrics::condition_number(result.R) << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
