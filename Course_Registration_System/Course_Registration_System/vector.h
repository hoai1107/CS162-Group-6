#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>

template <typename T>
struct vector {
	T* array;
	int _size = 0;
	int _capacity = 1;

	vector() : array(new T[_capacity]) {}

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

	void erase(int _index);

};
#endif
