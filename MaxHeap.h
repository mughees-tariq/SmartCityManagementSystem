#pragma once
#include <iostream>

using namespace std;

struct MaxHeapNode {
	double priority;
	void* data;

	MaxHeapNode() {
		priority = 0;
		data = nullptr;
	}

	MaxHeapNode(double p, void* d) {
		priority = p;
		data = d;
	}
};

class MaxHeap {
private:
	MaxHeapNode* heap;
	int capacity;
	int size;

	int parent(int i) {
		return (i - 1) / 2;
	}

	int leftChild(int i) {
		return 2 * i + 1;
	}

	int rightChild(int i) {
		return 2 * i + 2;
	}

	void swap(int i, int j) {
		MaxHeapNode temp = heap[i];
		heap[i] = heap[j];
		heap[j] = temp;
	}

	void heapifyUp(int index) {
		while (index > 0 && heap[parent(index)].priority < heap[index].priority) {
			swap(index, parent(index));
			index = parent(index);
		}
	}

	void heapifyDown(int index) {
		int maxIndex = index;
		int left = leftChild(index);
		int right = rightChild(index);

		if (left < size && heap[left].priority > heap[maxIndex].priority) {
			maxIndex = left;
		}
		if (right < size && heap[right].priority > heap[maxIndex].priority) {
			maxIndex = right;
		}

		if (maxIndex != index) {
			swap(index, maxIndex);
			heapifyDown(maxIndex);
		}
	}

public:
	MaxHeap(int cap = 1000) {
		capacity = cap;
		size = 0;
		heap = new MaxHeapNode[cap];
	}

	bool insert(double priority, void* data) {
		if (size >= capacity) {
			return false;
		}
		heap[size] = MaxHeapNode(priority, data);
		heapifyUp(size);
		size++;
		return true;
	}

	MaxHeapNode extractMax() {
		if (size == 0) {
			return MaxHeapNode();
		}
		MaxHeapNode maxNode = heap[0];
		heap[0] = heap[size - 1];
		size--;
		heapifyDown(0);
		return maxNode;
	}

	MaxHeapNode peek() {
		if (size == 0) {
			return MaxHeapNode();
		}
		return heap[0];
	}

	bool isEmpty() const {
		return size == 0;
	}

	int getSize() const {
		return size;
	}

	void clear() {
		size = 0;
	}

	~MaxHeap() {
		delete[] heap;
	}

	void display() {
		cout << "MaxHeap (size=" << size << "): ";
		for (int i = 0; i < size; i++) {
			cout << heap[i].priority << " ";
		}
		cout << endl;
	}
};