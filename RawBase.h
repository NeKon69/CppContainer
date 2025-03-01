#pragma once
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <type_traits>
#include <utility>
#include <stdexcept>

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
		if (data) { /*std::cout << "Freeing memory at address: " << static_cast<void*>(data) << " | ";*/ free(data); }
		//std::cout << "RawVector Object Destroyed with size: " << size << " and with capacity: " << capacity << std::endl;
	}

	virtual RawVector* clone() const = 0;

	virtual void push_back(const T& elem) = 0;
	virtual void push_back(T&& elem) = 0;

	virtual T& at(const size_t index) = 0;
	virtual const T& at(const size_t index) const = 0;
	virtual T& operator [] (const size_t index) = 0;
	virtual const T& operator [] (const size_t index) const = 0;

	size_t get_size() const { return size; }
	size_t get_capacity() const { return capacity; };

	virtual void resize(size_t new_size) = 0;
	virtual void reserve(size_t reserve_size) = 0;
	virtual void clear() = 0;
	virtual void shrink_to_fit() = 0;
	virtual void pop_back() = 0;

	/// BEGINNING
	/// ***************************************************************************************************************
	/// ITERATORS
	/// next goes class of itreators that uses "using" word to represent both const and non-const iterators
	///
	/// added mostly used operators, like ++, --, *, ->, extc.
	/// ***************************************************************************************************************
	/// TASKS ADD ADDITIONAL CLASS FOR R ITERATORS
	/// END

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
		size_t operator -(IteratorBase iter) const { return ptr - iter.ptr; }

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

	const Iterator data_get() const { return cbegin(); }

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

	/// Data members data, size, capacity ARE ALREADY inherited from RawVector, no need to declare them again here!

	/// BEGINNING
	/// ********************************************************************************************************************************************************************
	/// PRIVATE MEMBERS EXPLANATION CONTINUED
	/// normalize_capacity - function, that if not enough space doing a reallocation with copying all of the elements from data
	///
	/// returns a pointer to new copied and reallocated data
	/// ********************************************************************************************************************************************************************
	/// END

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

	/// BEGINNING
	/// *************************************************************************************************
	/// DEAFULT CONSTRUCTOR
	/// usual conctructor that is called than no arguments was gvien, reserves space for 1 element
	/// *************************************************************************************************
	/// END
	RawVectorTriv() {
		data = (T*)malloc(sizeof(T));
	};

	/// BEGINNING
	/// ***************************************************************************************
	/// CONSTRUCTOR WITH SIZE
	/// conctructor that is called when size is given, reserves space for size elements
	/// ***************************************************************************************
	/// END

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

	/// BEGINNING
	/// ************************************************************************************************************************************************
	/// COPY CONSTRUCTOR OVERLOAD
	/// conctructor that is called when size and value are given, reserves space for size elements and fills them with value from previous vector
	///
	/// IMPORTANT - doesn't copy the elements that are outside the size, but actually reserved
	/// ************************************************************************************************************************************************
	/// END
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

	/// BEGINNING
	/// **********************************************************************************************************
	/// CONSTRUCTOR WITH MOVE SEMANTIC
	/// conctructor that is called when given an object of the class with move semantic
	///
	/// takes all of the data from it, as well as size and capacity, leaving it without data nor size/capacity
	/// **********************************************************************************************************
	/// END
	RawVectorTriv(RawVectorTriv&& other) noexcept {
		data = other.data;
		size = other.size;
		capacity = other.capacity;

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

	/// BEGINNING
	/// ****************************************************************************************************************
	/// MOVE_SEMANTIC OPERATOR OVERLOAD
	/// this function being called then the object is given with move semantic
	///
	/// same logic as in the CONSTRUCTOR with move semantic, just with freeing the data of the curr object firstly
	/// ****************************************************************************************************************
	/// END
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

	/// BEGINNING
	/// *******************************************************************************************************
	/// INDEXING FUNCTIONS
	/// next 4 functions used to get element by index, with the check on the "at" ones, and without on []
	///
	/// const / non-const functions written
	/// *******************************************************************************************************
	/// END
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

	/// BEGINNING
	/// *********************************************************************************************************************************************
	/// RESERVE FUNCTION
	/// this function does a reserve operation, same as in the vector, it just reserves more space, without actually revealing those elements
	/// *********************************************************************************************************************************************
	/// END

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

	/// BEGINNING
	/// **************************************************************************************
	/// CLEAR FUNCTION
	/// frees memory, leaving vector with no data, and only 1 reserved undefined element
	/// **************************************************************************************
	/// END

	void clear() override {
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

	/// BEGINNING
	/// ************************************************************************************************************
	/// POP_BACK FUNCTION
	/// this one works a bit different from what it looks like to user
	///
	/// instead of actually deleting it from the back, we just make size smaller, so this element is not visible
	/// ************************************************************************************************************
	/// END

	void pop_back() override {
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

	/// BEGINNING
	/// **************************************************************************************************
	/// ERASE FUNCTIONS
	/// erases number on the index of the  vector, uses slide by 1 to move objects before given index
	/// **************************************************************************************************
	/// TASKS: ADD ITERATORS ACCEPTING FUNCTIONS
	/// END

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

	/// BEGINNING
	/// *************************************************************
	/// SWAP FUNCTION
	/// just swaps the data, size and capacity of the two objects
	/// *************************************************************
	/// END

	void swap(RawVector<T>& other) noexcept override {
		std::swap(data, other.data);
		std::swap(size, other.size);
		std::swap(capacity, other.capacity);
	}

	/// BEGINNING
	/// ***************************************************************************************************************
	/// COPY FUNCTION
	/// idk why i created this, but it copies data from curr vector, and retuns it
	///
	/// NOTE: RETURNS COMPLETELY NEW DATA, NEW POINTER, AND IT ALSO DONT HAVE ANY ACOSIANTIONS WITH THE OLD VECTOR
	/// ***************************************************************************************************************
	/// END

	RawVectorTriv copy() {
		RawVectorTriv<T> new_vector(size);
		new_vector.reserve(capacity);
		std::memcpy(new_vector.data, data, size * sizeof(T));
		return new_vector;
	}

	/// CHECKS IF OBJECT IS EMPTY, IF SIZE VARIABLE ISN'T HANDELED GOOD ENOUGH, CAN CAUSE CRUSH
	bool empty() { return size == 0; }

	/// BEGINNING
	/// *************************************************************************************
	/// DESTRUCTOR
	/// actual data already frees in parent class, so here it just for debugging purposes
	/// *************************************************************************************
	/// END

	~RawVectorTriv() override {
		//std::cout << "RawVectorTriv Object Destroyed next comes memory freeing" << std::endl;
	}
};

template<typename T>
class RawVectorNonTriv : public RawVector<T> {
protected:
	using RawVector<T>::data;
	using RawVector<T>::size;
	using RawVector<T>::capacity;

public:
	RawVectorNonTriv* clone() const override { /*TODO: Implement for non-trivial types*/ return nullptr; /* Placeholder */ }

	RawVectorNonTriv() : RawVector() {}
	RawVectorNonTriv(size_t size_) : RawVector() {}
	RawVectorNonTriv(const RawVectorNonTriv& other) : RawVector() {}
	RawVectorNonTriv(RawVectorNonTriv&& other) noexcept : RawVector() {}
	RawVectorNonTriv& operator=(const RawVectorNonTriv& other) override { return *this; }
	RawVectorNonTriv& operator=(RawVectorNonTriv&& other) noexcept override { return *this; }

	void push_back(const T& elem) override { /*TODO: Implement for non-trivial types*/ };
	void push_back(T&& elem) override { /*TODO: Implement for non-trivial types*/ };

	T& at(const size_t index) override { /*TODO: Implement for non-trivial types*/ throw std::logic_error("Not implemented"); };
	const T& at(const size_t index) const override { /*TODO: Implement for non-trivial types*/ throw std::logic_error("Not implemented"); };
	T& operator [] (const size_t index) override { /*TODO: Implement for non-trivial types*/ throw std::logic_error("Not implemented"); };
	const T& operator [] (const size_t index) const override { /*TODO: Implement for non-trivial types*/ throw std::logic_error("Not implemented"); };

	size_t get_size() const override { return RawVector::get_size(); }
	size_t get_capacity() const override { return RawVector::get_capacity(); }

	void resize(size_t new_size) override { /*TODO: Implement for non-trivial types*/ };
	void reserve(size_t reserve_size) override { /*TODO: Implement for non-trivial types*/ };
	void clear() override { /*TODO: Implement for non-trivial types*/ };
	void shrink_to_fit() override { /*TODO: Implement for non-trivial types*/ };
	void pop_back() override { /*TODO: Implement for non-trivial types*/ };

	void insert(size_t index, const T& value) override { /*TODO: Implement for non-trivial types*/ };
	void insert(size_t index, T&& value) override { /*TODO: Implement for non-trivial types*/ };

	void erase(size_t index) override { /*TODO: Implement for non-trivial types*/ };
	void swap(RawVector<T>& other) noexcept { /*TODO: Implement for non-trivial types*/ };

	~RawVectorNonTriv() override { /*std::cout << "RawVectorNonTriv Object Destroyed (empty impl)" << std::endl;*/ }
};



template <typename T>
using vector = std::conditional_t<myis_trivial<T>::value, RawVectorTriv<T>, RawVectorNonTriv<T>>;