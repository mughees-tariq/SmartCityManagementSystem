#pragma once
#include <iostream>
using namespace std;

// Circular Queue
class Queue {
private:
    void** data;
    int capacity;
    int front;
    int rear;
    int size;

public:
    Queue(int cap = 1000) : capacity(cap), front(0), rear(-1), size(0) {
        data = new void* [capacity];
    }

    // Enqueue rear moves (circular when at end)
    bool enqueue(void* item) {
        if (size >= capacity) return false;

        rear = (rear + 1) % capacity;
        data[rear] = item;
        size++;
        return true;
    }

    // Dequeue front moves (circular when at end)
    void* dequeue() {
        if (size == 0) return nullptr;

        void* item = data[front];
        front = (front + 1) % capacity;
        size--;
        return item;
    }

    // Peek front 
    void* peek() {
        if (size == 0) return nullptr;
        return data[front];
    }

    // Check if empty
    bool isEmpty() const { return size == 0; }

    // Check if full
    bool isFull() const { return size >= capacity; }

    // Get size
    int getSize() const { return size; }

    // Clear
    void clear() {
        front = 0;
        rear = -1;
        size = 0;
    }

    // Destructor
    ~Queue() {
        delete[] data;
    }
};

