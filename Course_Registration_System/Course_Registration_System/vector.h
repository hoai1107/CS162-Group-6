#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
struct vector {
	T* array;
	int _size = 0;
	int _capacity = 0;

	int size() {
		return _size;
	}

	void push_back(T _x);
	void erase(int _index);
	T operator[](int _index);
};
#endif