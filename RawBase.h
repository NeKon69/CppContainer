#pragma once
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <iterator>

/************************************************************************
 * typename created to handle different types, trivial and non-trivial
 * sets a value based on the copyability and destructability
 ***********************************************************************/
template<typename T>
struct myis_trivial {
	static const bool value = std::is_trivially_copyable<T>::value && std::is_trivially_destructible<T>::value;
};

template<typename T> class RawVectorTriv;
template<typename T> class RawVectorNonTriv;

template<typename T>
class RawVector {
private:
	static const bool is_trivial_v = myis_trivial<T>::value;
	T* data = nullptr;
	size_t size = 0;
	size_t capacity = 0;

	virtual T* normalize_capacity() = 0;

	friend class RawVectorTriv<T>;
	friend class RawVectorNonTriv<T>;
public:

	RawVector() : data(nullptr), size(0), capacity(1) {}
	virtual ~RawVector() {
		std::cout << "Freeing memory at address: " << static_cast<void*>(data) << " | ";
		if (data) { free(data); }
		std::cout << "RawVector Object Destroyed with size: " << size << " and with capacity: " << capacity << std::endl;
	}

	virtual RawVector* clone() const = 0;

	virtual void push_back(const T& elem) = 0;
	virtual void push_back(T&& elem) = 0;

	virtual T& at(const size_t index) = 0;
	virtual const T& at(const size_t index) const = 0;
	virtual T& operator [] (const size_t index) = 0;
	virtual const T& operator [] (const size_t index) const = 0;

	size_t get_size() const { return size; }
	size_t get_capacity() const { return capacity; }
	bool is_trivial() const { return is_trivial_v; }

	virtual void resize(size_t new_size) = 0;
	virtual void reserve(size_t reserve_size) = 0;
	virtual void clear() = 0;
	virtual void shrink_to_fit() = 0;
	virtual void pop_back() = 0;

	/*************************************************************************************************
	* ITERATORS
	*
	* Implements iterators using `using` to provide both const and non-const iterator types.
	*
	* Provides common iterator operators: ++, --, *, ->, etc.
	*
	* Tasks:
	*   - ADD ADDITIONAL CLASS FOR REVERSE ITERATORS
	*************************************************************************************************/

	template<typename iter>
	class IteratorBase {
	private:
		iter* ptr;
	public:

		IteratorBase(iter* ptr_) : ptr(ptr_) {};

		iter& operator *() const { return *ptr; }
		iter* operator ->() const { return ptr; }

		IteratorBase& operator +=(size_t n) { ptr += n; return *this; }
		IteratorBase& operator -=(size_t n) { ptr -= n; return *this; }

		IteratorBase operator +(size_t n) const { return IteratorBase(ptr + n); }

		IteratorBase operator -(size_t n) const { return IteratorBase(ptr - n); }
		size_t operator -(IteratorBase iter) const { return size_t(ptr - iter.ptr); }

		IteratorBase& operator ++() { ++ptr; return *this; }
		IteratorBase operator ++(int) { IteratorBase tmp = *this; ++ptr; return tmp; }

		IteratorBase& operator --() { --ptr; return *this; }
		IteratorBase operator --(int) { IteratorBase tmp = *this; ++ptr; return tmp; }

		bool operator==(const IteratorBase& other) const { return ptr == other.ptr; }
		bool operator!=(const IteratorBase& other) const { return ptr != other.ptr; }
		bool operator< (const IteratorBase& other) const { return ptr < other.ptr; }
		bool operator> (const IteratorBase& other) const { return ptr > other.ptr; }
		bool operator<=(const IteratorBase& other) const { return ptr <= other.ptr; }
		bool operator>=(const IteratorBase& other) const { return ptr >= other.ptr; }
	};

	using Iterator = IteratorBase<T>;
	using const_iterator = IteratorBase<const T>;

	Iterator begin() { return Iterator(data); }
	Iterator end() { return Iterator(data + size); }
	Iterator rbegin() { return size > 0 ? Iterator(data + size - 1) : Iterator(data); }
	Iterator rend() { return Iterator(data - 1); }

	Iterator data_get() { return begin(); }

	T& front() { return *begin(); }
	T& back() { return *rbegin(); }

