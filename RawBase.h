#pragma once
#include <cstdlib>
#include <iostream>
#include <cstring>

template<typename T>
class RawVector {
private:
	/// BEGINNING
	/// *****************************************************************************************************************************
	/// PRIVATE MEMBERS EXPLANATION
	/// data - the object that stores the capacity elements typenamed T (about capacity lower ↓)
	/// 
	///	size - is the element that allows the access to the amount of size elements, kind of hiding the actually used memory
	/// 
	/// capacity - is the amount of memory that is reserved for the data
	/// *****************************************************************************************************************************
	/// END

	T* data = nullptr;
	size_t size = 0;
	size_t capacity = 1;

	/// BEGINNING
	/// ********************************************************************************************************************************************************************
	/// PRIVATE MEMBERS EXPLANATION CONTINUED
	/// normalize_capacity - function, that if not enough space doing a reallocation with copying all of the elements from data, will be used a lot later
	/// 
	/// returns a pointer to new copied and reallocated data (P.S. IT DOESNT ACTUALLY REALLOC IT, JUST COPYING ALL OF THE ELEMENTS FROM CURR DATA, AND RESERVES NEW)
	/// ********************************************************************************************************************************************************************
	/// END

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
	/// BEGINNING
	/// *************************************************************************************************
	/// DEAFULT CONSTRUCTOR
	/// usual conctructor that is called than no arguments was gvien, reserves space for 1 element
	/// *************************************************************************************************
	/// END
	RawVector() : size(0), capacity(1), data(nullptr) {
		data = (T*)malloc(sizeof(T));
	};

	/// BEGINNING
	/// conctructor that is called when size is given, reserves space for size elements
	/// END

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

	/// BEGINNING
	/// ************************************************************************************************************************************************
	/// COPY CONSTRUCTOR OVERLOAD
	/// conctructor that is called when size and value are given, reserves space for size elements and fills them with value from previous vector
	/// 
	/// IMPORTANT - doesn't copy the elements that are outside the size, but actually reserved
	/// ************************************************************************************************************************************************
	/// END

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

	/// BEGINNING
	/// **********************************************************************************************************
	/// CONSTRUCTOR WITH MOVE SEMANTIC
	/// conctructor that is called when given an object of the class with move semantic
	/// 
	/// takes all of the data from it, as well as size and capacity, leaving it without data nor size/capacity
	/// **********************************************************************************************************
	/// END

	RawVector(RawVector&& other) noexcept
		: data(other.data), size(other.size), capacity(other.capacity) {

		other.data = nullptr;
		other.size = 0;
		other.capacity = 0;
	}

	/// BEGINNING
	/// ***********************************************************************************************
	/// COPY OPERATOR OVERLOAD
	/// uses this function than already a created object is given
	/// 
	/// data of the given object being copied to the curr object (WITHOUT ACTUALLY DESTROYING IT)
	/// ***********************************************************************************************
	/// END

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

	/// BEGINNING
	/// ****************************************************************************************************************
	/// MOVE_SEMANTIC OPERATOR OVERLOAD
	/// this function being called then the object is given with move semantic
	/// 
	/// same logic as in the CONSTRUCTOR with move semantic, just with freeing the data of the curr object firstly
	/// ****************************************************************************************************************
	/// END

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

	/// BEGINNING
	/// ***************************************************************************************************************************************
	/// PUSH_BACK FUNCTIONS
	/// next two functions are used to add elements to the vector
	/// 
	/// nothing special, just assigning to the data[size] element given, but if there is no enough memory additionally reserves it on-side
	/// 
	/// difference between these two, only with the fact that the second one is using moved object as a parameter
	/// ***************************************************************************************************************************************
	/// END

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

	/// BEGINNING
	/// *******************************************************************************************************
	/// INDEXING FUNCTIONS
	/// next 4 functions used to get element by index, with the check on the "at" ones, and without on []
	/// 
	/// const / non-const functions written
	/// *******************************************************************************************************
	/// END

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

	/// BEGINNING
	/// *********************************************************************
	/// GETTER FUNCTIONS
	/// next 2 functions return current data as it is, nothing fancy
	/// *********************************************************************
	/// END

	size_t get_size() const {
		return size;
	}
	size_t get_capacity() const {
		return capacity;
	}

	/// BEGINNING
	/// *********************************************************************************************************************************************
	/// RESIZE FUNCTION
	/// this one is the one that actually start top get a bit hard, first, if the size is smaller than the new size, it just changes the new size
	/// 
	/// if it isn't, it reallocates the memory with the new size, and if the new size is bigger than the old one, it fills the new memory with 0
	/// 
	/// also you should keep here an eye on memory managment, as it is a bit tricky
	/// *********************************************************************************************************************************************
	/// END

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

