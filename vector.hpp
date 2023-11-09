#ifndef VECTOR_H
#define VECTOR_H

#include<cstdlib>
#include <stdexcept>

using namespace std;

template <typename T>
class Vector
{
	private:
		T *data;						//pointer to int(array) to store elements
		int v_size;						//current size of vector (number of elements in vector)
		int v_capacity;					//capacity of vector
	public:
		Vector(int cap=0);			//Constructor
		~Vector();					//Destructor
		int size() const;				//Return current size of vector
		int capacity() const;			//Return capacity of vector
		bool empty() const; 			//Rturn true if the vector is empty, False otherwise
		const T& front();				//Returns reference of the first element in the vector
		const T& back();				//Returns reference of the Last element in the vector
		void push_back(T element);		//Add an element at the end of vector
		void insert(int index, T element); //Add an element at the index 
		void erase(int index);			//Removes an element from the index
		T& operator[](int index);			//Returns the reference of an element at given index
		T& at(int index); 				//return reference of the element at given index
		void shrink_to_fit();			//Reduce vector capacity to fit its size
		void display();
    protected:
		void reserve(int newCap);		//copies current data into an extended array
};

// ------------- Vector class definition ----------------------- //

// constructor of vector class
template <typename T>
Vector<T>::Vector(int cap) : v_size(0), v_capacity(cap) {
	data = new T[v_capacity];								// allocating cap size array to data
}

// destructor of vector class
template <typename T>
Vector<T>::~Vector() {
	delete [] data;											// deallocating memory assigned to data
}

// returns current size of vector
template <typename T>
int Vector<T>::size() const {
	return v_size;
}

// returns capacity of vector
template <typename T>
int Vector<T>::capacity() const {
	return v_capacity;
}

// returns true if the vector is empty, false otherwise
template <typename T>
bool Vector<T>::empty() const {
	return v_size == 0;
}

// returns reference of the first element in the vector
template <typename T>
const T& Vector<T>::front() {
	// throws an out of range error if the vector is empty
	if (empty()) throw out_of_range("The vector is empty!");

	// returns reference of the first element
	return data[0];
}

// returns reference of the Last element in the vector
template <typename T>
const T& Vector<T>::back() {
	// throws an out of range error if the vector is empty
	if (empty()) throw out_of_range("The vector is empty!");

	// returns reference of the last element
	return data[v_size - 1];
}

// copies current data into an extended array
template <typename T>
void Vector<T>::reserve(int newCap) {
	// new array to hold data
	T* newData = new T[newCap];

	// copies elements from data to newData
	for (int i = 0; i < v_size; i++) {
		newData[i] = data[i];
	}

	// checks if original data doesn't point to null and deallocates memory
	if (data != nullptr) delete [] data;

	// assigns new data to data
	data = newData;

	// updates the capacity
	v_capacity = newCap;
}

// adds an element at the end of vector
template <typename T>
void Vector<T>::push_back(T element) {
	// checks if the vector is full and resizes
	if (size() == capacity()) {
		reserve(max(1, 2 * v_capacity));
	}

	// adds element to the back and increases size
	data[v_size++] = element;
}

// adds an element at the index
template <typename T>
void Vector<T>::insert(int index, T element) {
	// checks if index is larger than size - 1
	if (index > size() - 1) {
		throw out_of_range("The index is out of range!");
	}

	// checks if vector is full and resizes
	if (size() == capacity()) {
		reserve(max(1, 2 * v_capacity));
	}

	// moves every element from index back by one
	for (int i = size() - 1; i >= index; i--) {
		data[i + 1] = data[i];
	}

	// inserts element at index
	data[index] = element;

	// increment size
	v_size++;
}

// removes an element from the index
template <typename T>
void Vector<T>::erase(int index) {
	// if index is out of range, throw out of range error
	if (index < 0 || index > size() - 1) {
		throw out_of_range("The index is out of range!");
	}

	// moves elements from index forward by one
	for (int i = index; i < size() - 1; i++) {
		data[i] = data[i + 1];
	}

	// decrement size
	v_size--;
}		

// returns the reference of an element at given index
template <typename T>
T& Vector<T>::operator[](int index) {
	// returns element at index
	return data[index];
}

// returns reference of the element at given index
template <typename T>
T& Vector<T>::at(int index) {
	// if index is out of range, throw out of range error
	if (index < 0 || index > size() - 1) {
		throw out_of_range("The index is out of range!");
	}

	// returns element at index
	return data[index];
}

// reduces vector capacity to fit its size
template <typename T>
void Vector<T>::shrink_to_fit() {
	reserve(size());
}			

#endif