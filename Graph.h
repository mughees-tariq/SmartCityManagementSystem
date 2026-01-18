#pragma once
#include "LinkedList.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "Utilities.h"
#include <string>
#include <iostream>

using namespace std;

struct GraphNode {
    string id;
    string name;
    double latitude;
    double longitude;

    GraphNode() {
        id = "";
        name = "";
        latitude = longitude = 0.0;
    }
    GraphNode(string i, string nm, double latit, double longit) {
        id = i;
        name = nm;
        latitude = latit;
        longitude = longit;
    }
};

struct Edge {
    string destination;
    double weight;
    Edge() {
        destination = "";
        weight = 0.0;
    }
    Edge(string des, double wt) {
        destination = des;
        weight = wt;
    }
};

class Graph {
private:
    HashTable nodes;
    HashTable adjlist;
    int nodeCount;
    int maxNodes;

    int findNodeIndex(string* keys, int count, string target) {
        for (int i = 0; i < count; i++) {
            if (keys[i] == target) return i;
        }
        return -1;
    }


public:
    Graph() : nodes(200), adjlist(200) {
        nodeCount = 0;
        maxNodes = 200;
    }
    bool nodeExists(string id) {
        return nodes.contains(id);
    }
    void removeNode(string id) {
        if (!nodes.contains(id)) {
            return;
        }
        nodes.remove(id);
        adjlist.remove(id);
        nodeCount--;
    }
    void addNode(string id, string name, double lat, double longit) {
        if (nodes.contains(id)) {
            cout << "Node " << id << " already exists." << endl;
            return;
        }

        GraphNode* newnode = new GraphNode(id, name, lat, longit);
        nodes.insert(id, (void*)newnode);
        LinkedList* edges = new LinkedList();
        adjlist.insert(id, (void*)edges);
        nodeCount++;

        cout << "Added node: " << name << " [" << id << "]" << endl;

        autoConnectNewNode(id);
    }

    void addEdge(string from, string to, double weight) {
        if (!nodes.contains(from) || !nodes.contains(to)) {
            return;
        }
        LinkedList* edges = (LinkedList*)adjlist.search(from);
        if (edges) {
            Edge* edge = new Edge(to, weight);
            edges->insert((void*)edge);
        }
    }

    void addUndirectedEdge(string from, string to, double wt) {
        addEdge(from, to, wt);
        addEdge(to, from, wt);
    }

    GraphNode* getNode(string id) {
        return (GraphNode*)nodes.search(id);
    }

    double calculateDistance(string id1, string id2) {
        GraphNode* node1 = getNode(id1);
        GraphNode* node2 = getNode(id2);

        if (!node1 || !node2) {
            return -1;
        }
        return MathUtils::calculateDistance(node1->latitude, node1->longitude, node2->latitude, node2->longitude);
    }

    string* dijkstra(string start, string end, double& totalDistance) {
        int keyCount = 0;
        string* allKeys = nodes.getAllKeys(keyCount);

        string* path = new string[100];
        for (int i = 0; i < 100; i++) path[i] = "";

        if (keyCount == 0 || !allKeys) {
            totalDistance = -1.0;
            return path;
        }

        double* distances = new double[keyCount];
        int* previousIdx = new int[keyCount];
        bool* visited = new bool[keyCount];

        int startIdx = findNodeIndex(allKeys, keyCount, start);
        int endIdx = findNodeIndex(allKeys, keyCount, end);

        if (startIdx == -1 || endIdx == -1) {
            delete[] distances;
            delete[] previousIdx;
            delete[] visited;
            delete[] allKeys;
            totalDistance = -1.0;
            return path;
        }

        for (int i = 0; i < keyCount; i++) {
            distances[i] = 999999.9;
            previousIdx[i] = -1;
            visited[i] = false;
        }
        distances[startIdx] = 0.0;

        MinHeap pq(2000);
        pq.insert(0.0, (void*)new string(start));

        bool foundEnd = false;
        int iterations = 0;
        int maxIterations = keyCount * 100;

        while (!pq.isEmpty() && iterations < maxIterations) {
            iterations++;

            HeapNode minNode = pq.extractMin();
            string* currentStr = (string*)minNode.data;

            if (!currentStr) continue;

            string current = *currentStr;
            delete currentStr;
            currentStr = nullptr;

            int currentIdx = findNodeIndex(allKeys, keyCount, current);
            if (currentIdx == -1) continue;

            if (visited[currentIdx]) continue;
            visited[currentIdx] = true;

            if (currentIdx == endIdx) {
                totalDistance = distances[endIdx];
                foundEnd = true;
                break;
            }

            double currentDist = distances[currentIdx];

            LinkedList* edges = (LinkedList*)adjlist.search(current);
            if (!edges) continue;

            Node* temp = edges->getHead();
            while (temp) {
                Edge* edge = (Edge*)temp->data;
                if (!edge) {
                    temp = temp->next;
                    continue;
                }

                int neighborIdx = findNodeIndex(allKeys, keyCount, edge->destination);

                if (neighborIdx == -1 || visited[neighborIdx]) {
                    temp = temp->next;
                    continue;
                }

                double newDist = currentDist + edge->weight;

                if (newDist < distances[neighborIdx]) {
                    distances[neighborIdx] = newDist;
                    previousIdx[neighborIdx] = currentIdx;

                    if (pq.getSize() < 1500) {
                        pq.insert(newDist, (void*)new string(edge->destination));
                    }
                }

                temp = temp->next;
            }
        }

        while (!pq.isEmpty()) {
            HeapNode remaining = pq.extractMin();
            string* str = (string*)remaining.data;
            if (str) delete str;
        }

        if (foundEnd) {
            int pathLen = 0;
            int currentIdx = endIdx;

            while (currentIdx != -1 && pathLen < 100) {
                path[pathLen++] = allKeys[currentIdx];
                if (currentIdx == startIdx) break;
                currentIdx = previousIdx[currentIdx];
            }

            for (int i = 0; i < pathLen / 2; i++) {
                string temp = path[i];
                path[i] = path[pathLen - 1 - i];
                path[pathLen - 1 - i] = temp;
            }
        }
        else {
            totalDistance = -1.0;
        }

        delete[] distances;
        delete[] previousIdx;
        delete[] visited;
        delete[] allKeys;

        return path;
    }

