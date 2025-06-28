#include "matrix.h"
#include "qr_householder.h"
#include "error_metrics.h"
#include "benchmark.h"
#include <iostream>
#include <string>
#include <fstream>    // For file existence check
#include <limits>     // For input validation

int main(int argc, char* argv[]) {
    // Command-line benchmark handling
    if (argc > 1 && std::string(argv[1]) == "bench") {
        Benchmark::run();
        return 0;
    }
    
    std::cout << "QR Householder Factorization\n"
              << "============================\n\n";
    
    int choice;
    std::cout << "1. Manual matrix input\n"
              << "2. Generate random matrix\n"
              << "3. Load matrix from file\n"
              << "4. Run benchmarks (n=100,500,1000)\n"
              << "5. Exit\n"
              << "Choice: ";
    std::cin >> choice;
    
    // Validate menu choice
    if (std::cin.fail() || choice < 1 || choice > 5) {
        std::cerr << "Invalid choice. Exiting.\n";
        return 1;
    }
    
    Matrix A(1, 1);  // Temporary 1x1 matrix
    
    try {
        switch(choice) {
            case 1: {  // Manual input
                int m, n;
                std::cout << "Enter matrix dimensions (rows cols): ";
                
                // Validate dimensions input
                while (!(std::cin >> m >> n) || m <= 0 || n <= 0) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "Error: Dimensions must be positive integers. Try again: ";
                }
                
                // Dimension limit check
                if (m > 10000 || n > 10000) {
                    throw std::invalid_argument("Dimensions too large (max 10000)");
                }
                
                A = Matrix(m, n);
                std::cout << "Enter matrix elements row-wise:\n";
                for (int i = 0; i < m; ++i) {
                    for (int j = 0; j < n; ++j) {
                        std::cout << "Element [" << i << "][" << j << "]: ";
                        while (!(std::cin >> A(i, j))) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cerr << "Invalid input. Enter a number: ";
                        }
                    }
                }
                break;
            }
            case 2: {  // Random matrix
                int m, n;
                std::cout << "Enter matrix dimensions (rows cols): ";
                
                // Validate dimensions input
                while (!(std::cin >> m >> n) || m <= 0 || n <= 0) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cerr << "Error: Dimensions must be positive integers. Try again: ";
                }
                
                // Dimension limit check
                if (m > 10000 || n > 10000) {
                    throw std::invalid_argument("Dimensions too large (max 10000)");
                }
                
                A = Matrix::random(m, n);
                std::cout << "Generated random matrix (" << m << "x" << n << ")\n";
                break;
            }
            case 3: {  // Load from file
                std::string path;
                std::cout << "Enter file path: ";
                std::cin >> path;
                
                // File existence check
                std::ifstream test(path);
                if (!test) {
                    throw std::runtime_error("File not found: " + path);
                }
                test.close();
                
                A = Matrix::loadFromFile(path);
                std::cout << "Loaded matrix (" << A.rows() << "x" << A.cols() << ")\n";
                
                // Dimension limit check
                if (A.rows() > 10000 || A.cols() > 10000) {
                    throw std::invalid_argument("Matrix dimensions too large (max 10000)");
                }
                break;
            }
            case 4:  // Run benchmarks
                Benchmark::run();
                return 0;
            case 5:  // Exit
                return 0;
        }
        
        // Perform QR decomposition
        QRResult result = HouseholderQR::decompose(A);
        Matrix& Q = result.Q;
        Matrix& R = result.R;
        
        // Compute and display metrics
        std::cout << "\nResults:\n";
        std::cout << "||A - QR||∞: " << ErrorMetrics::a_minus_qr(A, Q, R) << "\n";
        std::cout << "||QᵀQ - I||∞: " << ErrorMetrics::qtq_minus_i(Q) << "\n";
        std::cout << "||AR⁻¹ - Q||∞: " << ErrorMetrics::arinv_minus_q(A, Q, R) << "\n";
        std::cout << "cond(R): " << ErrorMetrics::condition_number(R) << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}