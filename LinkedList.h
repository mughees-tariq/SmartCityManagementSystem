#pragma once
#include <iostream>
using namespace std;

//Node public class
struct Node {
	void* data;
	Node* next;
	
	Node(void* d) {
		data = d;
		next = nullptr;
	}
};

class LinkedList {
private:
	Node* head;
	

public:
	int size;
	LinkedList() {
		head = nullptr;
		size = 0;
	}
	//Insert at tail func
	void insert(void* d) {
		Node* newnode = new Node(d);
		if (!head) {
			head = newnode;
		}
		else {
			Node* temp = head;
			while (temp->next) {
				temp = temp->next;
			}temp->next = newnode;
		}size++;
	}
	void insertAtHead(void* d) {
		Node* newnode = new Node(d);
			newnode->next = head;
			head = newnode;
			size++;
	}
	void insertAfter(void* newd, void* presentdata) {
		if (!head) {
			return;
		}
		Node* newnode = new Node(newd);
		Node* temp = head;
		while (temp->next) {
			if (temp->data == presentdata) {
				newnode->next = temp->next;
				temp->next = newnode;
				size++;
				return;
			}
			else {
				temp = temp->next;
			}
		}
		return;
	}
	//Get Node
	Node* getNode(int index) {
		if (index < 0 || index >= size) {
			return nullptr;
		}
		Node* temp = head;
		for (int i = 0; i < index; i++) {
			temp = temp->next;
		}return temp;
	}
	//Get data
	void* get(int index) {
		Node* node = getNode(index);
		if (node) {
			return node->data;
		}
		else {
			return nullptr;
		}
	}
	//Delete by index
	bool deleteAt(int index) {
		if (index < 0 || index >= size) {
			return false;
		}
		if (index == 0) {
			Node* temp = head;
			head = head->next;
			delete temp;
			size--;
			return true;
		}
		Node* prev = getNode(index - 1);
		Node* todelete = prev->next;
		prev->next = todelete->next;
		delete todelete;
		size--;
		return true;
	}
	//Getindex
	int search(void* data) {
		Node* temp = head;
		int index = 0;
		while (temp) {
			if (temp->data == data) {
				return index;
			}
			temp = temp->next;
			index++;
		}
		return-1;
	}
	int getSize()const {
		return size;
	}
	bool isEmpty()const {
		return head == nullptr;
	}
	Node* getHead()const {
		return head;
	}
	//Clear entire list
	void clear() {
		Node* current = head;
		while (current) {
			Node* next = current->next;
			delete current;
			current = next;
		}
		head = nullptr;
		size = 0;
	}
	void display() {
		Node* temp = head;
		while (temp) {
			cout << temp->data << " -> ";
			temp = temp->next;
		}
		cout << "Null" << endl;
	}
	void detach() {
		head = nullptr;
		size = 0;
	}
	~LinkedList() {
		clear();
	}
};