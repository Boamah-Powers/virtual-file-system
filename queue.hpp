#ifndef QUEUE_H
#define QUEUE_H

#include<cstdlib>
#include<stdexcept>

using namespace std;

template <typename T>
class Queue
{
	private:
		T *array;
		int capacity;	//Max Capacity of the Queue
		int size;		//current number of elements in the Queue
		int front;		//front of the Queue
		int rear;		//index where a new element will be added
	public:
		Queue(int capacity=10);
		~Queue();
		void enqueue(T); 
		T dequeue();
		bool isEmpty();
		bool isFull();
		T& front_element();
};
//================================================

// constructor for the queue class
template<typename T> Queue<T>::Queue(int capacity) : capacity(capacity), size(0), front(0), rear(0)
{
	// creates a dynamic array of the specified queue size
	array = new T[capacity];								
}

// destructor for the queue class
template<typename T> Queue<T>::~Queue()
{
	// deallocates dynamically assigned memory
	delete[] array;											
}
//=============================================
// returns reference to element at the front
template<typename T> T& Queue<T>::front_element() {
    return array[front];
}
//=============================================
//===========================================
// inserts element at the rear of the queue
template<typename T> void Queue<T>::enqueue(T element)
{
	// checks if the queue is full and throws an exception
	if (isFull()) {
		throw runtime_error("The queue is full");
	}

	// inserts element at the end of the queue
	array[rear] = element;

	// moves the rear to the next position
	// -- the modulus with the capacity handles the circularity
	// -- of the queue
	rear = (rear + 1) % capacity;

	// increasing size of the queue
	size++;
}

//===========================================
// removes an element from the front of the queue
template<typename T> T Queue<T>::dequeue()
{
	// checks if the queue is empty and throws an exception
	if (isEmpty()) {
		throw runtime_error("The queue is empty");
	}

	// assigning the front to a temp variable before increasing it
	T front_element = array[front];

	// moves the front to the next position
	// -- the modulus with the capacity handles the circularity
	// -- of the queue
	front = (front + 1) % capacity;

	// reducing size of the queue
	size--;

	// returning the front element of the queue
	return front_element;

}
//===========================================
// checks if the queue is empty
template<typename T> bool Queue<T>::isEmpty()
{
	return size == 0;
}
//===========================================
// checks if the queue is full
template<typename T> bool Queue<T>::isFull()
{
	return size == capacity;
}
//=============================================

#endif