#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include <string>

int main() {
    const int sizes[] = {100, 500, 1000};
    const double min_val = -10.0;
    const double max_val = 10.0;
    const std::string data_dir = "data";  // Your existing data folder
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(min_val, max_val);
    
    for (int n : sizes) {
        std::string filename = data_dir + "/matrix_" + std::to_string(n) + "x" + std::to_string(n) + ".txt";
        std::ofstream file(filename);
        
        if (!file) {
            std::cerr << "Error: Could not create file " << filename << "\n";
            std::cerr << "Make sure the 'data' directory exists in the current path.\n";
            continue;
        }
        
        file << std::fixed << std::setprecision(6);
        
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                file << dist(gen);
                if (j < n-1) file << " ";
            }
            file << "\n";
        }
        
        std::cout << "Generated " << filename << " (" << n << "x" << n << ")\n";
    }
    
    std::cout << "\nMatrix generation complete. Files saved to /data folder.\n";
    return 0;
}