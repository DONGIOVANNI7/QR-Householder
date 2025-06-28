# QR-Householder

**Numerical Calculation of Eigenvalues and Eigenvectors using QR Factorization with Householder Transformations**

## Overview
This project implements the QR factorization algorithm with Householder transformations for numerical computation of eigenvalues and eigenvectors. The Householder method provides exceptional numerical stability and accuracy for matrix factorization. The implementation includes a complete benchmarking system and user-friendly interface.

## Project Structure

### Key Files and Directories
```
QR-Householder-CPP/
├── data/ # Sample matrices (100x100, 500x500, 1000x1000)
├── include/ # Header files
│ ├── matrix.h
│ ├── qr_householder.h
│ ├── error_metrics.h
│ └── benchmark.h
├── src/ # Implementation files
│ ├── matrix.cpp
│ ├── qr_householder.cpp
│ ├── error_metrics.cpp
│ ├── benchmark.cpp
│ └── main.cpp
├── generate_matrices.cpp # Matrix generator utility
├── Makefile # Build configuration
└── README.md # This documentation
```

### `include/` Directory (Header Files)
1. **`matrix.h`**  
   - Matrix class definition
   - Operations: creation, arithmetic, norms, file I/O
   
2. **`qr_householder.h`**  
   - QR factorization algorithm declaration
   - QRResult struct for storing results
   
3. **`error_metrics.h`**  
   - Error computation functions:
     - Factorization residual
     - Orthogonality check
     - Inverse stability
     - Condition number
   
4. **`benchmark.h`**  
   - Benchmark system for performance testing

### `src/` Directory (Implementations)
1. **`matrix.cpp`**  
   - Complete matrix operations implementation
   - LU decomposition for inverse
   - File I/O handling
   
2. **`qr_householder.cpp`**  
   - Core QR factorization algorithm
   - Householder reflection implementation
   
3. **`error_metrics.cpp`**  
   - Error metric calculations
   - Specialized triangular matrix inversion
   
4. **`benchmark.cpp`**  
   - Performance testing system
   - Generates console and LaTeX output
   
5. **`main.cpp`**  
   - User interface with input validation:
     - Manual matrix input with element validation
     - Random matrix generation with size limits
     - File loading with existence check
     - Direct benchmark execution

### Additional Utility
**`generate_matrices.cpp`**  
- Generates random matrices for benchmarking
- Creates 100×100, 500×500, and 1000×1000 matrices
- Saves matrices to `/data` folder
- Values uniformly distributed in [-10.0, 10.0]

## Algorithmic Complexity
- **Time Complexity**: O(n³) for n × n matrix
- **Space Complexity**: O(n²)
- **Numerical Stability**: 
  - Orthogonality error ∼10⁻¹³
  - Residual error ∼10⁻¹²
  - Stable even for ill-conditioned matrices

## Program Features
1. **Manual Data Input**: 
   - Guided element-by-element entry
   - Input validation for numbers
   
2. **Matrix Generation**:
   - Random matrices with configurable size
   - Size limits (max 10000×10000)
   
3. **File Input**:
   - Load matrices from text files
   - Automatic file existence check
   
4. **Benchmark System**:
   - Tests 100×100, 500×500, 1000×1000 matrices
   - Measures execution time and accuracy
   - Generates LaTeX-ready results

## Building and Running

### Compilation
```bash
# Build main program
make

# Build matrix generator utility
g++ -std=c++11 generate_matrices.cpp -o generate_matrices

=Execution

# Run with menu interface
./main

# Run benchmarks directly
./main bench

# Generate sample matrices
./generate_matrices

- Makefile Targets

make        # Build main program
make run    # Run benchmarks
make clean  # Remove executables and object files
```


### Benchmark Results

The following results were obtained using random matrices:

| Dimension | α-Error (A-QR) | β-Error (QᵀQ-I) | γ-Error (AR⁻¹-Q) | cond(R) | Time (s) |
|----------|----------------|-----------------|------------------|---------|----------|
|  100     | 1.32e-13       | 1.86e-14        | 9.38e-14         | 1046.01 |    0.007 |
|  500     | 1.73e-12       | 1.02e-13        | 2.13e-12         | 31511.89 |    0.725 |
| 1000     | 4.86e-12       | 1.97e-13        | 2.20e-11         | 278217.78 |    6.174 |


For matrices larger than 1000×1000 (n > 1000), the following optimizations could be implemented:

Blocked Algorithms
Using cache-friendly block processing to improve memory efficiency

Parallel Computing
Utilizing OpenMP for loop-level parallelism to accelerate computations

SIMD Instructions
Employing AVX/SSE instructions for vectorized operations