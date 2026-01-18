#pragma once
#include <iostream>
using namespace std;

class Stack {
private:
	void** data;
	int capacity;
	int top;
public:
	Stack(int cap = 1000) {
		capacity = cap;
		top = -1;
		data = new void* [capacity];
	}
	bool push(void* item) {
		if (top >= capacity - 1) {
			return false;
		}
		data[++top] = item;
		return true;
	}
	void* pop() {
		if (top < 0) {
			return nullptr;
		}
		return data[top--];
	}
	void* peek() {
		if (top < 0) {
			return nullptr;
		}return data[top];
	}
	bool isEmpty()const {
		return top < 0;
	}
	int getSize()const {
		return top + 1;
	}
	void clear() {
		top = -1;
	}
	~Stack() {
		delete[]data;
	}
};