	/// BEGINNING
	/// *********************************************************************************************************************************************
	/// RESERVE FUNCTION
	/// this function does a reserve operation, same as in the vector, it just reserves more space, without actually revealing those elements
	/// *********************************************************************************************************************************************
	/// END

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

	/// BEGINNING
	/// **************************************************************************************
	/// CLEAR FUNCTION
	/// frees memory, leaving vector with no data, and only 1 reserved undefined element
	/// **************************************************************************************
	/// END

	void clear() {
		if (data) {
			free(data);
			data = nullptr;
		}
		size = 0;
		capacity = 1;
		data = (T*)malloc(sizeof(T));
	}

	/// BEGINNING
	/// ************************************************************************************************************************************************
	/// SHRINK_TO_FIT FUNCTION
	/// in my opinion kinda useless, especially if you call it a lot in for loop for example
	/// 
	/// but what it does, is just shrinking the memory to the size, leaving elements after size-element deleted, and not reserving memory for them
	/// ************************************************************************************************************************************************
	/// END

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

	/// BEGINNING
	/// ************************************************************************************************************
	/// POP_BACK FUNCTION
	/// this one works a bit different from what it looks like to user
	/// 
	/// instead of actually deleting it from the back, we just make size smaller, so this element is not visible
	/// ************************************************************************************************************
	/// END

	void pop_back() {
		if (size == 0) {
			throw std::out_of_range("Vector is empty");
		}
		if (--size == 0) { free(data); data = nullptr; }
	}

	/// BEGINNING
	/// **************************************************************************************************
	/// INSERT FUNCTION
	/// insert value on the given index, moving the elements via memmove function
	/// 
	/// there are 2 overloaded functions, that accept object, and that accept object with move semantic
	/// **************************************************************************************************
	/// TASKS: ADD ITERATORS ACCEPTING FUNCTIONS
	/// END

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

	/// BEGINNING
	/// **************************************************************************************************
	/// ERASE FUNCTIONS
	/// erases number on the index of the  vector, uses slide by 1 to move objects before given index
	/// **************************************************************************************************
	/// TASKS: ADD ITERATORS ACCEPTING FUNCTIONS
	/// END

	void erase(size_t index) {
		if (index >= size || index < 0) {
			throw std::out_of_range("Index out of range");
		}
		std::memmove(data + index, data + index + 1, (size - index - 1) * sizeof(T));
		if (--size == 0) { free(data); data = nullptr; }
	}

	/// BEGINNING
	/// *************************************************************
	/// SWAP FUNCTION
	/// just swaps the data, size and capacity of the two objects
	/// *************************************************************
	/// END


	void swap(RawVector& other) noexcept {
		std::swap(data, other.data);
		std::swap(size, other.size);
		std::swap(capacity, other.capacity);
	}

	/// BEGINNING
	/// ***************************************************************************************************************
	/// ITERATORS
	/// next goes two classes of itreators with the almost the same logic, but with the const and non-const pointer
	/// 
	/// added mostly used operators, like ++, --, *, ->, extc.
	/// ***************************************************************************************************************
	/// TASKS: ADD INHERITANCE FROM ITERATORBASE CLASS TO ITERATOR AND CONST_ITERATOR
	/// END

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

	/// BEGINNING
	/// ***************************************************************************************************************
	/// COPY FUNCTION
	/// idk why i created this, but it copies data from curr vector, and retuns it
	/// 
	/// NOTE: RETURNS COMPLETELY NEW DATA, NEW POINTER, AND IT ALSO DONT HAVE ANY ACOSIANTIONS WITH THE OLD VECTOR
	/// ***************************************************************************************************************
	/// END

	RawVector copy() {
		RawVector<T> new_vector(size);
		new_vector.reserve(capacity);
		std::memcpy(new_vector.data, data, size * sizeof(T));
		return new_vector;
	}

	/// CHECKS IF OBJECT IS EMPTY, IF SIZE VARIABLE ISN'T HANDELED GOOD ENOUGH, CAN CAUSE CRUSH
	bool empty() { return size == 0; }

	/// BEGINNING
	/// ***********************************************************************************************************************************
	/// DESTRUCTOR
	/// if data is already empty, or was freed before, can cause crush, so you should either not free it before, or make it a nullptr
	/// ***********************************************************************************************************************************
	/// END

	~RawVector() {
		if (data) { std::cout << "Freeing memory at address: " << static_cast<void*>(data) << " | "; free(data); }
		std::cout << "Object Destroyed with size: " << size << " and with capacity: " << capacity << std::endl;
	}
};