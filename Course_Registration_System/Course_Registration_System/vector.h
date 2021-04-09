#ifndef VECTOR_H
#define VECTOR_H

template <typename T>
struct vector {
	T* array;
	int _capacity;
	int _size;

	int size();
	void push_back(T _x);
	void erase(int _index);

};
#endif