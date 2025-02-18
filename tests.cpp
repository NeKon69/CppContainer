#include <iostream>
#include <cstdlib> // для malloc и free
#include <cstring> // для memcpy
#include "RawBase.h"

int main() {
	RawVector<size_t> v;
	v.reserve(100);
	v.resize(10);
	for (size_t i = 0; i < 10; ++i)
		std::cout << v[i] << " ";
	std::cout << std::endl;
}