    string* getAllNodeIDs(int& count) {
        return nodes.getAllKeys(count);
    }

    int getNodeCount()const {
        return nodeCount;
    }

    void display() {
        int count = 0;
        string* keys = nodes.getAllKeys(count);
        cout << "*** Graph ***" << endl;
        for (int i = 0; i < count; i++) {
            GraphNode* node = getNode(keys[i]);
            cout << node->id << " ( " << node->name << " ): ";
            LinkedList* edges = (LinkedList*)adjlist.search(keys[i]);
            if (edges) {
                Node* temp = edges->getHead();
                while (temp) {
                    Edge* edge = (Edge*)temp->data;
                    cout << edge->destination << "(" << edge->weight << " km) " << endl;
                    temp = temp->next;
                }
            }
            cout << endl;
        }
        delete[]keys;
    }
    void autoConnectNewNode(string newNodeId) {
        
        if (nodeCount <= 1) return;

        GraphNode* newNode = getNode(newNodeId);
        if (!newNode) return;

        int existingCount = 0;
        string* allIds = getAllNodeIDs(existingCount);

        if (!allIds || existingCount == 0) {
            if (allIds) delete[] allIds;
            return;
        }

        int connectionsMade = 0;
        double autoConnectRadius = 15.0; 

        for (int i = 0; i < existingCount; i++) {
            string existingId = allIds[i];

            if (existingId == newNodeId) continue;

            GraphNode* existingNode = getNode(existingId);
            if (!existingNode) continue;

            double distance = MathUtils::calculateDistance(
                newNode->latitude, newNode->longitude,
                existingNode->latitude, existingNode->longitude
            );

            if (distance > 0 && distance <= autoConnectRadius) {
               
                addEdge(newNodeId, existingId, distance);
                addEdge(existingId, newNodeId, distance);
                connectionsMade++;
            }
        }

        delete[] allIds;

        if (connectionsMade > 0) {
            cout << "  connected to " << connectionsMade << " nearby nodes." << endl;
        }
    }

    ~Graph() {
    }

    int getMaxNodes() {
        return maxNodes;
    }

    bool hasFreeSlots() {
        return nodeCount < maxNodes;
    }

    bool containsLocationId(string id) {
        return nodes.contains(id);
    }

    bool addLocation(string id, string name, double x, double y) {
        
        if (!hasFreeSlots() || nodes.contains(id)) return false;
        addNode(id, name, x, y);
        return true;
    }

    bool hasNonStopAtLocation(double x, double y) {

        int count = 0;
        string* keys = nodes.getAllKeys(count);
        if (!keys) return false;

        for (int i = 0; i < count; i++) {
            
            GraphNode* node = getNode(keys[i]);
            if (!node) continue;

            if (node->latitude == x && node->longitude == y) {
                if (!isStopId(node->id)) {
                    delete[]keys;
                    return true;
                }
            }
        }
        delete[]keys;
        return false;
    }

    bool isStopId(string id) {
        if (id.size() >= 5 && id.substr(0, 5) == "STOP_") return true;
        return false;
    }

};