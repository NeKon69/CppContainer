#pragma once
#include <cstdlib>
#include <iostream>
#include <cstring>

template<typename T>
class RawVector {
private:
	T* data = nullptr;
	size_t size = 0;
	size_t capacity = 1;
	T* normalize_capacity() {
		while (capacity <= size) {
			capacity *= 2;
		}
		T* newData = (T*)malloc(sizeof(T) * capacity);
		if (!newData) {
			throw std::bad_alloc();
		}
		if (data) {
			std::memcpy(newData, data, size * sizeof(T));
		}
		return newData;
	}
public:
	RawVector() {
		if (data)
			free(data);
		data = (T*)malloc(sizeof(T));
	};
    RawVector(size_t size_) : size(size_), capacity(1), data(nullptr) {
        try {
			auto newData = normalize_capacity();
			free(data);
			data = newData;
        }
        catch (std::bad_alloc& e) {
            std::cerr << e.what() << std::endl;
            free(data);
            throw;
        }
    };
    RawVector(const RawVector& other) : size(other.size), capacity(other.capacity) {
        try {
            T* newData = (T*)calloc(capacity, sizeof(T));
            if (!newData) {
                throw std::bad_alloc();
            }
            std::memcpy(newData, other.data, size * sizeof(T));
            free(data);
            data = newData;
        }
        catch (std::bad_alloc& e) {
            std::cerr << e.what() << std::endl;
            throw;
        }
    };
	RawVector& operator=(const RawVector& other) {
		if (this == &other) {
			return *this;
		}
		try {
			T* newData = (T*)calloc(other.capacity, sizeof(T));
			if (!newData) {
				throw std::bad_alloc();
			}
			std::memcpy(newData, other.data, other.size * sizeof(T));
			free(data);
			data = newData;
			size = other.size;
			capacity = other.capacity;
		}
		catch (std::bad_alloc& e) {
			std::cerr << e.what() << std::endl;
			throw;
		}
		return *this;
	};
	RawVector& operator=(const RawVector&& other) noexcept{
		if (this == &other) {
			return *this;
		}
		try {
			T* newData = (T*)calloc(other.capacity, sizeof(T));
			if (!newData) {
				throw std::bad_alloc();
			}
			std::memcpy(newData, other.data, other.size * sizeof(T));
			free(data);
			data = newData;
			size = other.size;
			capacity = other.capacity;
		}
		catch (std::bad_alloc& e) {
			std::cerr << e.what() << std::endl;
			if (data)
				free(data);
			throw;
		}
		return *this;
	}
	void push_back(const T& elem) {
		try {
			if (capacity <= size) {
				auto newData = normalize_capacity();
				std::memcpy(newData, data, size * sizeof(T));
				if (data != nullptr && newData != data) {
					free(data);
				}
				data = newData;
			}
			data[size] = elem;
			++size;
		}
		catch (...) {
			std::cerr << "Something happened in push_back" << std::endl;
			throw;
		}
	}
	T& operator [] (const size_t index) {
		if (index >= size) {
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	};

	const T& operator [] (const size_t index) const {
		if (index >= size) {
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	}

	size_t get_size() const {
		return size;
	};
	size_t get_capacity() const {
		return capacity;
	};

	void resize(size_t new_size) {
		T* new_data = (T*)realloc(data, new_size * sizeof(T));
		if (!new_data) {
			std::cerr << "Couldn't reserve that much space ERR" << std::endl;
			throw std::bad_alloc();
		}
		data = new_data;
		if (new_size > size)
			std::memset(data + size, 0, (new_size - size) * sizeof(T));
		size = new_size;
	}
	void reserve(size_t reserve_size) {
		if(reserve_size < capacity){
			return;
		}
		try {
			auto temp = (T*)realloc(data, reserve_size * sizeof(T));
			if (!temp) {
				std::cerr << "Couldn't reserve that much space ERR" << std::endl;
				free(data);
				throw std::bad_alloc();
			}
			data = temp;
		}
		catch (std::bad_alloc& err) {
			std::cerr << err.what() << std::endl;
			throw;
		}
		catch (...) {
			std::cerr << "What tf happened here? P.S -> (check reserve function)";
			throw;
		}
	}
	RawVector copy() {
		RawVector<T> new_vector(size);
		std::memcpy(new_vector.data, data, size * sizeof(T));
		return new_vector;
	}
	~RawVector() {
		free(data);
		std::cout << "Object Destroyed with size: " << size << " and with capacity: " << capacity << std::endl;
	};
};