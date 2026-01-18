#pragma once
#include <string>
#include <iostream>
#include "LinkedList.h"
using namespace std;

struct HashEntry {
    string key;
    void* value;

    HashEntry(string k, void* v) : key(k), value(v) {}
    HashEntry(const HashEntry&) = delete;
    HashEntry& operator=(const HashEntry&) = delete;
};

class HashTable {
private:
    LinkedList* table;
    int capacity;
    int count;

    int hashFunction(const string& key) const {
        if (key.empty() || capacity == 0) return 0;

        unsigned long hash = 0;
        for (size_t i = 0; i < key.length(); i++) {
            hash = (hash * 31 + key[i]) % (unsigned long)capacity;
        }
        return (int)hash;
    }

    HashEntry* getEntryFromNode(void* nodeData) const {
        if (!nodeData) return nullptr;
        return static_cast<HashEntry*>(nodeData);
    }

public:

    HashTable(int cap = 101) {  
        if (cap <= 0) {
            cap = 101;
        }

        capacity = cap;
        count = 0;
        table = new LinkedList[capacity];

        cout << "HashTable created with capacity " << capacity << endl;
    }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    void insert(const string& key, void* value) {
        if (key.empty()) {
            return;
        }

        if (!table) {
            return;
        }

        int index = hashFunction(key);

        if (index < 0 || index >= capacity) {
            return;
        }

        Node* current = table[index].getHead();
        while (current != nullptr) {
            HashEntry* entry = getEntryFromNode(current->data);
            if (entry && entry->key == key) {
                entry->value = value;
                return;
            }
            current = current->next;
        }

        // Create new entry
        HashEntry* newEntry = new HashEntry(key, value);

        // Insert into linked list
        table[index].insert(newEntry);
        count++;

    }

    void* search(const string& key) const {
        if (key.empty() || !table) return nullptr;

        int index = hashFunction(key);
        if (index < 0 || index >= capacity) return nullptr;

        Node* current = table[index].getHead();
        while (current != nullptr) {
            HashEntry* entry = getEntryFromNode(current->data);
            if (entry && entry->key == key) {
                return entry->value;
            }
            current = current->next;
        }

        return nullptr;
    }

    bool remove(const string& key) {
        if (key.empty() || !table) return false;

        int index = hashFunction(key);
        if (index < 0 || index >= capacity) return false;

        Node* current = table[index].getHead();
        Node* prev = nullptr;
        int position = 0;

        while (current != nullptr) {
            HashEntry* entry = getEntryFromNode(current->data);
            if (entry && entry->key == key) {
                // Remove node from linked list
                table[index].deleteAt(position);

                // Delete the HashEntry
                delete entry;

                count--;
                return true;
            }

            current = current->next;
            position++;
        }

        return false;
    }

    int getCount() const {
        return count;
    }

    bool contains(const string& key) const {
        return search(key) != nullptr;
    }

    string* getAllKeys(int& outSize) const {
        outSize = count;
        if (count == 0 || !table) return nullptr;

        string* keys = new string[count];
        int idx = 0;

        for (int i = 0; i < capacity && idx < count; i++) {
            Node* current = table[i].getHead();
            while (current != nullptr && idx < count) {
                HashEntry* entry = getEntryFromNode(current->data);
                if (entry) {
                    keys[idx] = entry->key;
                    idx++;
                }
                current = current->next;
            }
        }

        outSize = idx;  // Actual number of keys retrieved
        return keys;
    }

    void display() const {
        if (!table) {
            return;
        }
        for (int i = 0; i < capacity; i++) {
            if (!table[i].isEmpty()) {
                cout << "Index " << i << ": ";
                Node* current = table[i].getHead();
                while (current != nullptr) {
                    HashEntry* entry = getEntryFromNode(current->data);
                    if (entry) {
                        cout << "[" << entry->key << "] -> ";
                    }
                    current = current->next;
                }
                cout << "NULL" << endl;
            }
        }
    }


    ~HashTable() {

        if (table) {
            for (int i = 0; i < capacity; i++) {
                Node* current = table[i].getHead();
                while (current != nullptr) {
                    HashEntry* entry = getEntryFromNode(current->data);
                    if (entry) {
                        delete entry;
                    }
                    current = current->next;
                }
                // LinkedList destructor will handle node cleanup
            }

            delete[] table;
            table = nullptr;
        }

        capacity = 0;
        count = 0;
    }
};