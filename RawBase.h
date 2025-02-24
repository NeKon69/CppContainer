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
		while (size >= capacity) capacity *= 2;
		T* newData = (T*)malloc(sizeof(T) * capacity);
		if (!newData) {
			std::cout << "BAD ALLOC!!!!!!!\n";
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
			data = newData;
			std::memset(data, 0, size_ * sizeof(T));
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
	}

	RawVector(RawVector&& other) noexcept
		: data(other.data), size(other.size), capacity(other.capacity) {

		other.data = nullptr;
		other.size = 0;
		other.capacity = 0;
	}

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
	}
	RawVector& operator=(RawVector&& other) noexcept {
		if (this != &other) {
			free(data);
			data = other.data;
			size = other.size;
			capacity = other.capacity;
			other.data = nullptr;
			other.size = 0;
			other.capacity = 0;
		}
		return *this;
	}
	void push_back(const T& elem) {
		if (!data)
			data = (T*)malloc(sizeof(T));
		try {
			if (capacity <= size) {
				capacity *= 2;
				auto newdata = (T*)std::realloc(data, capacity * sizeof(T));
				if (!newdata) {
					std::cerr << "Couldn't reserve that much space ERR" << std::endl;
					free(data);
					throw std::bad_alloc();
				}
				data = newdata;
			}
			data[size] = elem;
			++size;
		}
		catch (...) {
			std::cerr << "Something happened in push_back" << std::endl;
			throw;
		}
	}

	void push_back(T&& elem) {
		if (!data)
			data = (T*)malloc(sizeof(T));
		try {
			if (capacity <= size) {
				auto newData = normalize_capacity();
				if (data) std::memcpy(newData, data, size * sizeof(T));
				if (data != nullptr && newData != data) {
					std::cout << data << std::endl;
					free(data);
					data = nullptr;
				}
				data = newData;
			}
			if (data) {
				data[size] = std::move(elem);
				++size;
			}
			else {
				throw std::bad_alloc();
			}
		}
		catch (...) {
			std::cerr << "Something happened in push_back" << std::endl;
			throw;
		}
	}
	T& at(const size_t index) {
		if (index >= size || index < 0) {
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	}

	const T& at(const size_t index) const {
		if (index >= size || index < 0) {
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	}

	T& operator [] (const size_t index) {
		return data[index];
	}

	const T& operator [] (const size_t index) const {
		return data[index];
	}

	size_t get_size() const {
		return size;
	}
	size_t get_capacity() const {
		return capacity;
	}

	void resize(size_t new_size) {
		if (new_size < size) {
			size = new_size;
			return;
		}
		T* new_data = (T*)realloc(data, new_size * sizeof(T));
		if (!new_data) {
			std::cerr << "Couldn't reserve that much space ERR" << std::endl;
			throw std::bad_alloc();
		}
		data = new_data;
		std::cout << size << std::endl;
		if (new_size > size) { std::memset(data + size, 0, (new_size - size) * sizeof(T)); }
		size = new_size;
		auto newdata = normalize_capacity();
		free(data);
		data = newdata;
	}

	void reserve(size_t reserve_size) {
		if (reserve_size < capacity) {
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
			capacity = reserve_size;
		}
		catch (std::bad_alloc& err) {
			std::cerr << err.what() << std::endl;
			throw;
		}
		catch (...) {
			std::cerr << "What tf happened here? P.S. -> (check reserve function)";
			throw;
		}
	}

	void clear() {
		if (data) {
			free(data);
			data = nullptr;
		}
		size = 0;
		capacity = 1;
		data = (T*)malloc(sizeof(T));
	}

	void shrink_to_fit() {
		if (size != capacity && size != 0) {
			try {
				auto shrinked = (T*)realloc(data, size * sizeof(T));
				if (!shrinked) {
					free(data);
					data = nullptr;
					throw std::bad_alloc();
				}
				data = shrinked;
				capacity = size;
			}
			catch (std::bad_alloc& err) {
				std::cerr << err.what() << std::endl;
				throw;
			}
		}
		if (size == 0) { free(data); data = nullptr; }
	}

	void pop_back() {
		if (size == 0) {
			throw std::out_of_range("Vector is empty");
		}
		if (--size == 0) { free(data); data = nullptr; }
	}

	void insert(size_t index, const T& value) {
		if (index > size) {
			throw std::out_of_range("Index out of range");
		}
		if (size == capacity) {
			auto newData = normalize_capacity();
			std::memcpy(newData, data, size * sizeof(T));
			free(data);
			data = newData;
		}
		std::memmove(data + index + 1, data + index, (size - index) * sizeof(T));
		data[index] = value;
		++size;
	}

	void insert(size_t index, T&& value) {
		if (index > size || index < 0) {
			throw std::out_of_range("Index out of range");
		}
		if (size == capacity) {
			auto newData = normalize_capacity();
			std::memcpy(newData, data, size * sizeof(T));
			free(data);
			data = newData;
		}
		std::memmove(data + index + 1, data + index, (size - index) * sizeof(T));
		data[index] = std::move(value);
		++size;
	}

	void erase(size_t index) {
		if (index >= size || index < 0) {
			throw std::out_of_range("Index out of range");
		}
		std::memmove(data + index, data + index + 1, (size - index - 1) * sizeof(T));
		if (--size == 0) { free(data); data = nullptr; }
	}

	void swap(RawVector& other) noexcept {
		std::swap(data, other.data);
		std::swap(size, other.size);
		std::swap(capacity, other.capacity);
	}

	class Iterator {
	private:
		T* ptr;
	public:
		Iterator(T* ptr_) : ptr(ptr_) {};
		bool operator ==(Iterator& Iteratorr) { return ptr == Iteratorr.ptr; }
		bool operator !=(Iterator& Iteratorr) { return ptr != Iteratorr.ptr; }
		T& operator *() const { return *ptr; }
		T* operator ->() const { return ptr; }

		Iterator& operator +=(size_t n) { ptr += n; return *this; }
		Iterator& operator -=(size_t n) { ptr -= n; return *this; }
		Iterator operator +(size_t n) const { return Iterator(ptr + n); }
		Iterator operator -(size_t n) const { return Iterator(ptr - n); }

		Iterator& operator ++() { ++ptr; return *this; }
		Iterator operator ++(int) { Iterator tmp = *this; ++ptr; return tmp; }

		Iterator& operator --() { --ptr; return *this; }
		Iterator operator --(int) { Iterator tmp = *this; --ptr; return tmp; }

		bool operator==(const Iterator& other) const { return ptr == other.ptr; }
		bool operator!=(const Iterator& other) const { return ptr != other.ptr; }
		bool operator< (const Iterator& other) const { return ptr < other.ptr; }
		bool operator> (const Iterator& other) const { return ptr > other.ptr; }
		bool operator<=(const Iterator& other) const { return ptr <= other.ptr; }
		bool operator>=(const Iterator& other) const { return ptr >= other.ptr; }

	};

	Iterator begin() { return Iterator(data); }
	Iterator end() { return Iterator(data + size); }
	Iterator rbegin() { return size > 0 ? Iterator(data + size - 1) : Iterator(data); }
	Iterator rend() { return Iterator(data - 1); }

	Iterator data_get() { return begin(); }

	T& front() { return *begin(); }
	T& back() { return *rbegin(); }


	class ConstIterator {
	private:
		const T* ptr;
	public:
		ConstIterator(const T* ptr_) : ptr(ptr_) {}
		const T& operator*() const { return *ptr; }
		const T* operator->() const { return ptr; }

		ConstIterator& operator+=(size_t n) { ptr += n; return *this; }
		ConstIterator& operator-=(size_t n) { ptr -= n; return *this; }
		ConstIterator operator+(size_t n) const { return ConstIterator(ptr + n); }
		ConstIterator operator-(size_t n) const { return ConstIterator(ptr - n); }

		ConstIterator& operator++() { ++ptr; return *this; }
		ConstIterator operator++(int) { ConstIterator tmp = *this; ++ptr; return tmp; }

		ConstIterator& operator--() { --ptr; return *this; }
		ConstIterator operator--(int) { ConstIterator tmp = *this; --ptr; return tmp; }

		bool operator<(const ConstIterator& other) const { return ptr < other.ptr; }
		bool operator>(const ConstIterator& other) const { return ptr > other.ptr; }
		bool operator<=(const ConstIterator& other) const { return ptr <= other.ptr; }
		bool operator>=(const ConstIterator& other) const { return ptr >= other.ptr; }
		bool operator==(const ConstIterator& other) const { return ptr == other.ptr; }
		bool operator!=(const ConstIterator& other) const { return ptr != other.ptr; }
	};

	using const_iterator = ConstIterator;
	const_iterator begin() const { return ConstIterator(data); }
	const_iterator end() const { return ConstIterator(data + size); }
	const_iterator cbegin() const { return const_iterator(data); }
	const_iterator cend() const { return const_iterator(data + size); }
	const_iterator crbegin() const { return size > 0 ? const_iterator(data + size - 1) : const_iterator(data); }
	const_iterator crend() const { return const_iterator(data - 1); }

	const Iterator data_get() const { return cbegin(); }

	const T& back() const { return *crbegin(); }
	const T& front() const { return *cbegin(); }

	RawVector copy() {
		RawVector<T> new_vector(size);
		new_vector.reserve(capacity);
		std::memcpy(new_vector.data, data, size * sizeof(T));
		return new_vector;
	}

	bool empty() { return size == 0; }

	~RawVector() {
		if (data) { std::cout << "Freeing memory at address: " << static_cast<void*>(data) << " | "; free(data); }
		std::cout << "Object Destroyed with size: " << size << " and with capacity: " << capacity << std::endl;
	}
};