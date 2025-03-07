#include "RawNamespace\vector\vector_alias.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <random>


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

void testDefaultConstructorInt() {
    std::cout << "Testing default constructor for int...\n";
    raw::vector<int> v;
    assert(v.get_size() == 0);
    assert(v.get_capacity() >= 0); // Capacity should be at least 1
    std::cout << "Default constructor test for int passed.\n";
}

void testSizeConstructorInt() {
    std::cout << "Testing size constructor for int...\n";
    size_t size = 10;
    raw::vector<int> v(size);
    assert(v.get_size() == size);
    assert(v.get_capacity() >= size);
    for (size_t i = 0; i < size; ++i) {
        assert(v[i] == 0); // Default constructed ints should be 0
    }
    std::cout << "Size constructor test for int passed.\n";
}

void testCopyConstructorInt() {
    std::cout << "Testing copy constructor for int...\n";
    raw::vector<int> v1(5);
    for (int i = 0; i < 5; ++i) {
        v1[i] = i;
    }
    raw::vector<int> v2 = v1;
    assert(v2.get_size() == v1.get_size());
    assert(v2.get_capacity() == v1.get_capacity());
    for (size_t i = 0; i < v1.get_size(); ++i) {
        assert(v2[i] == v1[i]);
    }
    std::cout << "Copy constructor test for int passed.\n";
}

void testMoveConstructorInt() {
    std::cout << "Testing move constructor for int...\n";
    raw::vector<int> v1(5);
    for (int i = 0; i < 5; ++i) {
        v1[i] = i;
    }
    size_t old_capacity = v1.get_capacity();
    raw::vector<int> v2 = std::move(v1);
    assert(v2.get_size() == 5);
    assert(v2.get_capacity() == old_capacity);
    for (int i = 0; i < 5; ++i) {
        assert(v2[i] == i);
    }
    assert(v1.get_size() == 0);
    assert(v1.get_capacity() == 0);
    std::cout << "Move constructor test for int passed.\n";
}

void testCopyAssignmentInt() {
    std::cout << "Testing copy assignment for int...\n";
    raw::vector<int> v1(5);
    for (int i = 0; i < 5; ++i) {
        v1[i] = i;
    }
    raw::vector<int> v2;
    v2 = v1;
    assert(v2.get_size() == v1.get_size());
    assert(v2.get_capacity() == v1.get_capacity());
    for (size_t i = 0; i < v1.get_size(); ++i) {
        assert(v2[i] == v1[i]);
    }
    std::cout << "Copy assignment test for int passed.\n";
}

void testMoveAssignmentInt() {
    std::cout << "Testing move assignment for int...\n";
    raw::vector<int> v1(5);
    for (int i = 0; i < 5; ++i) {
        v1[i] = i;
    }
    size_t old_capacity = v1.get_capacity();
    raw::vector<int> v2;
    v2 = std::move(v1);
    assert(v2.get_size() == 5);
    assert(v2.get_capacity() == old_capacity);
    for (int i = 0; i < 5; ++i) {
        assert(v2[i] == i);
    }
    assert(v1.get_size() == 0);
    assert(v1.get_capacity() == 0);
    std::cout << "Move assignment test for int passed.\n";
}

void testPushBackInt() {
    std::cout << "Testing push_back for int...\n";
    raw::vector<int> v;
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
        assert(v.get_size() == i + 1);
        assert(v.get_capacity() >= v.get_size());
        assert(v[i] == i);
    }
    std::cout << "push_back test for int passed.\n";
}

void testAtInt() {
    std::cout << "Testing at() for int...\n";
    raw::vector<int> v(5);
    for (int i = 0; i < 5; ++i) {
        v[i] = i;
    }
    for (int i = 0; i < 5; ++i) {
        assert(v.at(i) == i);
    }
    try {
        v.at(5);
        assert(false); // Should have thrown an exception
    }
    catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    std::cout << "at() test for int passed.\n";
}

void testOperatorBracketsInt() {
    std::cout << "Testing operator[] for int...\n";
    raw::vector<int> v(5);
    for (int i = 0; i < 5; ++i) {
        v[i] = i;
    }
    for (int i = 0; i < 5; ++i) {
        assert(v[i] == i);
    }
    std::cout << "operator[] test for int passed.\n";
}

