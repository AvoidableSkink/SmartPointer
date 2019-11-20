// SharedPointer.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

namespace usu {
	template <typename T> 
	class shared_ptr {
	public:
		shared_ptr(T dta) {
			data = new T(dta);
			useCount = new int(1);
		}

		shared_ptr(T* dta) {
			data = dta;
			useCount = new unsigned int(1);
		}

		shared_ptr(const shared_ptr& obj) noexcept {
			data = obj.data;
			useCount = obj.useCount;
			*useCount += 1;
		}     

		shared_ptr(shared_ptr&& obj) noexcept {
			//transfer ownership
			data = obj.data;
			useCount = obj.useCount;

			obj.useCount = nullptr;
			obj.data = nullptr;
		}

		//destructor
		~shared_ptr() {
			if (useCount == nullptr)
				return;

			*useCount -= 1;
			if (useCount == 0) {
				delete data;
				delete useCount;
			}
		}

		shared_ptr& operator=(const shared_ptr& rhs) noexcept {
			data = rhs.data;
			useCount = rhs.useCount;
			*useCount += 1;
			return *this;
		}

		shared_ptr& operator=(shared_ptr&& rhs) noexcept {
			if (this != &rhs) { 
				std::swap(data, rhs.data);
				std::swap(useCount, rhs.useCount);
			}
			return *this;
		}

		T* operator->() {
			return data;
		}

		T operator*() {
			return *data;
		}

		unsigned int use_count() {
			return *useCount;
		}

		T* get() {
			return data;
		}
	private:
		T* data;
		unsigned int* useCount;

	};

	template<typename T>
	class shared_ptr<T[]> {
	private:
		T* data;
		unsigned int* useCount;
		unsigned int arraySize = 0;

	public:
		shared_ptr(T myArray, unsigned int count) {
			data = myArray;
			arraySize = count;
			useCount = new int(1);
		}

		shared_ptr(T* myArray, unsigned int count) {
			data = myArray;
			arraySize = count;
			useCount = new unsigned int(1);
		}

		shared_ptr(const shared_ptr& obj) {
			arraySize = obj.arraySize;
			data = obj.data;
			useCount = obj.useCount;
			*useCount++;
		}

		shared_ptr(shared_ptr&& obj) {
			arraySize = obj.arraySize;

			//transfer ownership
			data = obj.data;
			useCount = obj.useCount;

			obj.useCount = nullptr;
			obj.data = nullptr;
		}

		shared_ptr& operator=(const shared_ptr& rhs) {
			data = rhs.data;
			useCount = rhs.useCount;
			*useCount += 1;
			return *this;
		}

		shared_ptr& operator=(shared_ptr&& rhs) {
			if (this != &rhs) {
				std::swap(data, rhs.data);
				std::swap(useCount, rhs.useCount);
			}
			return *this;
		}

		T& operator[](unsigned int index) {
			if (index < 0 || index >= arraySize) { 
				throw std::exception();
			}
			return data[index];
		}

		unsigned int size() { return arraySize; } 

		//destructor
		~shared_ptr() {
			*useCount -= 1;
			if (useCount == 0) {
				delete[] data;
				delete useCount;
			}
		}
	};

	template <typename T, typename... Args>
	shared_ptr<T> make_shared(Args&& ... args)
	{
		return shared_ptr<T>(new T(std::forward<Args>(args)...));
	}

	template <typename T, unsigned int N>
	shared_ptr<T[]> make_shared_array()
	{
		return shared_ptr<T[]>(new T[N], N);
	}
}
