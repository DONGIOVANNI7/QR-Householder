#pragma once
#include <functional>  
#include <vector>

class Benchmark {
public:
    static void run();
    
private:
    static void test_dimension(int n);
    static double measure_cpu_time(std::function<void()> func); 
};