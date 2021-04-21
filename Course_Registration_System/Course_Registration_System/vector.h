#ifndef Vector_H
#define Vector_H

#include <stdexcept>
#include <iostream>
using namespace std;

template <typename T>
struct Vector {
	T* array;
	int _size = 0;
	int _capacity = 1;

	Vector() {
		array = new T[_capacity];
	}

	Vector(const Vector <T> &that) {
        _size = that._size + 1;
	    _capacity = that._capacity;
	    delete[] array;
	    array = new T[_capacity];
	    for (int i = 0; i < _size; i++)
            array[i] = that.array[i];
        return;
	}

	int size() {
		return _size;
	}

    T& operator[](int _index) {
		if (_index < 0)
			throw std::invalid_argument("access negative index!");

		if (_index >= _size)
			throw std::invalid_argument("access out of bounds index!");

		return array[_index];

	}

	void push_back(T _x) {
		if (_size == _capacity) {
			T* tmp = new T[_capacity * 2];
			for (int i = 0; i < _capacity; i++)
				tmp[i] = array[i];
			array = tmp;
			_capacity *= 2;
		}
		array[_size++] = _x;
		return;
	}

	void erase(int _index) {
		if (_index < 0)
			throw std::invalid_argument("erase negative index!");

		if (_index >= _size)
			throw std::invalid_argument("erase out of bounds index!");

		for (int i = _index; i < _size - 1; i++)
			array[i] = array[i + 1];

		_size--;

		return;
	}

	void clear() {
		if (_size == 0) return;
		_size = 0;
		delete[] array;
		return;
	}

	~Vector() {
		delete[] array;
	}
};

#endif