void testResizeInt() {
    std::cout << "Testing resize() for int...\n";
    raw::vector<int> v;
    v.resize(5);
    assert(v.get_size() == 5);
    assert(v.get_capacity() >= 5);
    for (int i = 0; i < 5; ++i) {
        assert(v[i] == 0);
    }
    v.resize(2);
    assert(v.get_size() == 2);
    assert(v.get_capacity() >= 2);
    v.resize(8);
    assert(v.get_size() == 8);
    assert(v.get_capacity() >= 8);
    for (int i = 2; i < 8; ++i) {
        assert(v[i] == 0);
    }
    std::cout << "resize() test for int passed.\n";
}

void testReserveInt() {
    std::cout << "Testing reserve() for int...\n";
    raw::vector<int> v;
    v.reserve(10);
    assert(v.get_capacity() >= 10);
    v.push_back(1);
    v.reserve(5); // Should not change capacity
    assert(v.get_capacity() >= 10);
    std::cout << "reserve() test for int passed.\n";
}

void testClearInt() {
    std::cout << "Testing clear() for int...\n";
    raw::vector<int> v(5);
    for (int i = 0; i < 5; ++i) {
        v[i] = i;
    }
    v.clear();
    assert(v.get_size() == 0);
    assert(v.get_capacity() == 1); // Capacity should be reset to 1
    std::cout << "clear() test for int passed.\n";
}

void testShrinkToFitInt() {
    std::cout << "Testing shrink_to_fit() for int...\n";
    raw::vector<int> v(10);
    for (int i = 0; i < 10; ++i) {
        v[i] = i;
    }
    v.resize(5);
    size_t old_capacity = v.get_capacity();
    v.shrink_to_fit();
    assert(v.get_size() == 5);
    assert(v.get_capacity() == 5);
    std::cout << "shrink_to_fit() test for int passed.\n";
}

void testPopBackInt() {
    std::cout << "Testing pop_back() for int...\n";
    raw::vector<int> v;
    for (int i = 0; i < 5; ++i) {
        v.push_back(i);
    }
    v.pop_back();
    assert(v.get_size() == 4);
    assert(v[3] == 3);
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
    std::cout << "pop_back() test for int passed.\n";
}

void testInsertIndexInt() {
    std::cout << "Testing insert(index) for int...\n";
    raw::vector<int> v;
    for (int i = 0; i < 5; ++i) {
        v.push_back(i);
    }
    v.insert(2, 100);
    assert(v.get_size() == 6);
    assert(v[2] == 100);
    assert(v[3] == 2);
    v.insert(0, 200);
    assert(v.get_size() == 7);
    assert(v[0] == 200);
    assert(v[1] == 0);
    v.insert(v.get_size(), 300); // Insert at the end
    assert(v.get_size() == 8);
    assert(v[v.get_size() - 1] == 300);

    try {
        v.insert(v.get_size() + 1, 400); // out of range
        assert(false);
    }
    catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    std::cout << "insert(index) test for int passed.\n";
}

void testEraseIndexInt() {
    std::cout << "Testing erase(index) for int...\n";
    raw::vector<int> v;
    for (int i = 0; i < 5; ++i) {
        v.push_back(i);
    }
    v.erase(2);
    assert(v.get_size() == 4);
    assert(v[2] == 3);
    v.erase(0);
    assert(v.get_size() == 3);
    assert(v[0] == 1);
    v.erase(v.get_size() - 1); // Erase the last element
    assert(v.get_size() == 2);
    assert(v[v.get_size() - 1] == 3);

    try {
        v.erase(v.get_size()); // out of range
        assert(false);
    }
    catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }
    std::cout << "erase(index) test for int passed.\n";
}

void testInsertIteratorInt() {
    std::cout << "Testing insert(iterator) for int...\n";
    raw::vector<int> v;
    for (int i = 0; i < 3; ++i) {
        v.push_back(i);
    }
    auto it = v.insert(v.begin() + 1, 100);
    assert(v.get_size() == 4);
    assert(*it == 100);
    assert(v[1] == 100);
    assert(v[2] == 1);
    std::cout << "insert(iterator) test for int passed.\n";
}

void testEraseIteratorInt() {
    std::cout << "Testing erase(iterator) for int...\n";
    raw::vector<int> v;
    for (int i = 0; i < 3; ++i) {
        v.push_back(i);
    }
    auto it = v.erase(v.begin() + 1);
    assert(v.get_size() == 2);
    assert(*it == 2);
    assert(v[1] == 2);
    std::cout << "erase(iterator) test for int passed.\n";
}

