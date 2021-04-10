#ifndef VECTOR_H
#define VECTOR_H

#include <stdexcept>

template <typename T>
struct vector {
	T* array;
<<<<<<< HEAD
	int _capacity;
	int _size;

	int size();
	void push_back(T _x);
	void erase(int _index);s
=======
	int _size = 0;
	int _capacity = 1;

	vector() : array(new T[_capacity]) {}

	int size() {
		return _size;
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

>>>>>>> refs/remotes/origin/main
	T operator[](int _index);
};
#endif
