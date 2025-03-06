# raw::vector - A High-Performance, Type-Optimized Vector

`raw::vector` is a custom vector class designed to be a high-performance and user-friendly alternative to `std::vector`. It leverages template metaprogramming to automatically optimize its behavior based on the type of data it stores, delivering excellent performance for both trivial and non-trivial types.

## Features

- **Type-Based Optimization**: Automatically selects the most efficient implementation depending on whether the stored type is trivial or non-trivial.
- **Familiar Interface**: Offers an interface similar to `std::vector`, making it easy to integrate into existing codebases.
- **RAII-Based Memory Management**: Utilizes RAII principles to ensure proper memory allocation and deallocation, preventing memory leaks.
- **Exception Safety**: Designed with exception safety guarantees in mind.
- **Standard Iterator Support**: Provides iterators (begin, end, etc.) for seamless traversal.
- **Comprehensive Functionality**: Supports core vector operations including `push_back`, `at`, `operator[]`, `resize`, `reserve`, `clear`, `insert`, `erase`, `pop_back`, `shrink_to_fit`, and `swap`.

## Usage

### Include the Header

Include the main header file in your code:

```cpp
#include "RawNamespace/vector/vector_alias.hpp" // Adjust the path as needed
```

### Declare a raw::vector

Declare a vector by specifying the data type:

```cpp
raw::vector<int> myIntVector;
raw::vector<std::string> myStringVector;
```

### Use the Standard Interface

Use familiar methods to manipulate the vector:

```cpp
myIntVector.push_back(10);
myIntVector.push_back(20);
int value = myIntVector.at(0); // value will be 10

for (int i = 0; i < myIntVector.size(); ++i) {
    std::cout << myIntVector[i] << " "; // Output: 10 20
}

myStringVector.resize(2);
myStringVector[0] = "Hello";
myStringVector[1] = "World";
```

### Iterators

Use iterators for more advanced operations:

```cpp
for (auto it = myIntVector.begin(); it != myIntVector.end(); ++it) {
    std::cout << *it << " "; // Output: 10 20
}
```

## Performance Considerations

- **Trivial Types**: For types like `int`, `float`, or simple structs, `raw::vector` employs highly optimized memory copying techniques (e.g., `memcpy`), resulting in superior performance.
- **Non-Trivial Types**: For types like `std::string` or classes with custom constructors/destructors, `raw::vector` uses move semantics and explicit construction/destruction to ensure correctness and reasonable performance.

## Error Handling

- `at(index)`: Throws `std::out_of_range` if the index is out of bounds.
- Memory Allocation: Throws `std::bad_alloc` if memory allocation fails.

## Project Structure

- `RawBase.hpp`: Declares the `raw` namespace and potentially forward declarations for vector classes.
- `RawVector.hpp`: Defines `raw::vector_base`, the base class with a common interface and virtual methods for all vector implementations.
- `trivial_check.hpp`: Defines `raw::myis_trivial`, a struct using `<type_traits>` to determine if a type is trivial.
- `RawVectorTriv.hpp`: Implements `raw::vector_triv`, optimized for trivial types with efficient memory operations.
- `RawVectorNonTriv.hpp`: Implements `raw::vector_non_triv`, optimized for non-trivial types with move semantics and explicit object management.
- `vector_alias.hpp`: Defines the `raw::vector` alias, selecting between `vector_triv` and `vector_non_triv` based on type traits.

## Build Instructions

_To be added based on your project setup._

Example:

- Compile with a C++17-compatible compiler (e.g., `g++ -std=c++17`).
- Ensure the headers are in your include path (e.g., `-I./RawNamespace`).

## License

_To be specified._

Example: This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

_To be specified._

Example: Contributions are welcome! Please submit a pull request or open an issue on the project’s GitHub repository.

### Notes for Customization

- **Build Instructions**: Replace the placeholder with specific instructions for your build system (e.g., CMake, Make).
- **License**: Add your project’s license (e.g., MIT, Apache 2.0).
- **Contributing**: Include guidelines if you’re open to contributions.
- **Paths**: Adjust the include path in the usage examples to match your project’s directory structure.
- **Performance Details**: Optionally add benchmarks or comparisons to `std::vector` if available.