void testBeginEndInt() {
    std::cout << "Testing begin() and end() for int...\n";
    raw::vector<int> v;
    for (int i = 0; i < 3; ++i) {
        v.push_back(i);
    }
    auto it = v.begin();
    assert(*it == 0);
    ++it;
    assert(*it == 1);
    auto it_end = v.end();
    size_t count = 0;
    for (auto it_begin = v.begin(); it_begin != v.end(); ++it_begin) {
        ++count;
    }
    assert(count == v.get_size());
    std::cout << "begin() and end() test for int passed.\n";
}

void testConstBeginEndInt() {
    std::cout << "Testing const begin() and end() for int...\n";
    const raw::vector<int> v(3);
    for (int i = 0; i < 3; ++i) {
        ((raw::vector<int>&)v)[i] = i; // Workaround to modify const vector for testing
    }
    auto it = v.begin();
    assert(*it == 0);
    ++it;
    assert(*it == 1);
    auto it_end = v.end();
    size_t count = 0;
    for (auto it_begin = v.begin(); it_begin != v.end(); ++it_begin) {
        ++count;
    }
    assert(count == v.get_size());
    std::cout << "const begin() and end() test for int passed.\n";
}

void testIteratorArithmeticInt() {
    std::cout << "Testing iterator arithmetic for int...\n";
    raw::vector<int> v;
    for (int i = 0; i < 5; ++i) {
        v.push_back(i);
    }
    auto it = v.begin() + 2;
    assert(*it == 2);
    it = 2 + v.begin();
    assert(*it == 2);
    it = v.end() - 1;
    assert(*it == 4);
    std::cout << "iterator arithmetic test for int passed.\n";
}

void testIteratorComparisonInt() {
    std::cout << "Testing iterator comparison for int...\n";
    raw::vector<int> v;
    for (int i = 0; i < 3; ++i) {
        v.push_back(i);
    }
    auto it1 = v.begin();
    auto it2 = v.begin() + 1;
    assert(it1 < it2);
    assert(it2 > it1);
    assert(it1 != it2);
    it1 = v.begin() + 2;
    it2 = v.end() - 1;
    assert(it1 == it2);
    std::cout << "iterator comparison test for int passed.\n";
}

void testSwapInt() {
    std::cout << "Testing swap() for int...\n";
    raw::vector<int> v1(2);
    v1[0] = 1;
    v1[1] = 2;
    raw::vector<int> v2(3);
    v2[0] = 3;
    v2[1] = 4;
    v2[2] = 5;
    v1.swap(v2);
    assert(v1.get_size() == 3);
    assert(v1[0] == 3);
    assert(v1[1] == 4);
    assert(v1[2] == 5);
    assert(v2.get_size() == 2);
    assert(v2[0] == 1);
    assert(v2[1] == 2);
    std::cout << "swap() test for int passed.\n";
}

void testEmptyInt() {
    std::cout << "Testing empty() for int...\n";
    raw::vector<int> v;
    assert(v.empty());
    v.push_back(1);
    assert(!v.empty());
    v.clear();
    assert(v.empty());
    std::cout << "empty() test for int passed.\n";
}

void testStressTestInt() {
    std::cout << "Testing stress test for int...\n";
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

    std::cout << "Stress test for int passed.\n";
}