	const_iterator begin() const { return const_iterator(data); }
	const_iterator end() const { return const_iterator(data + size); }
	const_iterator cbegin() const { return const_iterator(data); }
	const_iterator cend() const { return const_iterator(data + size); }
	const_iterator crbegin() const { return size > 0 ? const_iterator(data + size - 1) : const_iterator(data); }
	const_iterator crend() const { return const_iterator(data - 1); }

	const_iterator data_get() const { return cbegin(); }

	const T& back() const { return *crbegin(); }
	const T& front() const { return *cbegin(); }


	virtual void insert(size_t index, const T& value) = 0;
	virtual void insert(size_t index, T&& value) = 0;
	virtual Iterator insert(Iterator pos, const T& value) = 0;
	virtual Iterator insert(Iterator pos, T&& value) = 0;

	virtual void erase(size_t index) = 0;
	virtual Iterator erase(Iterator pos) = 0;

	virtual void swap(RawVector<T>& other) noexcept = 0;

};

template<typename T>
class RawVectorTriv : public RawVector<T> {
private:

	/*************************************************************************************************
	 * PRIVATE MEMBERS:
	 *
	 * `data`     - Stores allocated memory for `capacity` elements of type `T`.
	 * `size`     - Number of currently used elements.
	 * `capacity` - Total allocated memory size for `data`.
	 *************************************************************************************************/

	 /// Data members data, size, capacity ARE ALREADY inherited from RawVector, no need to declare them again here!

