#include <iostream>
#include <cassert>
#include <crtdbg.h>
#include <chrono>
#include "RawBase.h"
#include <iomanip>
#include <random>

// Предполагаемый интерфейс RawVector
template <typename T>
class RawVector {
public:
    void push_back(const T& value);
    T& operator[](size_t index);
    void resize(size_t new_size);
    size_t get_size() const;
    size_t get_capacity() const;
    // Другие методы...
};

struct TestResult {
    double vector_time;
    double raw_vector_time;
};

// Функция для замера времени push_back
TestResult test_push_back(size_t num_elements, int runs = 10) {
    double vec_total = 0.0, raw_vec_total = 0.0;

    for (int r = 0; r < runs; ++r) {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<int> vec;
        for (size_t i = 0; i < num_elements; ++i) vec.push_back(i);
        auto end = std::chrono::high_resolution_clock::now();
        vec_total += std::chrono::duration<double, std::milli>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        RawVector<int> raw_vec;
        for (size_t i = 0; i < num_elements; ++i) raw_vec.push_back(i);
        end = std::chrono::high_resolution_clock::now();
        raw_vec_total += std::chrono::duration<double, std::milli>(end - start).count();
    }
    return { vec_total / runs, raw_vec_total / runs };
}

// Функция для замера времени operator[]
TestResult test_access(size_t num_elements, int runs = 10) {
    std::vector<int> vec(num_elements);
    RawVector<int> raw_vec; raw_vec.resize(num_elements);
    std::random_device rd; std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, num_elements - 1);

    double vec_total = 0.0, raw_vec_total = 0.0;
    for (int r = 0; r < runs; ++r) {
        auto start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < num_elements; ++i) volatile int x = vec[dis(gen)];
        auto end = std::chrono::high_resolution_clock::now();
        vec_total += std::chrono::duration<double, std::milli>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        for (size_t i = 0; i < num_elements; ++i) volatile int x = raw_vec[dis(gen)];
        end = std::chrono::high_resolution_clock::now();
        raw_vec_total += std::chrono::duration<double, std::milli>(end - start).count();
    }
    return { vec_total / runs, raw_vec_total / runs };
}

// Аналогичные функции для resize, size/get_size, capacity/get_capacity...

int main() {
    std::cout << std::fixed << std::setprecision(3);
    const size_t sizes[] = { 100, 10000, 1000000 };
    const char* size_names[] = { "Small (100)", "Medium (10,000)", "Large (1,000,000)" };

    std::cout << "Operation         | Elements      | vector (ms) | RawVector (ms)\n";
    std::cout << "------------------|---------------|-------------|---------------\n";

    for (int i = 0; i < 3; ++i) {
        auto pb_result = test_push_back(sizes[i]);
        std::cout << "push_back         | " << std::setw(13) << size_names[i] << " | "
            << std::setw(11) << pb_result.vector_time << " | "
            << std::setw(11) << pb_result.raw_vector_time << "\n";

        auto acc_result = test_access(sizes[i]);
        std::cout << "operator[]        | " << std::setw(13) << size_names[i] << " | "
            << std::setw(11) << acc_result.vector_time << " | "
            << std::setw(11) << acc_result.raw_vector_time << "\n";
    }

    return 0;
}