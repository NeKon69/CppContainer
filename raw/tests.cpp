#include "vector_alias.hpp" // Assuming your vector_base header file is named raw_vector.h
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
	raw::vector<std::string> n(10);
	n.push_back("Hello");
	n.push_back("World");
	n.push_back("!");
	auto it = n.begin();
}