void testCombinedOperationsInt() {
    std::cout << "Testing combined operations (push_back, insert, erase, resize, clear, etc.) for int - STRESS TEST!\n";
    raw::vector<int> v;
    std::vector<int> std_v;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 19); // 20 operations
    std::uniform_int_distribution<> size_dist(0, 30); // Random sizes

    for (int i = 0; i < 100000; ++i) { // Increased iterations
        int operation = dist(gen);

        switch (operation) {
        case 0: { // push_back
            int value = i;
            v.push_back(value);
            std_v.push_back(value);
            break;
        }
        case 1: { // insert (index)
            if (v.get_size() > 0) {
                size_t index = rand() % (v.get_size() + 1); // Allow insert at end
                int value = i;
                v.insert(index, value);
                std_v.insert(std_v.begin() + index, value);
            }
            break;
        }
        case 2: { // insert (iterator)
            if (v.get_size() > 0) {
                size_t index = rand() % (v.get_size() + 1);
                int value = i;
                v.insert(v.begin() + index, value);
                std_v.insert(std_v.begin() + index, value);
            }
            break;
        }
        case 3: { // erase (index)
            if (v.get_size() > 0) {
                size_t index = rand() % v.get_size();
                v.erase(index);
                std_v.erase(std_v.begin() + index);
            }
            break;
        }
        case 4: { // erase (iterator)
            if (v.get_size() > 0) {
                size_t index = rand() % v.get_size();
                v.erase(v.begin() + index);
                std_v.erase(std_v.begin() + index);
            }
            break;
        }
        case 5: { // resize
            size_t new_size = size_dist(gen); // Random size from size_dist
            v.resize(new_size);
            std_v.resize(new_size);
            break;
        }
        case 6: { // reserve
            size_t reserve_size = size_dist(gen); // Random size from size_dist
            v.reserve(reserve_size);
            break;
        }
        case 7: { // clear
            v.clear();
            std_v.clear();
            break;
        }
        case 8: { // shrink_to_fit
            v.shrink_to_fit();
            break;
        }
        case 9: { // pop_back
            if (v.get_size() > 0) {
                v.pop_back();
                std_v.pop_back();
            }
            break;
        }
        case 10: { // at
            if (v.get_size() > 0) {
                size_t index = rand() % v.get_size();
                try {
                    int val = v.at(index);
                    int std_val = std_v.at(index);
                    assert(val == std_val);
                }
                catch (const std::out_of_range& e) {
                    std::cout << "Caught expected exception: " << e.what() << std::endl;
                }
            }
            break;
        }
        case 11: { // insert multiple (index)
            if (v.get_size() > 0) {
                size_t index = rand() % (v.get_size() + 1);
                size_t num_inserts = rand() % 5; // Insert up to 5 elements
                for (size_t j = 0; j < num_inserts; ++j) {
                    int value = i + j;
                    v.insert(index + j, value);
                    std_v.insert(std_v.begin() + index + j, value);
                }
            }
            break;
        }
        case 12: { // insert multiple (iterator)
            if (v.get_size() > 0) {
                size_t index = rand() % (v.get_size() + 1);
                size_t num_inserts = rand() % 5;
                for (size_t j = 0; j < num_inserts; ++j) {
                    int value = i + j;
                    v.insert(v.begin() + index + j, value);
                    std_v.insert(std_v.begin() + index + j, value);
                }
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
            }
            break;
        }
        case 15: { // push_back with random value
            int random_value = rand();
            v.push_back(random_value);
            std_v.push_back(random_value);
            break;
        }
        case 16: { // copy constructor test
            if (v.get_size() > 0) {
                raw::vector<int> v_copy(v);
                std::vector<int> std_v_copy(std_v);
                assert(compareVectors(v_copy, std_v_copy));
            }
            break;
        }
        case 17: { // move constructor test
            if (v.get_size() > 0) {
                raw::vector<int> v_move(std::move(v));
                std::vector<int> std_v_move(std::move(std_v));
                assert(compareVectors(v_move, std_v_move));
                v = raw::vector<int>(); // Reset v
                std_v = std::vector<int>(); // Reset std_v
            }
            break;
        }
        case 18: { // copy assignment test
            if (v.get_size() > 0) {
                raw::vector<int> v_copy;
                v_copy = v;
                std::vector<int> std_v_copy;
                std_v_copy = std_v;
                assert(compareVectors(v_copy, std_v_copy));
            }
            break;
        }
        case 19: { // move assignment test
            if (v.get_size() > 0) {
                raw::vector<int> v_move;
                v_move = std::move(v);
                std::vector<int> std_v_move;
                std_v_move = std::move(std_v);
                v = raw::vector<int>(); // Reset v
                std_v = std::vector<int>(); // Reset std_v
                assert(compareVectors(v_move, std_v_move));
            }
            break;
        }
        }
        assert(compareVectors(v, std_v)); // Verify after each operation
    }
    std::cout << "Combined operations test for int passed - STRESS TEST COMPLETE!\n";
}


int main() {
    testDefaultConstructorInt();
    testSizeConstructorInt();
    testCopyConstructorInt();
    testMoveConstructorInt();
    testCopyAssignmentInt();
    testMoveAssignmentInt();
    testPushBackInt();
    testAtInt();
    testOperatorBracketsInt();
    testResizeInt();
    testReserveInt();
    testClearInt();
    testShrinkToFitInt();
    testPopBackInt();
    testInsertIndexInt();
    testEraseIndexInt();
    testInsertIteratorInt();
    testEraseIteratorInt();
    testBeginEndInt();
    testConstBeginEndInt();
    testIteratorArithmeticInt();
    testIteratorComparisonInt();
    testSwapInt();
    testEmptyInt();
    testStressTestInt();
    testCombinedOperationsInt();

    std::cout << "All int tests passed!\n";
    return 0;
}
