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
    bool operator==(const MyStruct& other) const {
        return x == other.x && s == other.s;
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

int main() {
    {
        std::cout << "Starting HARDCORE tests...\n";
        std::cout << "\n--- Test 1: Push_back, Resize, Reserve, Clear (string) ---\n";
        raw::vector<std::string> test1;
        std::cout << "Created test1. get_size(): " << test1.get_size() << "\n";
        assert(test1.get_size() == 0);

        test1.push_back("apple");
        test1.push_back("banana");
        test1.push_back("cherry");
        printVector<std::string>(test1);
        std::cout << "Pushed 3 strings. get_size(): " << test1.get_size() << "\n";
        assert(test1.get_size() == 3);
        assert(test1[0] == "apple");
        assert(test1[1] == "banana");
        assert(test1[2] == "cherry");

        test1.resize(2);
        std::cout << "Resized to 2. get_size(): " << test1.get_size() << "\n";
        assert(test1.get_size() == 2);
        assert(test1[0] == "apple");
        assert(test1[1] == "banana");
        printVector<std::string>(test1);
        test1.reserve(5);
        printVector<std::string>(test1);
        std::cout << "Reserved 5. get_size(): " << test1.get_size() << "\n";
        assert(test1.get_size() == 2);
        test1.push_back("date");
        assert(test1.get_size() == 3);
        assert(test1[2] == "date");

        test1.clear();
        std::cout << "Cleared. get_size(): " << test1.get_size() << "\n";
        assert(test1.get_size() == 0);
        assert(test1.empty());
        std::cout << "Test 1 passed\n";
    }

    {
        std::cout << "\n--- Test 2: Insert, Erase, Iterator Invalidation (string) ---\n";
        raw::vector<std::string> test2;
        test2.push_back("hello");
        test2.push_back("world");
        test2.push_back("!");
        std::cout << "Pushed 3 strings. get_size(): " << test2.get_size() << "\n";
        assert(test2.get_size() == 3);

        test2.insert(1, "beautiful");
        std::cout << "Inserted \"beautiful\" at index 1. get_size(): " << test2.get_size() << "\n";
        assert(test2.get_size() == 4);
        assert(test2[1] == "beautiful");
        assert(test2[2] == "world");

        test2.erase(0);
        std::cout << "Erased element at 0. get_size(): " << test2.get_size() << "\n";
        assert(test2.get_size() == 3);
        assert(test2[0] == "beautiful");

        test2.push_back("again");
        std::cout << "Pushed \"again\". get_size(): " << test2.get_size() << "\n";
        assert(test2.get_size() == 4);

        std::cout << "Test 2 passed\n";

        std::cout << "\n--- Test 3: Large Size, Copy/Move (string) ---\n";
        raw::vector<std::string> test3;
        const int largeSize = 1000;
        for (int i = 0; i < largeSize; ++i) {
            test3.push_back("string" + std::to_string(i));
        }
        std::cout << "Pushed " << largeSize << " strings. get_size(): " << test3.get_size() << "\n";
        assert(test3.get_size() == largeSize);
        for (int i = 0; i < largeSize; ++i) {
            assert(test3[i] == "string" + std::to_string(i));
        }

        raw::vector<std::string> test3_copy = test3;
        std::cout << "Copied test3. get_size(): " << test3_copy.get_size() << "\n";
        assert(test3_copy.get_size() == largeSize);
        assert(compareVectors(test3, toStdVector<std::string>(test3_copy)));

        raw::vector<std::string> test3_move = std::move(test3_copy);
        std::cout << "Moved test3_copy to test3_move. test3_move.get_size(): " << test3_move.get_size() << ", test3_copy.get_size(): " << test3_copy.get_size() << "\n";
        assert(test3_move.get_size() == largeSize);
        assert(test3_copy.get_size() == 0);
        for (int i = 0; i < largeSize; ++i) {
            assert(test3_move[i] == "string" + std::to_string(i));
        }
        std::cout << "Test 3 passed\n";
        std::cout << "\n--- Test 5: Custom Struct (MyStruct) ---\n";
        raw::vector<MyStruct> test5;
        test5.push_back(MyStruct(1, "one"));
        test5.push_back(MyStruct(2, "two"));
        std::cout << "Pushed MyStruct objects. get_size(): " << test5.get_size() << "\n";
        assert(test5.get_size() == 2);
        assert(test5[0].x == 1);
        assert(test5[0].s == "one");
        assert(test5[1].x == 2);
        assert(test5[1].s == "two");

        test5.resize(3);
        std::cout << "Resized to 3. get_size(): " << test5.get_size() << "\n";
        assert(test5.get_size() == 3);
        assert(test5[0].x == 1);
        assert(test5[0].s == "one");
        assert(test5[1].x == 2);
        assert(test5[1].s == "two");
        assert(test5[2].x == 0);
        assert(test5[2].s == "");
        std::cout << "Test 5 passed\n";

        std::cout << "\n--- Test 6: Exception Safety (Simplified) ---\n";
        try {
            raw::vector<std::string> exceptionTest;
            exceptionTest.push_back("hello");
            exceptionTest.push_back("world");
            std::cout << "Exception test: push_back successful\n";
        }
        catch (...) {
            std::cerr << "Exception caught in exception safety test." << std::endl;
            std::cout << "Exception test: exception caught\n";
        }
        std::cout << "Exception test: completed\n";
    }
    {
        std::cout << "\n--- Test 7: Mixed Operations and Shrink to Fit (string) ---\n";
        raw::vector<std::string> test7;
        for (int i = 0; i < 5; ++i) {
            test7.push_back("str" + std::to_string(i));
        }
        std::cout << "Pushed 5 strings. get_size(): " << test7.get_size() << "\n";
        assert(test7.get_size() == 5);

        test7.reserve(10);
        std::cout << "Reserved 10. get_size(): " << test7.get_size() << "\n";
        assert(test7.get_size() == 5);

        test7.resize(3);
        std::cout << "Resized to 3. get_size(): " << test7.get_size() << "\n";
        assert(test7.get_size() == 3);

        test7.insert(2, "inserted");
        std::cout << "Inserted \"inserted\" at 2. get_size(): " << test7.get_size() << "\n";
        assert(test7.get_size() == 4);
        assert(test7[2] == "inserted");

        test7.erase(0);
        std::cout << "Erased element at 0. get_size(): " << test7.get_size() << "\n";
        assert(test7.get_size() == 3);

        test7.shrink_to_fit();
        std::cout << "Shrink to fit. get_size(): " << test7.get_size() << "\n";
        assert(test7.get_size() == 3);

        std::cout << "Test 7 passed\n";
    }

    {
        std::cout << "\n--- Test 4: String Tests (string) ---\n";
        raw::vector<std::string> test4;
        test4.push_back("hello");
        test4.push_back("world");
        test4.insert(0, "beautiful");
        printVector < std::string >(test4);
        std::cout << "Pushed and inserted strings. get_size(): " << test4.get_size() << "\n";
        assert(test4.get_size() == 3);
        assert(test4[0] == "beautiful");
        assert(test4[1] == "hello");
        assert(test4[2] == "world");

        test4.erase(1);
        printVector < std::string >(test4);
        std::cout << "Erased string. get_size(): " << test4.get_size() << "\n";
        assert(test4.get_size() == 2);
        assert(test4[0] == "beautiful");
        assert(test4[1] == "world");
        std::cout << "Test 4 passed\n";
    }

    {
        // -------------------- Test 8: Stress Test - Random Operations (string) --------------------
        std::cout << "\n--- Test 8: Stress Test - Random Operations (string) ---\n";
        for (int iter = 0; iter < 1000; ++iter) { // Reduced iterations for string tests
            std::cout << "Iteration " << iter + 1 << "\n";
            raw::vector<std::string> test8;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, 9); // Reduced range for string length
            std::uniform_int_distribution<> sizeDist(0, 10); // Reduced range for resize/reserve
            std::uniform_int_distribution<> indexDist(0, 9); // Reduced range for insert/erase

            for (int i = 0; i < 100; ++i) { // Reduced iterations for string tests
                size_t res = 0;
                std::cout << "vector before change: ";
                printVector<std::string>(test8);
                int operation = dist(gen) % 5;
                switch (operation) {
                case 0: // push_back
                    std::cout << "vector after operation push_back: ";
                    test8.push_back(std::string(dist(gen), 'a'));
                    std::cout << "push_back: ";
                    break;
                case 1: // insert
                    if (test8.get_size() > 0) {
                        res = indexDist(gen) % test8.get_size();
                        std::cout << "vector after operation insert: " << res << " ";
                        test8.insert(res, std::string(dist(gen), 'b'));
                        std::cout << "insert: ";
                    }
                    break;
                case 2: // erase
                    if (test8.get_size() > 0) {
                        res = indexDist(gen) % test8.get_size();
                        std::cout << "vector after operation erase: " << res << " ";
                        test8.erase(res);
                        std::cout << "erase: ";
                    }
                    break;
                case 3: // resize
                    res = sizeDist(gen);
                    std::cout << "vector after operation resize: " << res << " ";
                    test8.resize(res);
                    std::cout << "resize: ";
                    break;
                case 4: // reserve
                    res = sizeDist(gen);
                    std::cout << "vector after operation reserve: " << res << " ";
                    test8.reserve(res);
                    std::cout << "reserve: ";
                    break;
                }
                printVector<std::string>(test8);
                std::cout << "Iteration " << i << ", get_size(): " << test8.get_size() << ", get_capacity(): " << test8.get_capacity() << "\n";
            }
            std::cout << "Final vector contents (Test 8):\n";
            printVector<std::string>(test8);
            std::cout << "Test 8 passed (or at least didn't crash!)\n";
        }
        std::cout << "HARDCORE tests completed!\n";
        return 0;
    }
    _CrtDumpMemoryLeaks();
}