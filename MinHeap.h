#pragma once
#include <iostream>

using namespace std;

struct HeapNode {
	double priority;
	void* data;
	HeapNode() {
		priority = 0;
		data = nullptr;

	}
	HeapNode(double p, void* d) {
		priority = p;
		data = d;
	}
};

class MinHeap {
private:
	HeapNode* heap;
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
		HeapNode temp = heap[i];
		heap[i] = heap[j];
		heap[j] = temp;
	}
	void heapifyUp(int index) {
		while (index > 0&&heap[parent(index)].priority>heap[index].priority) {
			swap(index, parent(index));
			index = parent(index);
		}
	}
	void heapifyDown(int index) {
		int minIndex = index;
		int left = leftChild(index);
		int right = rightChild(index);

		if (left < size && heap[left].priority < heap[minIndex].priority) {
			minIndex = left;
		}
		if (right < size && heap[right].priority < heap[minIndex].priority) {
			minIndex = right;
		}
		if (minIndex != index) {
			swap(index, minIndex);
			heapifyDown(minIndex);
		}
	}

public:
	MinHeap(int cap = 1000) {
		capacity = cap;
		size = 0;
		heap = new HeapNode[cap];
	}
	bool insert(double priority, void* data) {
		if (size >= capacity) {
			return false;
		}
		heap[size] = HeapNode(priority, data);
		heapifyUp(size);
		size++;
		return true;
	}
	HeapNode extractMin() {
		if (size == 0) {
			return HeapNode();
		}
		HeapNode minNode = heap[0];
		heap[0] = heap[size - 1];
		size--;
		heapifyDown(0);
		return minNode;
	}
	HeapNode peek() {
		if (size == 0) {
			return HeapNode();
		}
		return heap[0];
	}
	bool isEmpty()const {
		return size == 0;
	}
	bool getSize()const {
		return size;
	}
	void clear() {
		delete[]heap;
	}
	~MinHeap() {
		delete[]heap;
	}
	void display() {
		cout << "MinHeap ( size= " << size << ") :";
		for (int i = 0; i < size; i++) {
			cout << heap[i].priority << " ";
		}
		cout << endl;
	}
};

