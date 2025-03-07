---

## 📍 Overview

`raw::vector` is a custom vector class designed as a high-performance and user-friendly alternative to `std::vector`. It leverages template metaprogramming to automatically optimize its behavior based on the type of data it stores, delivering excellent performance for both trivial and non-trivial types.

Unlike `std::vector`, `raw::vector` provides more control over memory management and optimizations tailored to specific use cases, making it a strong choice for performance-critical applications.

---

## 👾 Features

- **Type-Based Optimization**: Automatically selects the most efficient implementation depending on whether the stored type is trivial or non-trivial.
- **Familiar Interface**: Offers an interface similar to `std::vector`, making it easy to integrate into existing codebases.
- **RAII-Based Memory Management**: Utilizes RAII principles to ensure proper memory allocation and deallocation, preventing memory leaks.
- **Exception Safety**: Designed with exception safety guarantees in mind.
- **Standard Iterator Support**: Provides iterators (`begin`, `end`, etc.) for seamless traversal.
- **Comprehensive Functionality**: Supports core vector operations including `push_back`, `at`, `operator[]`, `resize`, `reserve`, `clear`, `insert`, `erase`, `pop_back`, `shrink_to_fit`, and `swap`.
- **Custom Memory Control**: Unlike `std::vector`, allows finer control over allocation strategies, which can be beneficial in low-latency systems.
- **Reduced Overhead**: Avoids some of the internal complexities of `std::vector`, leading to potentially better cache locality and performance in specific scenarios.

---

## 🔥 Advantages Over `std::vector`

- **Optimized for Specific Use Cases**: Provides more efficient handling for trivial types, reducing unnecessary operations.
- **Better Performance for Large Data Structures**: Avoids some standard library overhead, making it suitable for high-performance applications.
- **More Transparent Memory Management**: Offers finer control over when and how memory is allocated and freed.
- **Custom Growth Strategies**: Allows alternative growth patterns instead of `std::vector`'s default exponential growth.
- **Minimal Dependencies**: Does not rely on the full STL, making it lightweight and easier to use in embedded or performance-sensitive environments.

However, `raw::vector` does not aim to fully replace `std::vector`. It is best suited for developers who need specific optimizations or control that `std::vector` does not provide.

---

## 🚀 Getting Started

### ☑️ Prerequisites

Before using `raw::vector`, ensure your runtime environment meets the following requirements:

- **Programming Language:** C++17 or newer
- **Compiler:** GCC, Clang, or MSVC with C++17 support

### ⚙️ Installation

1. Clone the `CppContainer` repository:

```sh
❯ git clone https://github.com/NeKon69/CppContainer
```

2. Navigate to the project directory:

```sh
❯ cd CppContainer
```

3. Include the necessary headers in your project:

```cpp
#include "RawNamespace/vector/RawVector.hpp"
```

### 🤖 Usage

```cpp
#include <iostream>
#include "RawNamespace/vector/RawVector.hpp"

int main() {
    raw::vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    for (int value : vec) {
        std::cout << value << " ";
    }
    return 0;
}
```

### 🧪 Testing

Run the test suite using the following command:

```sh
❯ make test
```

---

## 📌 Project Roadmap

- **All done!**

---

## 🔰 Contributing

- \*\*💬 \*\***[Join the Discussions](https://github.com/NeKon69/CppContainer/discussions)**
- \*\*🐛 \*\***[Report Issues](https://github.com/NeKon69/CppContainer/issues)**
- \*\*💡 \*\***[Submit Pull Requests](https://github.com/NeKon69/CppContainer/pulls)**

1. **Fork the Repository**
2. **Clone Locally**
   ```sh
   git clone https://github.com/NeKon69/CppContainer
   ```
3. **Create a New Branch**
   ```sh
   git checkout -b new-feature-x
   ```
4. **Make Your Changes & Commit**
   ```sh
   git commit -m 'Implemented new feature x.'
   ```
5. **Push & Submit a Pull Request**

---

## 🎗 License

This project is protected under the [MIT License](https://choosealicense.com/licenses/mit/). See the [LICENSE](https://github.com/NeKon69/CppContainer/blob/master/LICENSE) file for details.

---

## 🙌 Acknowledgments

- Inspired by `std::vector`
- Special thanks to contributors and the open-source community

