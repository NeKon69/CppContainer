#pragma once
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include "trivial_check.hpp"

namespace raw {
	template<typename T>
	class vector_base {
	private:
		static const bool is_trivial_v = myis_trivial<T>::value;
		T* data = nullptr;
		size_t size = 0;
		size_t capacity = 0;

		virtual T* normalize_capacity() = 0;

		friend class vector_triv<T>;
		friend class vector_non_triv<T>;
	public:

		vector_base() : data(nullptr), size(0), capacity(1) {}
		virtual ~vector_base() {
			std::cout << "Freeing memory at address: " << static_cast<void*>(data) << " | ";
			if (data) { free(data); }
			std::cout << "vector_base Object Destroyed with size: " << size << " and with capacity: " << capacity << std::endl;
		}

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

		template <typename iter>
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

		virtual void swap(vector_base<T>& other) noexcept = 0;

	};

}