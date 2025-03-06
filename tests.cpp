#include "vector_alias.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <random>

#ifdef _MSC_VER  // Только для Visual Studio
#include <crtdbg.h>
#define DEBUG_BREAK() _CrtDbgBreak()
#endif


struct MemoryLeakDetector {
    MemoryLeakDetector() {
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    }
    ~MemoryLeakDetector() {
        std::cout << "=== Leaks Detection ===\n";
        if (_CrtDumpMemoryLeaks()) {
            std::cout << "Leak detection!\n";
        }
        else {
            std::cout << "No leaks!\n";
        }
    }
} memoryLeakDetector;

// Helper function to check vector contents
template <typename T>
bool compareVectors(const raw::vector<T>& v1, const std::vector<T>& v2) {
    if (v1.get_size() != v2.size()) {
        return false;
    }
    for (size_t i = 0; i < v1.get_size(); ++i) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }
    return true;
}

// Helper function to create a std::vector from a raw::vector
template <typename T>
std::vector<T> toStdVector(const raw::vector<T>& rv) {
    std::vector<T> sv;
    for (size_t i = 0; i < rv.get_size(); ++i) {
        sv.push_back(rv[i]);
    }
    return sv;
}

// Custom struct for testing
struct MyStruct {
    int x;
    std::string s;
    MyStruct(int x_ = 0, const std::string& s_ = "") : x(x_), s(s_) {}
    MyStruct(const MyStruct& other) : x(other.x), s(other.s) {
        std::cout << "MyStruct copy constructor\n";
    }
    MyStruct(MyStruct&& other) noexcept : x(other.x), s(std::move(other.s)) {
        std::cout << "MyStruct move constructor\n";
        other.x = 0;
        other.s = "";
    }
    MyStruct& operator=(const MyStruct& other) {
        std::cout << "MyStruct copy assignment\n";
        if (this != &other) {
            x = other.x;
            s = other.s;
        }
        return *this;
    }
    MyStruct& operator=(MyStruct&& other) noexcept {
        std::cout << "MyStruct move assignment\n";
        if (this != &other) {
            x = other.x;
            s = std::move(other.s);
            other.x = 0;
            other.s = "";
        }
        return *this;
    }
    bool operator==(const MyStruct& other) const {
        return x == other.x && s == other.s;
    }
    ~MyStruct() {
        std::cout << "MyStruct destructor\n";
    }
};

