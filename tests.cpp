#include "RawBase.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::cout << "Starting vector<int> HARD tests..." << std::endl;

    // Test 26: Massive push_back test
    {
        vector<int> vec;
        size_t initial_capacity = vec.get_capacity();
        size_t num_elements = 100000;
        for (size_t i = 0; i < num_elements; ++i) {
            vec.push_back(i);
        }
        assert(vec.get_size() == num_elements);
        assert(vec.get_capacity() >= num_elements);
        for (size_t i = 0; i < num_elements; ++i) {
            assert(vec[i] == i);
        }
        std::cout << "Test 26: Massive push_back - PASSED" << std::endl;
    }

    // Test 27: Interleaved inserts and erases - middle
    {
        vector<int> vec;
        for (int i = 0; i < 10; ++i) vec.push_back(i);
        for (int i = 0; i < 5; ++i) {
            vec.insert(vec.begin() + 5, 100 + i);
            vec.erase(vec.begin() + 6); // Erase element just after insertion
        }
        assert(vec.get_size() == 10);
        for (int i = 0; i < 5; ++i) assert(vec[i] == i);
        for (int i = 5; i < 10; ++i) assert(vec[i] == i); // Original elements should remain
        std::cout << "Test 27: Interleaved inserts/erases - middle - PASSED" << std::endl;
    }

    // Test 28: Interleaved inserts and erases - front
    {
        vector<int> vec;
        for (int i = 0; i < 10; ++i) vec.push_back(i);
        for (int i = 0; i < 5; ++i) {
            vec.insert(vec.begin(), 100 + i);
            vec.erase(vec.begin() + 1); // Erase element just after insertion
        }
        assert(vec.get_size() == 10);
        for (int i = 0; i < 5; ++i) assert(vec[i] == 104 - i); // Inserted elements (reversed order)
        for (int i = 5; i < 10; ++i) assert(vec[i] == i + 5); // Shifted original elements
        std::cout << "Test 28: Interleaved inserts/erases - front - PASSED" << std::endl;
    }

    // Test 29: Interleaved inserts and erases - back
    {
        vector<int> vec;
        for (int i = 0; i < 10; ++i) vec.push_back(i);
        for (int i = 0; i < 5; ++i) {
            vec.insert(vec.end(), 100 + i);
            vec.erase(vec.end() - 1); // Erase the last inserted element
        }
        assert(vec.get_size() == 10);
        for (int i = 0; i < 10; ++i) assert(vec[i] == i); // Original elements should remain
        std::cout << "Test 29: Interleaved inserts/erases - back - PASSED" << std::endl;
    }

    // Test 30: Insert at begin and end repeatedly
    {
        vector<int> vec;
        for (int i = 0; i < 1000; ++i) {
            if (i % 2 == 0) {
                vec.insert(vec.begin(), i);
            }
            else {
                vec.insert(vec.end(), i);
            }
        }
        assert(vec.get_size() == 1000);
        for (int i = 0; i < 500; ++i) {
            assert(vec[i] == (998 - 2 * i)); // Even numbers from 998 down to 0
        }
        for (int i = 0; i < 500; ++i) {
            assert(vec[500 + i] == (2 * i + 1)); // Odd numbers from 1 up to 999
        }
        std::cout << "Test 30: Insert at begin/end repeatedly - PASSED" << std::endl;
    }

    // Test 31: Erase from begin and end repeatedly
    {
        vector<int> vec;
        for (int i = 0; i < 1000; ++i) vec.push_back(i);
        while (vec.get_size() > 0) {
            if (vec.get_size() % 2 == 0 && vec.get_size() > 0) {
                vec.erase(vec.begin());
            }
            else if (vec.get_size() > 0) {
                vec.erase(vec.end() - 1);
            }
        }
        assert(vec.get_size() == 0);
        std::cout << "Test 31: Erase from begin/end repeatedly - PASSED" << std::endl;
    }

    // Test 32: Resize to very large and back to small
    {
        vector<int> vec(10);
        size_t large_size = 1000000;
        vec.resize(large_size);
        assert(vec.get_size() == large_size);
        assert(vec.get_capacity() >= large_size);
        vec.resize(10);
        assert(vec.get_size() == 10);
        std::cout << "Test 32: Resize large then small - PASSED" << std::endl;
    }

    // Test 33: Reserve very large and shrink to fit
    {
        vector<int> vec;
        size_t large_reserve = 1000000;
        vec.reserve(large_reserve);
        assert(vec.get_capacity() >= large_reserve);
        vec.shrink_to_fit();
        assert(vec.get_capacity() <= 1); // Should shrink to default capacity for empty vector
        std::cout << "Test 33: Reserve large and shrink - PASSED" << std::endl;
    }

    // Test 34: Copy and modify large vector
    {
        vector<int> vec1(10000);
        for (size_t i = 0; i < 10000; ++i) vec1[i] = i;
        vector<int> vec2 = vec1;
        for (size_t i = 0; i < 10000; ++i) vec2[i] *= 2;
        for (size_t i = 0; i < 10000; ++i) assert(vec1[i] == i); // Original should not be modified
        for (size_t i = 0; i < 10000; ++i) assert(vec2[i] == i * 2); // Copy should be modified
        std::cout << "Test 34: Copy and modify large - PASSED" << std::endl;
    }

    // Test 35: Move and modify large vector
    {
        vector<int> vec1(10000);
        for (size_t i = 0; i < 10000; ++i) vec1[i] = i;
        vector<int> vec2 = std::move(vec1);
        for (size_t i = 0; i < 10000; ++i) vec2[i] *= 2;
        assert(vec1.get_size() == 0); // Original should be empty after move
        for (size_t i = 0; i < 10000; ++i) assert(vec2[i] == i * 2); // Moved vector should be modified
        std::cout << "Test 35: Move and modify large - PASSED" << std::endl;
    }

    // Test 36: Iterator invalidation after insert - before insert
    {
        vector<int> vec;
        for (int i = 0; i < 5; ++i) vec.push_back(i);
        vector<int>::Iterator it = vec.begin() + 2;
        int val_before_insert = *it;
        vec.insert(vec.begin(), 100); // Insert at the beginning
        vector<int>::Iterator it_after_insert = vec.begin() + 3; // Iterator pointing to the same logical position
        assert(*it_after_insert == val_before_insert); // Iterator after insert should point to the correct element (shifted)
        std::cout << "Test 36: Iterator invalidation - insert before - PASSED" << std::endl;
    }

    // Test 37: Iterator invalidation after erase - before erase
    {
        vector<int> vec;
        for (int i = 0; i < 5; ++i) vec.push_back(i);
        vector<int>::Iterator it = vec.begin() + 2;
        vector<int>::Iterator next_it = it + 1;
        vec.erase(vec.begin()); // Erase the first element
        vector<int>::Iterator it_after_erase = vec.begin() + 1; // Iterator pointing to the logically same position
        vector<int>::Iterator next_it_after_erase = it_after_erase + 1;
        assert(*it_after_erase == *it); // Iterator after erase should point to the correct element (shifted)
        assert(*next_it_after_erase == *next_it);
        std::cout << "Test 37: Iterator invalidation - erase before - PASSED" << std::endl;
    }

    // Test 38: Const iterator operations
    {
        vector<int> vec;
        for (int i = 0; i < 5; ++i) vec.push_back(i);
        const vector<int> const_vec = vec;
        vector<int>::const_iterator cit = const_vec.cbegin();
        for (int i = 0; cit != const_vec.cend(); ++cit, ++i) {
            assert(*cit == i);
        }
        vector<int>::const_iterator cit2 = cit - 3;
        assert(*cit2 == 2);
        vector<int>::const_iterator cit3 = const_vec.cbegin() + 4;
        assert(*cit3 == 4);
        assert(cit3 - const_vec.cbegin() == 4);
        std::cout << "Test 38: Const iterator operations - PASSED" << std::endl;
    }

    // Test 39: Insert at the end many times, check capacity growth
    {
        vector<int> vec;
        size_t capacity_history[20];
        for (int i = 0; i < 20; ++i) {
            vec.push_back(i);
            capacity_history[i] = vec.get_capacity();
        }
        for (int i = 1; i < 20; ++i) {
            assert(capacity_history[i] >= capacity_history[i - 1]); // Capacity should not decrease
        }
        std::cout << "Test 39: Capacity growth on push_back - PASSED" << std::endl;
    }

    // Test 40: Self-assignment copy operator
    {
        vector<int> vec(5);
        for (int i = 0; i < 5; ++i) vec[i] = i;
        vector<int>& self_ref = vec;
        self_ref = vec; // Self-assignment
        assert(vec.get_size() == 5);
        for (int i = 0; i < 5; ++i) assert(vec[i] == i);
        std::cout << "Test 40: Self-assignment copy - PASSED" << std::endl;
    }

    // Test 41: Self-assignment move operator
    {
        vector<int> vec(5);
        for (int i = 0; i < 5; ++i) vec[i] = i;
        vector<int>& self_ref = vec;
        self_ref = std::move(vec); // Self-move-assignment - should still be valid (though maybe moved-from state)
        assert(self_ref.get_size() == 5); // Or size could be 0, depending on move impl, but shouldn't crash
        // Cannot reliably assert elements after self-move, implementation defined for moved-from state
        std::cout << "Test 41: Self-assignment move - PASSED" << std::endl;
    }

    // Test 42: Copy constructor from empty vector
    {
        vector<int> vec1;
        vector<int> vec2 = vec1;
        assert(vec2.get_size() == 0);
        assert(vec2.get_capacity() >= 1); // Or default capacity
        std::cout << "Test 42: Copy from empty - PASSED" << std::endl;
    }

    // Test 43: Move constructor from empty vector
    {
        vector<int> vec1;
        vector<int> vec2 = std::move(vec1);
        assert(vec2.get_size() == 0);
        assert(vec2.get_capacity() >= 1); // Or default capacity
        assert(vec1.get_size() == 0); // Moved from should be empty
        std::cout << "Test 43: Move from empty - PASSED" << std::endl;
    }

    // Test 44: Insert at iterator returned by insert (chaining)
    {
        vector<int> vec;
        vector<int>::Iterator it = vec.begin();
        it = vec.insert(it, 1);
        it = vec.insert(it + 1, 2); // Insert after the first inserted element
        it = vec.insert(it + 2, 3); // Insert after the second inserted element
        assert(vec.get_size() == 3);
        assert(vec[0] == 1);
        assert(vec[1] == 2);
        assert(vec[2] == 3);
        std::cout << "Test 44: Insert chaining - PASSED" << std::endl;
    }

    // Test 45: Copy vector with different capacities and sizes
    {
        vector<int> vec1(5);
        vec1.reserve(10);
        for (int i = 0; i < 5; ++i) vec1[i] = i;

        vector<int> vec2 = vec1;
        assert(vec2.get_size() == 5);
        assert(vec2.get_capacity() >= 5); // Capacity can be equal or larger than size

        for (int i = 0; i < 5; ++i) {
            assert(vec2[i] == i);
        }
        std::cout << "Test 45: Copy different size/capacity - PASSED" << std::endl;
    }

    // Test 46: Move vector with different capacities and sizes
    {
        vector<int> vec1(5);
        vec1.reserve(10);
        for (int i = 0; i < 5; ++i) vec1[i] = i;

        vector<int> vec2 = std::move(vec1);
        assert(vec2.get_size() == 5);
        assert(vec2.get_capacity() == 10); // Capacity should be moved

        for (int i = 0; i < 5; ++i) {
            assert(vec2[i] == i);
        }
        assert(vec1.get_size() == 0); // Source vector should be empty
        assert(vec1.get_capacity() == 0); // Or default, check move implementation
        std::cout << "Test 46: Move different size/capacity - PASSED" << std::endl;
    }

    std::cout << "All vector<int> HARD tests PASSED!" << std::endl;
    return 0;
}