	 /*************************************************************************************************
	  * PRIVATE FUNCTION: `normalize_capacity()`
	  *
	  * Reallocates `data` with doubled `capacity` if needed (when `size` >= `capacity`).
	  * Copies existing data to new memory.
	  * Returns pointer to the new `data`.
	  *
	  * Throws: std::bad_alloc on allocation failure.
	  *************************************************************************************************/
	T* normalize_capacity() override {
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

protected:
	using RawVector<T>::data;
	using RawVector<T>::size;
	using RawVector<T>::capacity;

public:
	using Iterator = typename RawVector<T>::template IteratorBase<T>;
	using const_iterator = typename RawVector<T>::template IteratorBase<const T>;
	RawVectorTriv* clone() const override { return new RawVectorTriv(*this); }

	/*********************************************************************
	 * DEFAULT CONSTRUCTOR: `RawVectorTriv()`
	 *
	 * Initializes with capacity for 1 element.
	 *********************************************************************/
	RawVectorTriv() {
		data = (T*)malloc(sizeof(T));
	};

	/*********************************************************************
	 * SIZE CONSTRUCTOR: `RawVectorTriv(size_t size_)`
	 *
	 * Initializes with given `size_`.
	 * Reserves memory and initializes elements to 0.
	 *
	 * Throws: std::bad_alloc on allocation failure.
	 *********************************************************************/
	RawVectorTriv(size_t size_) {
		size = size_;
		capacity = 1;
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

	/**************************************************************************************
	 * COPY CONSTRUCTOR: `RawVectorTriv(const RawVectorTriv& other)`
	 *
	 * Deep copy from `other`.
	 * Copies size, capacity, and data.
	 *
	 * Throws: std::bad_alloc on allocation failure.
	 **************************************************************************************/
	RawVectorTriv(const RawVectorTriv& other) {
		size = other.size;
		capacity = other.capacity;
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

	/*************************************************************************************
	 * MOVE CONSTRUCTOR: `RawVectorTriv(RawVectorTriv&& other) noexcept`
	 *
	 * Move resources from `other`.
	 * Transfers data, size, and capacity. `other` becomes empty.
	 *************************************************************************************/
	RawVectorTriv(RawVectorTriv&& other) noexcept {
		data = other.data;
		size = other.size;
		capacity = other.capacity;

		other.data = nullptr;
		other.size = 0;
		other.capacity = 0;
	}

	/*************************************************************************************
	 * COPY ASSIGNMENT: `operator=(const RawVectorTriv& other)`
	 *
	 * Deep copy assignment from `other`.
	 * Frees current data, then copies size, capacity, and data from `other`.
	 *
	 * Throws: std::bad_alloc on allocation failure.
	 *************************************************************************************/
	RawVectorTriv& operator=(const RawVectorTriv& other) {
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

	/*************************************************************************************
	 * MOVE ASSIGNMENT: `operator=(RawVectorTriv&& other) noexcept`
	 *
	 * Move assignment from `other`.
	 * Frees current data, then moves data, size, and capacity from `other`.
	 * `other` becomes empty.
	 *************************************************************************************/
	RawVectorTriv& operator=(RawVectorTriv&& other) noexcept {
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

	/*************************************************************************************
	 * PUSH_BACK FUNCTIONS: `push_back(const T& elem)`, `push_back(T&& elem)`
	 *
	 * Appends element `elem` to the end.
	 * Reallocates memory if `capacity` is full (using `normalize_capacity`).
	 * Overloads for copy and move semantics.
	 *
	 * Throws: std::bad_alloc on allocation failure.
	 *************************************************************************************/
	void push_back(const T& elem) override {
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
			if (data)
				data[size] = elem;
			++size;
		}
		catch (...) {
			std::cerr << "Something happened in push_back" << std::endl;
			throw;
		}
	}

	void push_back(T&& elem) override {
		if (!data)
			data = (T*)malloc(sizeof(T));
		try {
			if (capacity <= size) {
				auto newData = normalize_capacity();
				if (data) std::memcpy(newData, data, size * sizeof(T));
				if (data != nullptr && newData != data) {
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

	/*********************************************************************
	 * INDEXING FUNCTIONS: `at()`, `operator[]` (const and non-const)
	 *
	 * Access elements by index.
	 * `at()` - with bounds checking, throws `std::out_of_range`.
	 * `operator[]` - no bounds checking.
	 *********************************************************************/
	T& at(const size_t index) override {
		if (index >= size || index < 0) {
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	}

	const T& at(const size_t index) const override {
		if (index >= size || index < 0) {
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	}

	T& operator [] (const size_t index) override {
		return data[index];
	}

	const T& operator [] (const size_t index) const override {
		return data[index];
	}

	/*********************************************************************
	 * GETTER FUNCTIONS: `get_size()`, `get_capacity()`
	 *
	 * Returns current `size` and `capacity`.
	 *********************************************************************/
	size_t get_size() const {
		return size;
	}
	size_t get_capacity() const {
		return capacity;
	}

	/*************************************************************************************
	 * RESIZE FUNCTION: `resize(size_t new_size)`
	 *
	 * Changes vector size to `new_size`.
	 * Reallocates if needed, initializes new elements to 0.
	 *
	 * Throws: std::bad_alloc on allocation failure.
	 *************************************************************************************/
	void resize(size_t new_size) override {
		if (new_size <= size) {
			size = new_size;
			return;
		}
		T* new_data = (T*)realloc(data, new_size * sizeof(T));
		if (!new_data) {
			std::cerr << "Couldn't reserve that much space ERR" << std::endl;
			throw std::bad_alloc();
		}
		data = new_data;
		if (new_size > size) { std::memset(data + size, 0, (new_size - size) * sizeof(T)); }
		size = new_size;
		capacity = std::max(capacity, size);
	}

	/*************************************************************************************
	 * RESERVE FUNCTION: `reserve(size_t reserve_size)`
	 *
	 * Reserves memory for at least `reserve_size` elements.
	 * Does not change `size`.
	 *
	 * Throws: std::bad_alloc on allocation failure.
	 *************************************************************************************/
	void reserve(size_t reserve_size) override {
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

	/*********************************************************************
	 * CLEAR FUNCTION: `clear()`
	 *
	 * Removes all elements, frees memory.
	 * Resets to initial state (capacity 1).
	 *********************************************************************/
	void clear() override {
		if (data) {
			free(data);
			data = nullptr;
		}
		size = 0;
		capacity = 1;
		data = (T*)malloc(sizeof(T));
	}

	/**************************************************************************************
	 * SHRINK_TO_FIT FUNCTION: `shrink_to_fit()`
	 *
	 * Reduces capacity to match `size`, minimizing memory.
	 * Can be inefficient if called frequently.
	 *
	 * Throws: std::bad_alloc on allocation failure.
	 **************************************************************************************/
	void shrink_to_fit() override {
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
		if (size == 0) { free(data); data = nullptr; capacity = 0; }
	}

	/*********************************************************************
	 * POP_BACK FUNCTION: `pop_back()`
	 *
	 * Removes the last element (decrements `size`).
	 * Does not actually deallocate memory.
	 *
	 * Throws: std::out_of_range if vector is empty.
	 *********************************************************************/
	void pop_back() override {
		if (size == 0) {
			throw std::out_of_range("Vector is empty");
		}
		if (--size == 0) { free(data); data = calloc(1, sizeof(T)); capacity = 1; }
	}

	/*************************************************************************************
	 * INSERT FUNCTIONS: `insert()` overloads
	 *
	 * Inserts element at given index/iterator.
	 * Shifts existing elements.
	 * Reallocates if needed.
	 * Overloads for copy and move semantics, and index/iterator position.
	 *
	 * Throws: std::out_of_range, std::bad_alloc.
	 *************************************************************************************/

	void insert(size_t index, const T& value) override {
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

	void insert(size_t index, T&& value) override {
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

	Iterator insert(Iterator pos, const T& value) override {
		size_t insert_index = pos - Iterator(data);

		if (insert_index > size) {
			throw std::out_of_range("Index out of range");
		}
		if (size == capacity) {
			auto newData = normalize_capacity();
			free(data);
			data = newData;
		}
		std::memmove(data + insert_index + 1, data + insert_index, (size - insert_index) * sizeof(T));
		data[insert_index] = value;
		++size;
		return Iterator(data + insert_index);
	}

	Iterator insert(Iterator pos, T&& value) override {
		size_t insert_index = pos - Iterator(data);

		if (insert_index > size) {
			throw std::out_of_range("Index out of range");
		}
		if (size == capacity) {
			auto newData = normalize_capacity();
			free(data);
			data = newData;
		}
		std::memmove(data + insert_index + 1, data + insert_index, (size - insert_index) * sizeof(T));
		data[insert_index] = std::move(value);
		++size;
		return Iterator(data + insert_index);
	}

	/*********************************************************************
	 * ERASE FUNCTIONS: `erase()` overloads
	 *
	 * Erases element at given index/iterator.
	 * Shifts subsequent elements.
	 * Overloads for index and iterator position.
	 *
	 * Throws: std::out_of_range.
	 *********************************************************************/

	void erase(size_t index) override {
		if (index >= size || index < 0) {
			throw std::out_of_range("Index out of range");
		}
		std::memmove(data + index, data + index + 1, (size - index - 1) * sizeof(T));
		if (--size == 0) { free(data); data = nullptr; }
	}

	Iterator erase(Iterator pos) override {
		size_t erase_index = pos - Iterator(data);
		if (erase_index >= size || erase_index < 0) {
			throw std::out_of_range("Index out of range");
		}
		std::memmove(data + erase_index, data + erase_index + 1, (size - erase_index - 1) * sizeof(T));
		if (--size == 0) { free(data); data = nullptr; }
		return Iterator(data + erase_index);
	}

	/************************************************************
	 * SWAP FUNCTION: `swap(RawVector<T>& other) noexcept`
	 *
	 * Swaps data, size, and capacity with `other`.
	 * Fast, no memory operations.
	 *************************************************************/
	void swap(RawVector<T>& other) noexcept override {
		std::swap(data, other.data);
		std::swap(size, other.size);
		std::swap(capacity, other.capacity);
	}

	/*************************************************************************************
	 * COPY FUNCTION: `copy()`
	 *
	 * Returns a deep copy of the vector.
	 * New vector with copied data and capacity.
	 *************************************************************************************/

	RawVectorTriv copy() {
		RawVectorTriv<T> new_vector(size);
		new_vector.reserve(capacity);
		std::memcpy(new_vector.data, data, size * sizeof(T));
		return new_vector;
	}

	/// CHECKS IF OBJECT IS EMPTY, IF SIZE VARIABLE ISN'T HANDELED GOOD ENOUGH, CAN CAUSE CRUSH
	bool empty() { return size == 0; }

	/*********************************************************************
	 * DESTRUCTOR: `~RawVectorTriv()`
	 *
	 * Prints destruction message (for debugging).
	 * Memory freeing handled in base class.
	 *********************************************************************/
	~RawVectorTriv() override {
		std::cout << "RawVectorTriv Object Destroyed next comes memory freeing" << std::endl;
	}
};


template<typename T>
class RawVectorNonTriv : public RawVector<T> {
private:

	/*************************************************************************************
	 * PRIVATE FUNCTION: `normalize_capacity()`
	 *
	 * Reallocates `data` with doubled capacity if needed.
	 * Constructs new elements in new memory using move semantics.
	 * Destroys old elements.
	 * Returns pointer to the new `data`.
	 *
	 * Throws: std::bad_alloc on allocation failure.
	 *************************************************************************************/

	T* normalize_capacity() {
		while (size >= capacity) capacity *= 2;

		void* raw = malloc(sizeof(T) * capacity);
		if (!raw) throw std::bad_alloc();

		T* new_data = static_cast<T*>(raw);

		for (size_t i = 0; i < size; ++i) {
			new (new_data + i) T(std::move(data[i]));
			data[i].~T();
		}

		for (size_t i = size; i < capacity; ++i)
			new (new_data + i) T();

		free(data);
		return new_data;
	}

	/*************************************************************************************
	 * PRIVATE FUNCTION: `normalize_capacity(size_t size_)`
	 *
	 * Reallocates `data` with doubled capacity to fit `size_`.
	 * Constructs new elements in new memory using move semantics.
	 * Destroys old elements.
	 * Returns pointer to the new `data`.
	 *
	 * Throws: std::bad_alloc on allocation failure.
	 *************************************************************************************/

	T* normalize_capacity(size_t size_) {
		while (size_ >= capacity) capacity *= 2;

		void* raw = malloc(sizeof(T) * capacity);
		if (!raw) throw std::bad_alloc();

		T* new_data = static_cast<T*>(raw);

		for (size_t i = 0; i < size; ++i) {
			new (new_data + i) T(std::move(data[i]));
			data[i].~T();
		}

		for (size_t i = size; i < capacity; ++i)
			new (new_data + i) T();

		free(data);
		return new_data;
	}

	/*************************************************************************************
	 * PRIVATE FUNCTION: `reserve_space()`
	 *
	 * Reserves memory for `capacity` elements.
	 * Constructs default-initialized elements in the reserved space.
	 * Returns pointer to the new `data`.
	 *
	 * Throws: std::bad_alloc on allocation failure.
	 *************************************************************************************/

	T* reserve_space() {
		while (size >= capacity) capacity *= 2;

		void* raw = malloc(sizeof(T) * capacity);
		if (!raw) throw std::bad_alloc();

		T* new_data = static_cast<T*>(raw);
		for (size_t i = 0; i < capacity; ++i)
			new (new_data + i) T();
		return new_data;
	}
protected:
	using RawVector<T>::data;
	using RawVector<T>::size;
	using RawVector<T>::capacity;

public:
	using Iterator = typename RawVector<T>::template IteratorBase<T>;
	using const_iterator = typename RawVector<T>::template IteratorBase<const T>;

	/*********************************************************************
	 * DEFAULT CONSTRUCTOR: `RawVectorNonTriv()`
	 *
	 * Initializes with capacity 1 and default-constructs one element.
	 *
	 * Throws: std::bad_alloc on allocation failure, exceptions from T's constructor.
	 *********************************************************************/

	RawVectorNonTriv() {
		size = 0;
		capacity = 1;
		void* raw_memory = malloc(sizeof(T));
		if (!raw_memory) throw std::bad_alloc();

		try {
			data = new (raw_memory) T();
		}
		catch (...) {
			free(raw_memory);
			throw;
		}
	}

	/*********************************************************************
	 * SIZE CONSTRUCTOR: `RawVectorNonTriv(size_t size_)`
	 *
	 * Initializes with given `size_`.
	 * Reserves memory and default-constructs `size_` elements.
	 *
	 * Throws: std::bad_alloc on allocation failure, exceptions from T's constructor.
	 *********************************************************************/

	RawVectorNonTriv(size_t size_) {
		size = size_;
		capacity = 1;
		try {
			data = reserve_space();
		}
		catch (const std::bad_alloc& e) {
			if (data) {
				for (size_t i = 0; i < capacity; ++i)
					data[i].~T();
				free(data);
			}
			std::cerr << e.what() << std::endl;
			throw;
		}
	}

	/**************************************************************************************
	 * COPY CONSTRUCTOR: `RawVectorNonTriv(const RawVectorNonTriv& other)`
	 *
	 * Deep copy from `other`.
	 * Copies size, capacity, and constructs new elements by copying from `other`.
	 *
	 * Throws: std::bad_alloc on allocation failure, exceptions from T's copy constructor.
	 **************************************************************************************/

	RawVectorNonTriv(const RawVectorNonTriv& other) {
		size = other.size;
		capacity = other.capacity;
		try {
			data = reserve_space();
			for (size_t i = 0; i < size; ++i)
				new (data + i) T(other.data[i]);
		}
		catch (const std::bad_alloc& e) {
			if (data) {
				for (size_t i = 0; i < capacity; ++i)
					data[i].~T();
				free(data);
			}
			std::cerr << e.what() << std::endl;
			throw;
		}
	}

	/*************************************************************************************
	 * MOVE CONSTRUCTOR: `RawVectorNonTriv(RawVectorNonTriv&& other) noexcept`
	 *
	 * Move resources from `other`.
	 * Transfers size, capacity, and move-constructs elements from `other`.
	 * `other` becomes empty.
	 *************************************************************************************/

	RawVectorNonTriv(RawVectorNonTriv&& other) noexcept {
		size = other.size;
		capacity = other.capacity;
		data = reserve_space();
		for (size_t i = 0; i < size; ++i)
			new (data + i) T(std::move(other.data[i]));
		other.size = 0;
		other.capacity = 0;
		other.data = nullptr;
	}

	/*************************************************************************************
	 * COPY ASSIGNMENT: `operator=(const RawVectorNonTriv& other)`
	 *
	 * Deep copy assignment from `other`.
	 * Frees current data, then copies size, capacity, and copy-constructs elements.
	 *
	 * Throws: std::bad_alloc on allocation failure, exceptions from T's copy constructor.
	 *************************************************************************************/

	RawVectorNonTriv& operator=(const RawVectorNonTriv& other) {
		if (data == other.data)
			return *this;
		size_t free_cap = capacity;
		size = other.size;
		capacity = other.capacity;
		try {
			for (size_t i = 0; i < free_cap; ++i)
				data[i].~T();
			free(data);
			data = (T*)malloc(sizeof(T) * capacity);
			for (size_t i = 0; i < other.size; ++i) {
				new (data + i) T(other.data[i]);
			}
			for (size_t i = other.size; i < capacity; ++i)
				new (data + i) T();
		}

		catch (const std::bad_alloc& e) {
			if (data) {
				for (size_t i = 0; i < free_cap; ++i)
					data[i].~T();
				free(data);
			}
			std::cerr << e.what() << std::endl;
			throw;
		}
		return *this;
	}

	/*************************************************************************************
	 * MOVE ASSIGNMENT: `operator=(RawVectorNonTriv&& other) noexcept`
	 *
	 * Move assignment from `other`.
	 * Frees current data, then moves size, capacity, and move-constructs elements.
	 * `other` becomes empty.
	 *************************************************************************************/

	RawVectorNonTriv& operator=(RawVectorNonTriv&& other) noexcept {
		if (data == other.data)
			return *this;
		size_t free_cap = capacity;
		size = other.size;
		capacity = other.capacity;
		for (size_t i = 0; i < free_cap; ++i)
			data[i].~T();
		free(data);
		data = (T*)malloc(sizeof(T) * capacity);
		for (size_t i = 0; i < other.size; ++i) {
			new (data + i) T(std::move(other.data[i]));
			other.data[i].~T();
		}

		for (size_t i = other.size; i < capacity; ++i) {
			other.data[i].~T();
			new (data + i) T();
		}

		free(other.data);
		other.size = 0;
		other.capacity = 0;
		other.data = nullptr;
		return *this;
	}

	/*********************************************************************
	 * PUSH_BACK FUNCTIONS: `push_back(const T& elem)`, `push_back(T&& elem)`
	 *
	 * Appends element `elem` to the end.
	 * Reallocates memory if needed (using `normalize_capacity`).
	 * Overloads for copy and move semantics.
	 *
	 * Throws: std::bad_alloc on allocation failure, exceptions from T's constructors.
	 *********************************************************************/

	void push_back(const T& elem) override {
		if (size >= capacity) {
			data = normalize_capacity();
		}
		data[size++] = elem;
	};
	void push_back(T&& elem) override {
		if (size >= capacity) {
			data = normalize_capacity();
		}
		data[size++] = std::move(elem);
	};

	/*********************************************************************
	 * INDEXING FUNCTIONS: `at()`, `operator[]` (const and non-const)
	 *
	 * Access elements by index.
	 * `at()` - with bounds checking, throws `std::out_of_range`.
	 * `operator[]` - no bounds checking.
	 *********************************************************************/

	T& at(const size_t index) override {
		if (index >= size) {
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	}
	const T& at(const size_t index) const override {
		if (index >= size) {
			throw std::out_of_range("Index out of range");
		}
		return data[index];
	};

	T& operator [] (const size_t index) override {
		return data[index];
	};
	const T& operator [] (const size_t index) const override {
		return data[index];
	};

	/*********************************************************************
	 * RESIZE FUNCTION: `resize(size_t new_size)`
	 *
	 * Changes vector size to `new_size`.
	 * Reallocates if needed, default-constructs new elements.
	 *
	 * Throws: std::bad_alloc on allocation failure, exceptions from T's constructor.
	 *********************************************************************/

	void resize(size_t new_size) override {
		if (new_size < size)
			size = new_size;
		else {
			if (new_size >= capacity) {
				data = normalize_capacity(new_size);
			}
			size = new_size;
		}
	}

	/*********************************************************************
	 * RESERVE FUNCTION: `reserve(size_t reserve_size)`
	 *
	 * Reserves memory for at least `reserve_size` elements.
	 * Does not change `size`.
	 * Default-constructs elements in reserved space.
	 *
	 * Throws: std::bad_alloc on allocation failure, exceptions from T's constructor.
	 *********************************************************************/

	void reserve(size_t reserve_size) override {
		if (reserve_size < capacity)
			return;
		capacity = reserve_size;
		data = normalize_capacity();
	}

	/*********************************************************************
	 * CLEAR FUNCTION: `clear()`
	 *
	 * Removes all elements, destroys them, and frees memory.
	 * Resets to initial state (capacity 1, one default-constructed element).
	 *
	 * Throws: std::bad_alloc on allocation failure, exceptions from T's constructor/destructor.
	 *********************************************************************/

	void clear() override {
		for (size_t i = 0; i < capacity; ++i)
			data[i].~T();
		free(data);
		size = 0;
		capacity = 1;
		void* raw_memory = malloc(sizeof(T));
		if (!raw_memory) throw std::bad_alloc();
		data = new (raw_memory) T();
	}

	/**************************************************************************************
	 * SHRINK_TO_FIT FUNCTION: `shrink_to_fit()`
	 *
	 * Reduces capacity to match `size`, minimizing memory.
	 * Destroys elements beyond `size`.
	 * Can be inefficient if called frequently.
	 *
	 * Throws: std::bad_alloc on allocation failure, exceptions from T's destructor.
	 **************************************************************************************/

	void shrink_to_fit() override {
		if (size == capacity)
			return;
		for (size_t i = size; i < capacity; ++i)
			data[i].~T();
		capacity = size;
	}

	/*********************************************************************
	 * POP_BACK FUNCTION: `pop_back()`
	 *
	 * Removes the last element (decrements `size`), destroying it if needed.
	 * Frees memory and resets if the vector becomes empty.
	 *
	 * Throws: std::out_of_range if vector is empty, exceptions from T's destructor.
	 *********************************************************************/

	void pop_back() override {
		if (size == 0)
			throw std::out_of_range("Index out of range");
		if (--size == 0) {
			for (size_t i = 0; i < capacity; ++i)
				data[i].~T();
			free(data);
			size = 0;
			capacity = 1;
			void* raw_memory = malloc(sizeof(T));
			if (!raw_memory) throw std::bad_alloc();
			data = new (raw_memory) T();
		}
	}

	/*********************************************************************
	 * INSERT FUNCTIONS: `insert()` overloads
	 *
	 * Inserts element at given index/iterator.
	 * Shifts existing elements using move semantics.
	 * Reallocates if needed.
	 * Overloads for copy and move semantics, and index/iterator position.
	 *
	 * Throws: std::out_of_range, std::bad_alloc, exceptions from T's constructors.
	 *********************************************************************/

	void insert(size_t index, const T& value) override {
		if (index > size) {
			throw std::out_of_range("Index out of range");
		}
		if (size == capacity) {
			data = normalize_capacity();
		}
		for (size_t i = size; i > index; --i) {
			data[i] = std::move(data[i - 1]);
		}
		data[index] = value;
		++size;
	}
	void insert(size_t index, T&& value) override {
		if (index > size) {
			throw std::out_of_range("Index out of range");
		}
		if (size == capacity) {
			data = normalize_capacity();
		}
		for (size_t i = size; i > index; --i) {
			data[i] = std::move(data[i - 1]);
		}
		data[index] = std::move(value);
		++size;
	}
	Iterator insert(Iterator pos, const T& value) override {
		size_t insert_index = pos - Iterator(data);
		if (insert_index > size) {
			throw std::out_of_range("Index out of range");
		}
		if (size == capacity) {
			data = normalize_capacity();
		}
		for (size_t i = size; i > insert_index; --i) {
			data[i] = std::move(data[i - 1]);
		}
		data[insert_index] = value;
		++size;
		return Iterator(data + insert_index);
	}
	Iterator insert(Iterator pos, T&& value) override {
		size_t insert_index = pos - Iterator(data);
		if (insert_index > size) {
			throw std::out_of_range("Index out of range");
		}
		if (size == capacity) {
			data = normalize_capacity();
		}
		for (size_t i = size; i > insert_index; --i) {
			data[i] = std::move(data[i - 1]);
		}
		data[insert_index] = std::move(value);
		++size;
		return Iterator(data + insert_index);
	}

	/*********************************************************************
	 * ERASE FUNCTIONS: `erase()` overloads
	 *
	 * Erases element at given index/iterator, destroying it.
	 * Shifts subsequent elements using move semantics.
	 * Overloads for index and iterator position.
	 *
	 * Throws: std::out_of_range, exceptions from T's destructor.
	 *********************************************************************/

	void erase(size_t index) override {
		if (index >= size) {
			throw std::out_of_range("Index out of range");
		}
		data[index].~T();
		for (size_t i = index; i < size - 1; ++i) {
			data[i] = std::move(data[i + 1]);
		}
		--size;
	}

	Iterator erase(Iterator pos) override {
		size_t erase_index = pos - Iterator(data);

		if (erase_index >= size) {
			throw std::out_of_range("Index out of range");
		}
		data[erase_index].~T();
		for (size_t i = erase_index; i < size - 1; ++i) {
			data[i] = std::move(data[i + 1]);
		}
		--size;
		return pos;
	}

	/************************************************************
	 * SWAP FUNCTION: `swap(RawVector<T>& other) noexcept`
	 *
	 * Swaps data, size, and capacity with `other`.
	 * Fast, no element operations.
	 *************************************************************/

	void swap(RawVector<T>& other) noexcept {
		std::swap(size, other.size);
		std::swap(capacity, other.capacity);
		std::swap(data, other.data);
	};

	/// CHECKS IF OBJECT IS EMPTY, IF SIZE VARIABLE ISN'T HANDELED GOOD ENOUGH, CAN CAUSE CRUSH
	bool empty() { return size == 0 ? true : false; }

	/*********************************************************************
	 * DESTRUCTOR: `~RawVectorNonTriv()`
	 *
	 * Destroys all constructed elements and prints a message.
	 * Frees allocated memory.
	 *
	 * Throws: exceptions from T's destructor.
	 *********************************************************************/

	~RawVectorNonTriv() override { for (size_t i = 0; i < capacity; ++i) data[i].~T(); std::cout << "Destryed objects in RawVectorNonTriv, next comes memory freeing" << std::endl; }
};



/*********************************************************************
 * RAWVECTOR ALIAS TEMPLATE: `rawvector<T>`
 *
 * Alias template for `RawVector` specialized on `T`.
 * Chooses between `RawVectorTriv` and `RawVectorNonTriv` based on `myis_trivial`.
 *********************************************************************/
template <typename T>
using rawvector = std::conditional_t<myis_trivial<T>::value, RawVectorTriv<T>, RawVectorNonTriv<T>>;