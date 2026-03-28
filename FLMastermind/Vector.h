#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

template <typename T> class Vector {
	private:
		T* data;
		int capacity;
		int sz; // size
	public:
		// constructor
		Vector() {
			sz = 0;
			capacity = 1296; // total # of combinations of colors there can be
			data = new T[capacity];
		};
		Vector(int cap) {
			sz = 0;
			capacity = cap;
			data = new T[capacity];
		};
		// copy constructor - needed when pushing Vector<string> into Vector<Vector<string>>
		Vector(const Vector& other) {
			sz = other.sz;
			capacity = other.capacity;
			data = new T[capacity];
			for (int i = 0; i < sz; i++)
				data[i] = other.data[i];
		};

		Vector& operator=(const Vector& other) {
			if (this != &other) {
				delete[] data;
				sz = other.sz;
				capacity = other.capacity;
				data = new T[capacity];
				for (int i = 0; i < sz; i++)
					data[i] = other.data[i];
			}
			return *this;
		}

		// add item
		void push_back(const T& item) {
			if (sz >= capacity) {
				throw std::overflow_error("Vector capacity exceeded"); // should never be called based on default capacity
			}
			else {
				data[sz++] = item;
			};
		};

		// access item at certain index
		T& at(int index) {
			if (index < 0 || index >= sz) {
				throw std::out_of_range("Index out of range");
			}
			return data[index];
		};

		// get vector size
		int size() const {
			return sz;
		};

		// clear vector out
		void clear() {
			sz = 0;
		};

		// destructor
		~Vector() {
			delete[] data;
		};
};

#endif