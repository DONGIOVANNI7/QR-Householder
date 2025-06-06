#pragma once
#include <vector>
#include <utility>
#include <string>

class Benchmark {
public:
    static void run();
    
private:
    static void test_dimension(int n);
    static double measure_cpu_time(std::function<void()> func);
};