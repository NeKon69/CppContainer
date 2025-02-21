#include <iostream>
#include <cassert>
#include <crtdbg.h>
#include <chrono>
#include "RawBase.h"

template<typename Func>
void measure_time(const char* name, Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << name << ": "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms\n";
}

void test_push_back() {
    RawVector<int> rv;
    measure_time("RawVector push_back", [&]() {
        rv.reserve(134217728);
        for (int i = 0; i < 100000000; ++i) {
            rv.push_back(i);
        }
        });
    assert(rv.get_size() == 100000000);
    std::cout << "Push_back test passed.\n";
}

void test_resize() {
    RawVector<int> rv;
    measure_time("RawVector resize", [&]() {
        rv.resize(1000000);
        });
    assert(rv.get_size() == 1000000);
    std::cout << "Resize test passed.\n";
}

void test_reserve() {
    RawVector<int> rv;
    measure_time("RawVector reserve", [&]() {
        rv.reserve(1000000);
        });
    assert(rv.get_capacity() >= 1000000);
    std::cout << "Reserve test passed.\n";
}

void test_access() {
    RawVector<int> rv;
    rv.resize(100);
    for (int i = 0; i < 100; ++i) {
        rv[i] = i;
    }
    measure_time("RawVector access", [&]() {
        for (int i = 0; i < 100; ++i) {
            assert(rv[i] == i);
        }
        });
    std::cout << "Access test passed.\n";
}

void test_iterator() {
    RawVector<int> rv;
    rv.resize(100);
    for (int i = 0; i < 100; ++i) {
        rv[i] = i;
    }
    measure_time("RawVector iterator", [&]() {
        int sum = 0;
        for (auto it = rv.begin(); it != rv.end(); ++it) {
            sum += *it;
        }
        assert(sum == 4950);
        });
    std::cout << "Iterator test passed.\n";
}

void test_copy() {
    RawVector<int> rv;
    rv.resize(100);
    for (int i = 0; i < 100; ++i) {
        rv[i] = i;
    }
    measure_time("RawVector copy", [&]() {
        RawVector<int> rv_copy = rv.copy();
        for (int i = 0; i < 100; ++i) {
            assert(rv_copy[i] == i);
        }
        });
    std::cout << "Copy test passed.\n";
}

void test_clear() {
    RawVector<int> rv;
    rv.resize(100);
    measure_time("RawVector clear", [&]() {
        rv.clear();
        });
    assert(rv.get_size() == 0);
    std::cout << "Clear test passed.\n";
}

void test_erase() {
    RawVector<int> rv;
    rv.resize(100);
    for (int i = 0; i < 100; ++i) {
        rv[i] = i;
    }
    measure_time("RawVector erase", [&]() {
        rv.erase(50);
        });
    assert(rv.get_size() == 99);
    std::cout << "Erase test passed.\n";
}

void test_insert() {
    RawVector<int> rv;
    rv.resize(100);
    for (int i = 0; i < 100; ++i) {
        rv[i] = i;
    }
    measure_time("RawVector insert", [&]() {
        rv.insert(50, 999);
        });
    assert(rv[50] == 999);
    assert(rv.get_size() == 101);
    std::cout << "Insert test passed.\n";
}

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    test_push_back();
    test_resize();
    test_reserve();
    test_access();
    test_iterator();
    test_copy();
    test_clear();
    test_erase();
    test_insert();
    RawVector<int> V(1000);
    auto start = std::chrono::high_resolution_clock::now();
    V.resize(10);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "resized to less test (should be near zero)" << ": "
        << (end - start).count()
        << " nanos\n";
    _CrtDumpMemoryLeaks();
    return 0;
}