// Helper function to print the contents of a raw::vector
template <typename T>
void printVector(const raw::vector<T>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.get_size(); ++i) {
        std::cout << "\"" << v[i] << "\""; // Print strings with quotes
        if (i < v.get_size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}

template <typename T>
void printVector(const std::vector<T>& v) {
    std::cout << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << "\"" << v[i] << "\""; // Print strings with quotes
        if (i < v.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]\n";
}

void testDefaultConstructor() {
    std::cout << "Testing default constructor...\n";
    raw::vector<std::string> v;
    assert(v.get_size() == 0);
    assert(v.get_capacity() >= 0); // Capacity should be at least 1
    std::cout << "Default constructor test passed.\n";
}

void testSizeConstructor() {
    std::cout << "Testing size constructor...\n";
    size_t size = 10;
    raw::vector<std::string> v(size);
    assert(v.get_size() == size);
    assert(v.get_capacity() >= size);
    for (size_t i = 0; i < size; ++i) {
        assert(v[i] == ""); // Default constructed strings should be empty
    }
    std::cout << "Size constructor test passed.\n";
}

void testCopyConstructor() {
    std::cout << "Testing copy constructor...\n";
    raw::vector<std::string> v1(5);
    for (int i = 0; i < 5; ++i) {
        v1[i] = "string" + std::to_string(i);
    }
    raw::vector<std::string> v2 = v1;
    assert(v2.get_size() == v1.get_size());
    assert(v2.get_capacity() == v1.get_capacity());
    for (size_t i = 0; i < v1.get_size(); ++i) {
        assert(v2[i] == v1[i]);
    }
    std::cout << "Copy constructor test passed.\n";
}

void testMoveConstructor() {
    std::cout << "Testing move constructor...\n";
    raw::vector<std::string> v1(5);
    for (int i = 0; i < 5; ++i) {
        v1[i] = "string" + std::to_string(i);
    }
    size_t old_capacity = v1.get_capacity();
    raw::vector<std::string> v2 = std::move(v1);
    assert(v2.get_size() == 5);
    assert(v2.get_capacity() == old_capacity);
    for (int i = 0; i < 5; ++i) {
        assert(v2[i] == "string" + std::to_string(i));
    }
    assert(v1.get_size() == 0);
    assert(v1.get_capacity() == 0);
    std::cout << "Move constructor test passed.\n";
}

void testCopyAssignment() {
    std::cout << "Testing copy assignment...\n";
    raw::vector<std::string> v1(5);
    for (int i = 0; i < 5; ++i) {
        v1[i] = "string" + std::to_string(i);
    }
    raw::vector<std::string> v2;
    v2 = v1;
    assert(v2.get_size() == v1.get_size());
    assert(v2.get_capacity() == v1.get_capacity());
    for (size_t i = 0; i < v1.get_size(); ++i) {
        assert(v2[i] == v1[i]);
    }
    std::cout << "Copy assignment test passed.\n";
}

void testMoveAssignment() {
    std::cout << "Testing move assignment...\n";
    raw::vector<std::string> v1(5);
    for (int i = 0; i < 5; ++i) {
        v1[i] = "string" + std::to_string(i);
    }
    size_t old_capacity = v1.get_capacity();
    raw::vector<std::string> v2;
    v2 = std::move(v1);
    assert(v2.get_size() == 5);
    assert(v2.get_capacity() == old_capacity);
    for (int i = 0; i < 5; ++i) {
        assert(v2[i] == "string" + std::to_string(i));
    }
    assert(v1.get_size() == 0);
    assert(v1.get_capacity() == 0);
    std::cout << "Move assignment test passed.\n";
}

void testPushBack() {
    std::cout << "Testing push_back...\n";
    raw::vector<std::string> v;
    for (int i = 0; i < 10; ++i) {
        v.push_back("string" + std::to_string(i));
        assert(v.get_size() == i + 1);
        assert(v.get_capacity() >= v.get_size());
        assert(v[i] == "string" + std::to_string(i));
    }
    std::cout << "push_back test passed.\n";
}

void testPushBackMove() {
    std::cout << "Testing push_back with move semantics...\n";
    raw::vector<MyStruct> v;
    MyStruct ms1(1, "one");
    v.push_back(std::move(ms1));
    assert(v.get_size() == 1);
    assert(v[0].x == 1);
    assert(v[0].s == "one");
    assert(ms1.x == 0);
    assert(ms1.s == "");
    MyStruct ms2(2, "two");
    v.push_back(std::move(ms2));
    assert(v.get_size() == 2);
    assert(v[1].x == 2);
    assert(v[1].s == "two");
    assert(ms2.x == 0);
    assert(ms2.s == "");
    std::cout << "push_back move test passed.\n";
}

void testAt() {
    std::cout << "Testing at()...\n";
    raw::vector<std::string> v(5);
    for (int i = 0; i < 5; ++i) {
        v[i] = "string" + std::to_string(i);
    }
    for (int i = 0; i < 5; ++i) {
        assert(v.at(i) == "string" + std::to_string(i));
    }
    try {
        v.at(5);
        assert(false); // Should have thrown an exception
    }
    catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    std::cout << "at() test passed.\n";
}

void testAtConst() {
    std::cout << "Testing const at()...\n";
    const raw::vector<std::string> v(5);
    for (int i = 0; i < 5; ++i) {
        ((raw::vector<std::string>&)v)[i] = "string" + std::to_string(i); // Workaround to modify const vector for testing
    }
    for (int i = 0; i < 5; ++i) {
        assert(v.at(i) == "string" + std::to_string(i));
    }
    try {
        v.at(5);
        assert(false); // Should have thrown an exception
    }
    catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    std::cout << "const at() test passed.\n";
}

void testOperatorBrackets() {
    std::cout << "Testing operator[]...\n";
    raw::vector<std::string> v(5);
    for (int i = 0; i < 5; ++i) {
        v[i] = "string" + std::to_string(i);
    }
    for (int i = 0; i < 5; ++i) {
        assert(v[i] == "string" + std::to_string(i));
    }
    std::cout << "operator[] test passed.\n";
}

void testResize() {
    std::cout << "Testing resize()...\n";
    raw::vector<std::string> v;
    v.resize(5);
    assert(v.get_size() == 5);
    assert(v.get_capacity() >= 5);
    for (int i = 0; i < 5; ++i) {
        assert(v[i] == "");
    }
    v.resize(2);
    assert(v.get_size() == 2);
    assert(v.get_capacity() >= 2);
    v.resize(8);
    assert(v.get_size() == 8);
    assert(v.get_capacity() >= 8);
    for (int i = 2; i < 8; ++i) {
        assert(v[i] == "");
    }
    std::cout << "resize() test passed.\n";
}

void testReserve() {
    std::cout << "Testing reserve()...\n";
    raw::vector<std::string> v;
    v.reserve(10);
    assert(v.get_capacity() >= 10);
    v.push_back("test");
    v.reserve(5); // Should not change capacity
    assert(v.get_capacity() >= 10);
    std::cout << "reserve() test passed.\n";
}

void testClear() {
    std::cout << "Testing clear()...\n";
    raw::vector<std::string> v(5);
    for (int i = 0; i < 5; ++i) {
        v[i] = "string" + std::to_string(i);
    }
    v.clear();
    assert(v.get_size() == 0);
    assert(v.get_capacity() == 1); // Capacity should be reset to 1
    std::cout << "clear() test passed.\n";
}

void testShrinkToFit() {
    std::cout << "Testing shrink_to_fit()...\n";
    raw::vector<std::string> v(10);
    for (int i = 0; i < 10; ++i) {
        v[i] = "string" + std::to_string(i);
    }
    v.resize(5);
    size_t old_capacity = v.get_capacity();
    v.shrink_to_fit();
	std::cout << v.get_capacity() << std::endl;
    assert(v.get_size() == 5);
    assert(v.get_capacity() == 5);
    std::cout << "shrink_to_fit() test passed.\n";
}

void testPopBack() {
    std::cout << "Testing pop_back()...\n";
    raw::vector<std::string> v;
    for (int i = 0; i < 5; ++i) {
        v.push_back("string" + std::to_string(i));
    }
    v.pop_back();
    assert(v.get_size() == 4);
    assert(v[3] == "string3");
    v.pop_back();
    v.pop_back();
    v.pop_back();
    v.pop_back();
    assert(v.get_size() == 0);
    try {
        v.pop_back();
        assert(false); // Should have thrown an exception
    }
    catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    std::cout << "pop_back() test passed.\n";
}

void testInsertIndex() {
    std::cout << "Testing insert(index)...\n";
    raw::vector<std::string> v;
    for (int i = 0; i < 5; ++i) {
        v.push_back("string" + std::to_string(i));
    }
    v.insert(2, "inserted");
    assert(v.get_size() == 6);
    assert(v[2] == "inserted");
    assert(v[3] == "string2");
    v.insert(0, "first");
    assert(v.get_size() == 7);
    assert(v[0] == "first");
    assert(v[1] == "string0");
    v.insert(v.get_size(), "last"); // Insert at the end
    std::cout << v.get_size() << "\n";
    assert(v.get_size() == 8);
    assert(v[v.get_size() - 1] == "last");

    try {
        v.insert(v.get_size() + 1, "out_of_range"); // out of range
        assert(false);
    }
    catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    std::cout << "insert(index) test passed.\n";
}

void testInsertIndexMove() {
    std::cout << "Testing insert(index) with move semantics...\n";
    raw::vector<MyStruct> v;
    for (int i = 0; i < 3; ++i) {
        v.push_back(MyStruct(i, "original" + std::to_string(i)));
    }
    MyStruct to_insert(100, "inserted");
    v.insert(1, std::move(to_insert));
    assert(v.get_size() == 4);
    assert(v[1].x == 100);
    assert(v[1].s == "inserted");
    assert(to_insert.x == 0);
    assert(to_insert.s == "");
    assert(v[2].x == 1);
    std::cout << "insert(index) move test passed.\n";
}

void testEraseIndex() {
    std::cout << "Testing erase(index)...\n";
    raw::vector<std::string> v;
    for (int i = 0; i < 5; ++i) {
        v.push_back("string" + std::to_string(i));
    }
    printVector<std::string>(v);
    v.erase(2);
    printVector<std::string>(v);
    assert(v.get_size() == 4);
    assert(v[2] == "string3");
    v.erase(0);
	printVector<std::string>(v);
    assert(v.get_size() == 3);
    assert(v[0] == "string1");
    v.erase(v.get_size() - 1); // Erase the last element
    assert(v.get_size() == 2);
    printVector<std::string>(v);
    assert(v[v.get_size() - 1] == "string3");

    try {
        v.erase(v.get_size()); // out of range
        assert(false);
    }
    catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    std::cout << "erase(index) test passed.\n";
}

void testInsertIterator() {
    std::cout << "Testing insert(iterator)...\n";
    raw::vector<std::string> v;
    for (int i = 0; i < 3; ++i) {
        v.push_back("string" + std::to_string(i));
    }
    auto it = v.insert(v.begin() + 1, "inserted");
    assert(v.get_size() == 4);
    assert(*it == "inserted");
    assert(v[1] == "inserted");
    assert(v[2] == "string1");
    std::cout << "insert(iterator) test passed.\n";
}

void testInsertIteratorMove() {
    std::cout << "Testing insert(iterator) with move semantics...\n";
    raw::vector<MyStruct> v;
    for (int i = 0; i < 3; ++i) {
        v.push_back(MyStruct(i, "original" + std::to_string(i)));
    }
    MyStruct to_insert(100, "inserted");
    auto it = v.insert(v.begin() + 1, std::move(to_insert));
    assert(v.get_size() == 4);
    assert(it->x == 100);
    assert(it->s == "inserted");
    assert(to_insert.x == 0);
    assert(to_insert.s == "");
    assert(v[2].x == 1);
    std::cout << "insert(iterator) move test passed.\n";
}

void testEraseIterator() {
    std::cout << "Testing erase(iterator)...\n";
    raw::vector<std::string> v;
    for (int i = 0; i < 3; ++i) {
        v.push_back("string" + std::to_string(i));
    }
    auto it = v.erase(v.begin() + 1);
    assert(v.get_size() == 2);
    assert(*it == "string2");
    assert(v[1] == "string2");
    std::cout << "erase(iterator) test passed.\n";
}

void testBeginEnd() {
    std::cout << "Testing begin() and end()...\n";
    raw::vector<std::string> v;
    for (int i = 0; i < 3; ++i) {
        v.push_back("string" + std::to_string(i));
    }
    auto it = v.begin();
    assert(*it == "string0");
    ++it;
    assert(*it == "string1");
    auto it_end = v.end();
    size_t count = 0;
    for (auto it_begin = v.begin(); it_begin != v.end(); ++it_begin) {
        ++count;
    }
    assert(count == v.get_size());
    std::cout << "begin() and end() test passed.\n";
}

void testConstBeginEnd() {
    std::cout << "Testing const begin() and end()...\n";
    const raw::vector<std::string> v(3);
    for (int i = 0; i < 3; ++i) {
        ((raw::vector<std::string>&)v)[i] = "string" + std::to_string(i); // Workaround to modify const vector for testing
    }
    auto it = v.begin();
    assert(*it == "string0");
    ++it;
    assert(*it == "string1");
    auto it_end = v.end();
    size_t count = 0;
    for (auto it_begin = v.begin(); it_begin != v.end(); ++it_begin) {
        ++count;
    }
    assert(count == v.get_size());
    std::cout << "const begin() and end() test passed.\n";
}

void testIteratorArithmetic() {
    std::cout << "Testing iterator arithmetic...\n";
    raw::vector<std::string> v;
    for (int i = 0; i < 5; ++i) {
        v.push_back("string" + std::to_string(i));
    }
    auto it = v.begin() + 2;
    assert(*it == "string2");
    it = 2 + v.begin();
    assert(*it == "string2");
    it = v.end() - 1;
    assert(*it == "string4");
    std::cout << "iterator arithmetic test passed.\n";
}

void testIteratorComparison() {
    std::cout << "Testing iterator comparison...\n";
    raw::vector<std::string> v;
    for (int i = 0; i < 3; ++i) {
        v.push_back("string" + std::to_string(i));
    }
    auto it1 = v.begin();
    auto it2 = v.begin() + 1;
    assert(it1 < it2);
    assert(it2 > it1);
    assert(it1 != it2);
    it1 = v.begin() + 2;
    it2 = v.end() - 1;
    assert(it1 == it2);
    std::cout << "iterator comparison test passed.\n";
}

void testSwap() {
    std::cout << "Testing swap()...\n";
    raw::vector<std::string> v1(2);
    v1[0] = "one";
    v1[1] = "two";
    raw::vector<std::string> v2(3);
    v2[0] = "three";
    v2[1] = "four";
    v2[2] = "five";
    v1.swap(v2);
    assert(v1.get_size() == 3);
    assert(v1[0] == "three");
    assert(v1[1] == "four");
    assert(v1[2] == "five");
    assert(v2.get_size() == 2);
    assert(v2[0] == "one");
    assert(v2[1] == "two");
    std::cout << "swap() test passed.\n";
}

void testEmpty() {
    std::cout << "Testing empty()...\n";
    raw::vector<std::string> v;
    assert(v.empty());
    v.push_back("test");
    assert(!v.empty());
    v.clear();
    assert(v.empty());
    std::cout << "empty() test passed.\n";
}

void testExceptionSafety() {
    std::cout << "Testing exception safety...\n";
    try {
        raw::vector<std::string> v;
        for (int i = 0; i < 10; ++i) {
            v.push_back("string" + std::to_string(i));
        }
        // Simulate an exception during push_back (e.g., bad_alloc)
        // This is difficult to simulate directly without modifying the vector's internals.
        // Instead, we'll test that the vector is still in a consistent state after an operation that *could* throw.
        v.reserve(std::numeric_limits<size_t>::max()); // This could throw bad_alloc
        assert(v.get_size() == 10);
        assert(v.get_capacity() >= 10);
    }
    catch (const std::bad_alloc& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cout << "Caught unexpected exception.\n";
        assert(false);
    }
    std::cout << "Exception safety test (basic) passed.\n";
}

void testMyStructDestructorCalls() {
    std::cout << "Testing MyStruct destructor calls during vector operations...\n";
    {
        raw::vector<MyStruct> v;
        v.push_back(MyStruct(1, "one"));
        v.push_back(MyStruct(2, "two"));
        v.pop_back();
        v.clear();
    } // Destructors should be called here
    std::cout << "MyStruct destructor calls test passed.\n";
}

void testMyStructCopyAndMove() {
    std::cout << "Testing MyStruct copy and move operations...\n";
    {
        raw::vector<MyStruct> v;
        MyStruct ms1(1, "one");
        v.push_back(ms1); // Copy
        assert(v[0].x == 1);
        assert(v[0].s == "one");
        MyStruct ms2(2, "two");
        v.push_back(std::move(ms2)); // Move
        assert(v[1].x == 2);
        assert(v[1].s == "two");
        assert(ms2.x == 0);
        assert(ms2.s == "");
    }
    std::cout << "MyStruct copy and move test passed.\n";
}

void testStressTest() {
    std::cout << "Testing stress test...\n";
    const size_t num_elements = 1000;
    raw::vector<int> v;
    std::vector<int> std_v;

    // Push back
    for (size_t i = 0; i < num_elements; ++i) {
        v.push_back(i);
        std_v.push_back(i);
        assert(compareVectors(v, std_v));
    }

    // Insert
    for (size_t i = 0; i < num_elements / 2; ++i) {
        size_t index = rand() % v.get_size();
        int value = rand();
        v.insert(index, value);
        std_v.insert(std_v.begin() + index, value);
        assert(compareVectors(v, std_v));
    }

    // Erase
    for (size_t i = 0; i < num_elements / 4; ++i) {
        if (v.get_size() > 0) {
            size_t index = rand() % v.get_size();
            v.erase(index);
            std_v.erase(std_v.begin() + index);
            assert(compareVectors(v, std_v));
        }
    }

    // Resize
    v.resize(num_elements / 3);
    std_v.resize(num_elements / 3);
    assert(compareVectors(v, std_v));

    // Clear
    v.clear();
    std_v.clear();
    assert(compareVectors(v, std_v));

    std::cout << "Stress test passed.\n";
}

void testCombinedOperations() {
    std::cout << "Testing combined operations (push_back, insert, erase, resize, clear, etc.) - STRESS TEST!\n";
    raw::vector<std::string> v;
    std::vector<std::string> std_v;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 19); // 20 operations
    std::uniform_int_distribution<> size_dist(0, 30); // Random sizes
    std::uniform_int_distribution<> string_length_dist(1, 10);

    for (int i = 0; i < 100000; ++i) { // Increased iterations
        int operation = dist(gen);

        std::cout << "Iteration " << i << ", Operation: " << operation << std::endl;
        std::cout << "  raw::vector: ";
        printVector<std::string>(v);
        std::cout << "  std::vector: ";
        printVector(std_v);

        switch (operation) {
        case 0: { // push_back
            std::string value = "push_" + std::to_string(i);
            v.push_back(value);
            std_v.push_back(value);
            std::cout << "    push_back(\"" << value << "\")" << std::endl;
            break;
        }
        case 1: { // insert (index)
            if (v.get_size() > 0) {
                size_t index = rand() % (v.get_size() + 1); // Allow insert at end
                std::string value = "insert_" + std::to_string(i);
                v.insert(index, value);
                std_v.insert(std_v.begin() + index, value);
                std::cout << "    insert(" << index << ", \"" << value << "\")" << std::endl;
            }
            else {
                std::cout << "    insert skipped (empty)" << std::endl;
            }
            break;
        }
        case 2: { // insert (iterator)
            if (v.get_size() > 0) {
                size_t index = rand() % (v.get_size() + 1);
                std::string value = "insert_it_" + std::to_string(i);
                v.insert(v.begin() + index, value);
                std_v.insert(std_v.begin() + index, value);
                std::cout << "    insert(it + " << index << ", \"" << value << "\")" << std::endl;
            }
            else {
                std::cout << "    insert(iterator) skipped (empty)" << std::endl;
            }
            break;
        }
        case 3: { // erase (index)
            if (v.get_size() > 0) {
                size_t index = rand() % v.get_size();
                v.erase(index);
                std_v.erase(std_v.begin() + index);
                std::cout << "    erase(" << index << ")" << std::endl;
            }
            else {
                std::cout << "    erase(index) skipped (empty)" << std::endl;
            }
            break;
        }
        case 4: { // erase (iterator)
            if (v.get_size() > 0) {
                size_t index = rand() % v.get_size();
                v.erase(v.begin() + index);
                std_v.erase(std_v.begin() + index);
                std::cout << "    erase(it + " << index << ")" << std::endl;
            }
            else {
                std::cout << "    erase(iterator) skipped (empty)" << std::endl;
            }
            break;
        }
        case 5: { // resize
            size_t new_size = size_dist(gen); // Random size from size_dist
            std::cout << "    resize(" << new_size << ")" << std::endl;
            v.resize(new_size);
            std_v.resize(new_size);
            break;
        }
        case 6: { // reserve
            size_t reserve_size = size_dist(gen); // Random size from size_dist
            std::cout << "    reserve(" << reserve_size << ")" << std::endl;
            v.reserve(reserve_size);
            break;
        }
        case 7: { // clear
            std::cout << "    clear()" << std::endl;
            v.clear();
            std_v.clear();
            break;
        }
        case 8: { // shrink_to_fit
            std::cout << "    shrink_to_fit()" << std::endl;
            v.shrink_to_fit();
            break;
        }
        case 9: { // pop_back
            if (v.get_size() > 0) {
                std::cout << "    pop_back()" << std::endl;
                v.pop_back();
                std_v.pop_back();
            }
            else {
                std::cout << "    pop_back skipped (empty)" << std::endl;
            }
            break;
        }
        case 10: { // at
            if (v.get_size() > 0) {
                size_t index = rand() % v.get_size();
                std::cout << "    at(" << index << ")" << std::endl;
                try {
                    std::string val = v.at(index);
                    std::string std_val = std_v.at(index);
                    assert(val == std_val);
                }
                catch (const std::out_of_range& e) {
                    std::cout << "Caught expected exception: " << e.what() << std::endl;
                }
            }
            else {
                std::cout << "    at skipped (empty)" << std::endl;
            }
            break;
        }
        case 11: { // insert multiple (index)
            if (v.get_size() > 0) {
                size_t index = rand() % (v.get_size() + 1);
                size_t num_inserts = rand() % 5; // Insert up to 5 elements
                for (size_t j = 0; j < num_inserts; ++j) {
                    std::string value = "multi_insert_" + std::to_string(i) + "_" + std::to_string(j);
                    v.insert(index + j, value);
                    std_v.insert(std_v.begin() + index + j, value);
                }
                std::cout << "    insert (multiple) at " << index << " count " << num_inserts << std::endl;
            }
            else {
                std::cout << "    insert (multiple) skipped (empty)" << std::endl;
            }
            break;
        }
        case 12: { // insert multiple (iterator)
            if (v.get_size() > 0) {
                size_t index = rand() % (v.get_size() + 1);
                size_t num_inserts = rand() % 5;
                for (size_t j = 0; j < num_inserts; ++j) {
                    std::string value = "multi_insert_it_" + std::to_string(i) + "_" + std::to_string(j);
                    v.insert(v.begin() + index + j, value);
                    std_v.insert(std_v.begin() + index + j, value);
                }
                std::cout << "    insert (multiple it) at " << index << " count " << num_inserts << std::endl;
            }
            else {
                std::cout << "    insert (multiple it) skipped (empty)" << std::endl;
            }
            break;
        }
        case 13: { // erase multiple (index)
            if (v.get_size() > 0) {
                size_t index = rand() % v.get_size();
                size_t num_erases = rand() % std::min((size_t)5, v.get_size() - index); // Erase up to 5 or to the end
                for (size_t j = 0; j < num_erases; ++j) {
                    v.erase(index);
                    std_v.erase(std_v.begin() + index);
                }
                std::cout << "    erase (multiple) from " << index << " count " << num_erases << std::endl;
            }
            else {
                std::cout << "    erase (multiple) skipped (empty)" << std::endl;
            }
            break;
        }
        case 14: { // erase multiple (iterator)
            if (v.get_size() > 0) {
                size_t index = rand() % v.get_size();
                size_t num_erases = rand() % std::min((size_t)5, v.get_size() - index);
                for (size_t j = 0; j < num_erases; ++j) {
                    v.erase(v.begin() + index);
                    std_v.erase(std_v.begin() + index);
                }
                std::cout << "    erase (multiple it) from " << index << " count " << num_erases << std::endl;
            }
            else {
                std::cout << "    erase (multiple it) skipped (empty)" << std::endl;
            }
            break;
        }
        case 15: { // push_back with random string length
            size_t str_len = string_length_dist(gen);
            std::string random_str(str_len, ' ');
            for (size_t j = 0; j < str_len; ++j) {
                random_str[j] = 'a' + (rand() % 26); // Random lowercase letters
            }
            v.push_back(random_str);
            std_v.push_back(random_str);
            std::cout << "    push_back (random string) \"" << random_str << "\"" << std::endl;
            break;
        }
        case 16: { // copy constructor test
            if (v.get_size() > 0) {
                raw::vector<std::string> v_copy(v);
                std::vector<std::string> std_v_copy(std_v);
                assert(compareVectors(v_copy, std_v_copy));
                std::cout << "    copy constructor test" << std::endl;
            }
            else {
                std::cout << "    copy constructor skipped (empty)" << std::endl;
            }
            break;
        }
        case 17: { // move constructor test
            if (v.get_size() > 0) {
                raw::vector<std::string> v_move(std::move(v));
                std::vector<std::string> std_v_move(std::move(std_v));
                assert(compareVectors(v_move, std_v_move));
                v = raw::vector<std::string>(); // Reset v
                std_v = std::vector<std::string>(); // Reset std_v
                std::cout << "    move constructor test" << std::endl;
            }
            else {
                std::cout << "    move constructor skipped (empty)" << std::endl;
            }
            break;
        }
        case 18: { // copy assignment test
            if (v.get_size() > 0) {
                raw::vector<std::string> v_copy;
                v_copy = v;
                std::vector<std::string> std_v_copy;
                std_v_copy = std_v;
                assert(compareVectors(v_copy, std_v_copy));
                std::cout << "    copy assignment test" << std::endl;
            }
            else {
                std::cout << "    copy assignment skipped (empty)" << std::endl;
            }
            break;
        }
        case 19: { // move assignment test
            if (v.get_size() > 0) {
                raw::vector<std::string> v_move;
                v_move = std::move(v);
                std::vector<std::string> std_v_move;
                std_v_move = std::move(std_v);
                v = raw::vector<std::string>(); // Reset v
                std_v = std::vector<std::string>(); // Reset std_v
                assert(compareVectors(v_move, std_v_move));
                std::cout << "    move assignment test" << std::endl;
            }
            else {
                std::cout << "    move assignment skipped (empty)" << std::endl;
            }
            break;
        }
        }
        std::cout << "  After operation raw::vector: ";
        printVector<std::string>(v);
        std::cout << "  After operation std::vector: ";
        printVector(std_v);
        assert(compareVectors(v, std_v)); // Verify after each operation
    }
    std::cout << "Combined operations test passed - STRESS TEST COMPLETE!\n";
}

int main() {
    testDefaultConstructor();
    testSizeConstructor();
    testCopyConstructor();
    testMoveConstructor();
    testCopyAssignment();
    testMoveAssignment();
    testPushBack();
    testPushBackMove();
    testAt();
    testAtConst();
    testOperatorBrackets();
    testResize();
    testReserve();
    testClear();
    testShrinkToFit();
    testPopBack();
    testInsertIndex();
    testInsertIndexMove();
    testEraseIndex();
    testInsertIterator();
    testInsertIteratorMove();
    testEraseIterator();
    testBeginEnd();
    testConstBeginEnd();
    testIteratorArithmetic();
    testIteratorComparison();
    testSwap();
    testEmpty();
    testMyStructDestructorCalls();
    testMyStructCopyAndMove();
    testStressTest();
     testCombinedOperations();

    std::cout << "All tests passed!\n";
    return 0;
}

