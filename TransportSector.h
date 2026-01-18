#pragma once
#include "Graph.h"
#include "MedicalSector.h"
#include <string>
#include <iostream>
#include <ctime>
using namespace std;

class RouteNode {
private:
	string stopId;
	RouteNode* next;

public:
	RouteNode(string i = "") {
		stopId = i;
		next = nullptr;
	}

	string getId() {
		return stopId;
	}
	RouteNode* getNext() {
		return next;
	}

	void setId(string i) {
		stopId = i;
	}
	void setNext(RouteNode* ptr) {
		next = ptr;
	}
};
class Route {
private:
	RouteNode* head;
public:
	Route() {
		head = nullptr;
	}
	RouteNode* getHead() {
		return head;
	}

	void insert(string id) {

		if (id.empty()) return;
		RouteNode* newNode = new RouteNode(id);
		if (!head) {
			head = newNode;
			return;
		}

		RouteNode* temp = head;
		while (temp->getNext()) {
			temp = temp->getNext();
		}
		temp->setNext(newNode);
	}
	void display() {

		if (!head) {
			cout << "Route is empty for now" << endl;
			return;
		}

		RouteNode* temp = head;
		while (temp) {
			cout << temp->getId();
			temp = temp->getNext();
			if (temp) cout << "->";
		}
		cout << endl;
	}

	bool removeStop(string stopID) {
		if (!head)
			return false;

		if (head->getId() == stopID) {
			RouteNode* temp = head;
			head = head->getNext();
			delete temp;
			return true;
		}

		RouteNode* current = head;
		while (current->getNext() != nullptr) {
			if (current->getNext()->getId() == stopID) {
				RouteNode* temp = current->getNext();
				current->setNext(temp->getNext());
				delete temp;
				return true;
			}
			current = current->getNext();
		}

		return false;
	}
};

class Bus {
private:
	string number;
	string company;
	string currentStop;
	Route* currentRoute;
	bool isSchoolBus;
	string assignedSchool;
	string* residentialSectors;
	int sectorCount;
	int maxSectors;
public:
	Bus(string num = "", string comp = "", string stop = "") {
		number = num;
		company = comp;
		currentStop = stop;
		currentRoute = nullptr;
		isSchoolBus = false;
		assignedSchool = "";
		maxSectors = 10;
		sectorCount = 0;
		residentialSectors = new string[maxSectors];
	}
	bool getIsSchoolBus() { return isSchoolBus; }
	void setIsSchoolBus(bool val) { isSchoolBus = val; }

	string getAssignedSchool() { return assignedSchool; }
	void setAssignedSchool(string school) { assignedSchool = school; }

	void addResidentialSector(string sector) {
		if (sectorCount < maxSectors) {
			residentialSectors[sectorCount] = sector;
			sectorCount++;
		}
	}
	bool servesSector(string sector) {
		for (int i = 0; i < sectorCount; i++) {
			if (residentialSectors[i] == sector) {
				return true;
			}
		}
		return false;
	}

	void displaySchoolBusInfo() {
		if (!isSchoolBus) return;

		cout << "SCHOOL BUS: " << number << " (" << company << ")" << endl;
		cout << "Assigned School: " << (assignedSchool.empty() ? "Not assigned" : assignedSchool) << endl;
		cout << "Serves " << sectorCount << " residential sectors: ";
		for (int i = 0; i < sectorCount; i++) {
			cout << residentialSectors[i];
			if (i < sectorCount - 1) cout << ", ";
		}
		cout << endl;
	}

	void setNumber(string num) {
		if (!num.empty()) number = num;
	}
	void setCompany(string comp) {
		if (!comp.empty()) company = comp;
	}
	void setCurrentStop(string stop) {
		currentStop = stop;
	}
	void setCurrentRoute(Route* route) {
		currentRoute = route;
	}

	string getNumber() {
		return number;
	}
	string getCompany() {
		return company;
	}
	string getCurrentStop() {
		return currentStop;
	}
	Route* getCurrentRoute() {
		return currentRoute;
	}
	void updateSchoolBusStatus() {
		if (!currentRoute) {
			isSchoolBus = false;
			assignedSchool = "";
			return;
		}

		int schoolStopCount = 0;
		string schoolID = "";
		RouteNode* temp = currentRoute->getHead();

		while (temp != nullptr) {
			string stopId = temp->getId();
			if (stopId.find("SCH_") == 0) {
				schoolStopCount++;
				schoolID = stopId.substr(4);
			}
			temp = temp->getNext();
		}

		if (schoolStopCount == 1) {
			isSchoolBus = true;
			assignedSchool = schoolID;
		}
		else {
			isSchoolBus = false;
			assignedSchool = "";
		}
	}
};
class BusStop {
private:
	string id;
	string name;
	double x_cord;
	double y_cord;

public:
	BusStop(string i = "", string n = "", double x = 0.0, double y = 0.0) {
		id = i;
		name = n;
		x_cord = x;
		y_cord = y;
	}

	string getId() {
		return id;
	}
	string getName() {
		return name;
	}
	double getXcord() {
		return x_cord;
	}
	double getYcord() {
		return y_cord;
	}

	void setId(string i) {
		id = i;
	}
	void setName(string n) {
		name = n;
	}
	void setXcord(double x) {
		x_cord = x;
	}
	void setYcord(double y) {
		y_cord = y;
	}
};

class EdgeData {
private:
	BusStop* destination;
	int weight;

public:
	EdgeData(BusStop* stop = nullptr, int w = 0) {
		destination = stop;
		weight = w;
	}

	BusStop* getDestination() {
		return destination;
	}
	int getWeight() {
		return weight;
	}

	void setDestination(BusStop* stop) {
		destination = stop;
	}
	void setWeight(int w) {
		weight = w;
	}
};
class EdgesList {
private:
	EdgeData* edges;
	int maxEdges;
	int actualEdges;

public:
	EdgesList() {
		maxEdges = 1000;
		edges = new EdgeData[maxEdges];
		actualEdges = 0;
	}
	~EdgesList() {
		if (edges) delete[]edges; edges = nullptr;
	}

	void addEdge(BusStop* stop, int weight) {
		if (actualEdges >= maxEdges || !stop) return;
		edges[actualEdges] = EdgeData(stop, weight);
		actualEdges++;
	}

	int getEdges() {
		return actualEdges;
	}
	EdgeData* getEdge(int idx) {
		if (idx < 0 || idx >= actualEdges) return nullptr;
		return &edges[idx];
	}
};


class PassengerLocation {
private:
	string nearestStop;
	double x_cord;
	double y_cord;

public:
	PassengerLocation(string stop, double x, double y) {
		nearestStop = stop;
		x_cord = x;
		y_cord = y;
	}

	string getNearestStop() {
		return nearestStop;
	}
	double getXCord() {
		return x_cord;
	}
	double getYCord() {
		return y_cord;
	}

	void setNearestStop(string stop) {
		nearestStop = stop;
	}
	void setXCord(double x) {
		x_cord = x;
	}
	void setYCord(double y) {
		y_cord = y;
	}
};
class Passenger {
private:
	string name;
	string cnic;
	string destination;
	int ticketNumber;

public:
	Passenger(string n = "", string c = "", string d = "", int t = 0) {
		name = n;
		cnic = c;
		destination = d;
		ticketNumber = t;
	}

	string getName() {
		return name;
	}
	string getCnic() {
		return cnic;
	}
	string getDestination() {
		return destination;
	}
	int getTicketNumber() {
		return ticketNumber;
	}

	void setName(string name) {
		this->name = name;
	}
	void setCnic(string cnic) {
		this->cnic = cnic;
	}
	void setDestination(string destination) {
		this->destination = destination;
	}
	void setTicketNumber(int ticketNumber) {
		this->ticketNumber = ticketNumber;
	}

	void display() {
		cout << "(Passenger, CNIC, Ticket Number, Destination) = (" << name << ", " << cnic << ", "
			<< ticketNumber << ", " << destination << ")";
	}
};
class PassengerQueue {
private:
	Passenger* arr;
	int front;
	int rear;
	int capacity;
	int occupied;

public:
	PassengerQueue(int size = 50) {
		front = 0;
		rear = -1;
		occupied = 0;
		capacity = size;
		arr = new Passenger[capacity];
	}
	~PassengerQueue() {
		if (arr) delete[]arr; arr = nullptr;
	}

	bool isFull() {
		if (occupied == capacity) return true;
		return false;
	}
	bool isEmpty() {
		if (occupied == 0) return true;
		return false;
	}

	void enqueue(string name, string cnic, string destination, int ticketNumber) {

		if (isFull()) {
			cout << "Cannot add more passengers" << endl;
			return;
		}

		//Passenger* newNode = new Passenger(name, cnic, destination, ticketNumber);

		rear += 1;
		rear %= capacity;
		arr[rear] = Passenger(name, cnic, destination, ticketNumber);;

		arr[rear].display();
		cout << " added to the queue" << endl;
		occupied++;
	}
	void dequeue() {
		if (isEmpty()) {
			cout << "Cannot remove passengers" << endl;
			return;
		}

		Passenger removed = arr[front];
		front += 1;
		front %= capacity;
		occupied--;

		removed.display();
		cout << " removed from the queue" << endl;
	}

	int getFront() {
		return front;
	}
	int getRear() {
		return rear;
	}
	int getCapacity() {
		return capacity;
	}
	int getOccupied() {
		return occupied;
	}

	void display() {
		if (isEmpty()) {
			cout << "Queue is empty" << endl;
			return;
		}

		int idx = front;
		for (int i = 0; i < occupied; i++) {
			cout << "Position " << i + 1 << ": ";
			arr[idx].display();
			cout << endl;
			idx += 1;
			idx %= capacity;
		}
	}
	void clear() {
		front = 0;
		rear = -1;
		occupied = 0;
		cout << "Cleared the queue" << endl;
	}
};

class TravelRecord {
private:
	string busNumber;
	string from;
	string to;
	string time;
	double distance;

public:
	TravelRecord(string bN = "", string f = "", string t = "", string time = "", double d = 0) {
		busNumber = bN;
		from = f;
		to = t;
		this->time = time;
		distance = d;
	}

	string getBusNumber() {
		return busNumber;
	}
	string getFrom() {
		return from;
	}
	string getTo() {
		return to;
	}
	string getTime() {
		return time;
	}
	int getDistance() {
		return distance;
	}

	void setBusNumber(string busNumber) {
		this->busNumber = busNumber;
	}
	void setFrom(string from) {
		this->from = from;
	}
	void setTo(string to) {
		this->to = to;
	}
	void setTime(string time) {
		this->time = time;
	}
	void setDistance(int distance) {
		this->distance = distance;
	}

	void display() {
		cout << "(Bus Number, From Stop, To Stop, Time, Distance) = (" << busNumber << ", " << from << ", " << to
			<< ", " << time << ", " << distance << " km)" << endl;
	}
};
class TravelHistory {
private:
	TravelRecord* arr;
	int top;
	int capacity;

public:
	TravelHistory(int size = 100) {
		capacity = size;
		top = -1;
		arr = new TravelRecord[capacity];
	}
	~TravelHistory() {
		if (arr) delete[]arr; arr = nullptr;
	}

	bool isEmpty() {
		if (top == -1) return true;
		return false;
	}
	bool isFull() {
		if (top == capacity - 1) return true;
		return false;
	}
	void clear() {
		top = -1;
		cout << "Travel History cleared" << endl;
	}

	void push(string bN = "", string f = "", string t = "", string time = "", double d = 0) {

		if (isFull()) {
			cout << "Travel History is already full" << endl;
			return;
		}

		//TravelRecord* newRecord = new TravelRecord(bN, f, t, time, d);
		arr[++top] = TravelRecord(bN, f, t, time, d);;
	}
	void push(TravelRecord history) {
		if (isFull()) {
			cout << "Travel History is already full" << endl;
			return;
		}
		arr[++top] = history;
	}
	TravelRecord pop() {

		if (isEmpty()) {
			cout << "Travel History is currently empty" << endl;
			return TravelRecord();
		}
		return arr[top--];
	}

	int getOccupied() {
		return top + 1;
	}
	int getCapacity() {
		return capacity;
	}

	void display() {
		if (isEmpty()) {
			cout << "Nothing to display as travel history is empty" << endl;
			return;
		}

		cout << "Travel History:" << endl;
		TravelHistory* revHistory = new TravelHistory(capacity);
		while (!isEmpty()) revHistory->push(pop());

		while (!revHistory->isEmpty()) {
			TravelRecord record = revHistory->pop();
			record.display();
			push(record);
		}
		delete revHistory;
		cout << endl;
	}
};

class BusHashNode {
private:
	string number;
	Bus* bus;
	BusHashNode* next;

public:
	BusHashNode(string num, Bus* b) {
		number = num;
		bus = b;
		next = nullptr;
	}

	string getNumber() {
		return number;
	}
	Bus* getBus() {
		return bus;
	}
	BusHashNode* getNext() {
		return next;
	}

	void setNumber(string num) {
		number = num;
	}
	void setBus(Bus* bus) {
		this->bus = bus;
	}
	void setNext(BusHashNode* node) {
		next = node;
	}
};
class BusHashTable {
private:
	BusHashNode** table;
	int maxNodes;
	int actualNodes;

	int hashFunction(string key) {
		if (key.empty()) return 0;
		unsigned long hash = 0;
		int p = 31;
		int p_pow = 1;

		for (int i = 0; i < key.length(); i++) {
			hash = (hash + (unsigned char)key[i] * p_pow) % maxNodes;
			p_pow = (p_pow * p) % maxNodes;
		}
		return int(hash);
	}

public:
	BusHashTable() {
		maxNodes = 1000;
		table = new BusHashNode * [maxNodes];
		actualNodes = 0;

		for (int i = 0; i < maxNodes; i++) {
			table[i] = nullptr;
		}
	}
	~BusHashTable() {
		for (int i = 0; i < maxNodes; i++) {
			BusHashNode* curr = table[i];
			while (curr) {
				BusHashNode* temp = curr;
				curr = curr->getNext();
				delete temp;
			}
		}
		delete[]table;
	}

	void insertBus(Bus* bus) {

		if (!bus) return;

		string num = bus->getNumber();
		if (num.empty()) return;
		if (searchBus(num)) return;

		int idx = hashFunction(num);
		BusHashNode* newNode = new BusHashNode(num, bus);

		newNode->setNext(table[idx]);
		table[idx] = newNode;
		actualNodes++;
	}
	Bus* searchBus(string number) {

		if (number.empty()) return nullptr;

		int idx = hashFunction(number);
		BusHashNode* curr = table[idx];
		while (curr) {
			if (curr->getBus()->getNumber() == number) return curr->getBus();
			curr = curr->getNext();
		}
		return nullptr;
	}

	int getNodesCount() {
		return actualNodes;
	}
	void display() {
		cout << "Hash Table Data:" << endl;
		cout << "Total Buses: " << actualNodes << endl;
		cout << "Total Space: " << maxNodes << endl;

		int maxChain = 0;
		int empty = 0;

		for (int i = 0; i < maxNodes; i++) {
			int chainLen = 0;
			BusHashNode* curr = table[i];
			while (curr) {
				chainLen++;
				curr = curr->getNext();
			}
			if (chainLen == 0) empty++;
			if (chainLen > maxChain) maxChain = chainLen;
		}

		cout << "Max Chain: " << maxChain << endl;
		cout << "Empty Slots: " << empty << endl;
	}
};

class BusLocationTracker {
private:
	//TransportGraph* graph;
	Graph* graph;
	Bus** buses;
	int maxBuses;
	int currentBus;

public:

	BusLocationTracker(Graph* graph) {
		this->graph = graph;
		maxBuses = 1000;
		currentBus = 0;
		buses = new Bus * [maxBuses];
		for (int i = 0; i < maxBuses; i++) {
			buses[i] = nullptr;
		}
	}
	~BusLocationTracker() {
		delete[]buses;
	}

	void addBus(Bus* bus) {
		if (!bus || currentBus >= maxBuses) return;
		buses[currentBus] = bus;
		currentBus++;
	}
	string findNearest(string stop) {

		if (stop.empty()) return "";
		if (!graph->nodeExists(stop)) return "";

		string nearest = "";
		double minDistance = pow(10, 18);

		for (int i = 0; i < currentBus; i++) {

			if (!buses[i] || buses[i]->getCurrentStop().empty()) continue;

			double lenght = -1;
			string* path = graph->dijkstra(buses[i]->getCurrentStop(), stop, lenght);
			if (path) delete[] path;

			if (lenght >= 0 && lenght < minDistance) {
				minDistance = lenght;
				nearest = buses[i]->getNumber();
			}
		}
		return nearest;
	}
};

class TransportSector {
private:
	string company;
	//TransportGraph* graph;
	Graph* graph;
	Bus** buses;
	Route** routes;
	int maxBuses;
	int currentBus;
	int maxRoutes;
	int currentRoute;
	BusHashTable* busTable;
	BusLocationTracker* busTracker;
	PassengerQueue** passengerQueue;
	TravelHistory* travelHistory;
	int nextTicketNumber;

	Bus** schoolBuses;
	int schoolBusCount;
	int maxSchoolBuses;

	bool isReUsed(double x, double y) {

		if (!graph) return false;
		int count = 0;
		string* ids = graph->getAllNodeIDs(count);
		if (!ids) return false;

		for (int i = 0; i < count; i++) {

			GraphNode* node = graph->getNode(ids[i]);
			if (!node) continue;

			double dx = node->latitude - x;
			double dy = node->longitude - y;

			if (dx == 0 && dy == 0) {
				delete[]ids;
				return graph->isStopId(node->id);
			}
		}
		delete[]ids;
		return false;
	}

public:
	TransportSector(Graph* graph, string c = "Default Transport") {

		this->graph = graph;
		company = c; nextTicketNumber = 1000;

		maxBuses = 1000; currentBus = 0;
		buses = new Bus * [maxBuses];
		for (int i = 0; i < maxBuses; i++) buses[i] = nullptr;

		maxRoutes = 1000; currentRoute = 0;
		routes = new Route * [maxRoutes];
		for (int i = 0; i < maxRoutes; i++) routes[i] = nullptr;

		passengerQueue = new PassengerQueue * [maxBuses];
		for (int i = 0; i < maxBuses; i++) passengerQueue[i] = nullptr;

		busTracker = new BusLocationTracker(graph);
		busTable = new BusHashTable;
		travelHistory = new TravelHistory(500);

		maxSchoolBuses = 100;
		schoolBusCount = 0;
		schoolBuses = new Bus * [maxSchoolBuses];
		for (int i = 0; i < maxSchoolBuses; i++) {
			schoolBuses[i] = nullptr;
		}
	}
	~TransportSector() {
		for (int i = 0; i < currentBus; i++) delete buses[i]; delete[]buses;
		for (int i = 0; i < currentRoute; i++) delete routes[i]; delete[]routes;
		for (int i = 0; i < maxBuses; i++) delete passengerQueue[i]; delete[]passengerQueue;
		delete busTable;
		delete busTracker;
		delete travelHistory;

		delete[]schoolBuses;
	}
	bool loadBusStops(const string& filename) {
		ifstream file(filename.c_str());
		if (!file.is_open()) {
			cout << "Error: Could not open " << filename << endl;
			return false;
		}

		string line;
		getline(file, line);

		int loadCount = 0;
		int lineNum = 1;

		while (getline(file, line)) {
			lineNum++;
			if (line.empty()) continue;

			int firstComma = -1;
			int secondComma = -1;
			bool inQuotes = false;

			for (int i = 0; i < line.length(); i++) {
				if (line[i] == '"') {
					inQuotes = !inQuotes;
				}
				if (line[i] == ',' && !inQuotes) {
					if (firstComma == -1) {
						firstComma = i;
					}
					else if (secondComma == -1) {
						secondComma = i;
						break;
					}
				}
			}
			if (firstComma == -1 || secondComma == -1) {
				cout << "Warning: Invalid format at line " << lineNum << endl;
				continue;
			}

			string stopID = StringUtils::trim(line.substr(0, firstComma));
			string stopName = StringUtils::trim(line.substr(firstComma + 1, secondComma - firstComma - 1));
			string coordStr = StringUtils::trim(line.substr(secondComma + 1));

			coordStr = StringUtils::removeQuotes(coordStr);

			int commaPos = -1;
			for (int i = 0; i < coordStr.length(); i++) {
				if (coordStr[i] == ',') {
					commaPos = i;
					break;
				}
			}

			if (commaPos == -1) {
				cout << "Warning: Invalid coordinates at line " << lineNum << endl;
				continue;
			}

			string latStr = StringUtils::trim(coordStr.substr(0, commaPos));
			string lonStr = StringUtils::trim(coordStr.substr(commaPos + 1));

			double latitude = StringUtils::toDouble(latStr);
			double longitude = StringUtils::toDouble(lonStr);

			if (!InputValidator::isValidString(stopID) || !InputValidator::isValidString(stopName)) {
				cout << "Warning: Invalid stop data at line " << lineNum << endl;
				continue;
			}

			if (!InputValidator::isValidLatitude(latitude) || !InputValidator::isValidLongitude(longitude)) {
				cout << "Warning: Invalid coordinates at line " << lineNum << endl;
				continue;
			}

			if (graph->nodeExists(stopID)) {
				cout << "Warning: Stop " << stopID << " already exists (line " << lineNum << ")" << endl;
				continue;
			}

			addStop(stopID, stopName, latitude, longitude);
			loadCount++;
		}

		file.close();
		cout << "Loaded " << loadCount << " bus stops from " << filename << endl;
		return loadCount > 0;
	}
	bool loadBuses(const string& filename) {
		ifstream file(filename.c_str());
		if (!file.is_open()) {
			cout << "Error: Could not open " << filename << endl;
			return false;
		}

		string line;
		getline(file, line);

		int loadCount = 0;
		int lineNum = 1;

		while (getline(file, line)) {
			lineNum++;
			if (line.empty()) continue;

			int comma1 = -1, comma2 = -1, comma3 = -1;
			int commaCount = 0;

			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ',') {
					commaCount++;
					if (commaCount == 1) comma1 = i;
					else if (commaCount == 2) comma2 = i;
					else if (commaCount == 3) comma3 = i;
				}
			}

			if (comma3 == -1) {
				cout << "Warning: Invalid bus data at line " << lineNum << endl;
				continue;
			}

			string busNo = StringUtils::trim(line.substr(0, comma1));
			string company = StringUtils::trim(line.substr(comma1 + 1, comma2 - comma1 - 1));
			string currentStop = StringUtils::trim(line.substr(comma2 + 1, comma3 - comma2 - 1));
			string routeStr = StringUtils::trim(line.substr(comma3 + 1));

			if (!InputValidator::isValidString(busNo) || !InputValidator::isValidString(company)) {
				cout << "Warning: Invalid bus data at line " << lineNum << endl;
				continue;
			}

			if (!graph->nodeExists(currentStop)) {
				cout << "Warning: Current stop " << currentStop << " does not exist for bus "
					<< busNo << " (line " << lineNum << ")" << endl;
				continue;
			}

			if (busTable->searchBus(busNo)) {
				cout << "Warning: Bus " << busNo << " already exists (line " << lineNum << ")" << endl;
				continue;
			}
			string routeForParsing = "";
			for (int i = 0; i < routeStr.length(); i++) {
				if (i + 2 < routeStr.length() &&
					routeStr[i] == ' ' && routeStr[i + 1] == '>' && routeStr[i + 2] == ' ') {
					routeForParsing += '|';
					i += 2;
				}
				else {
					routeForParsing += routeStr[i];
				}
			}

			int stopCount = 0;
			string* stops = StringUtils::split(routeForParsing, '|', stopCount);

			if (!stops || stopCount == 0) {
				cout << "Warning: Invalid route format for bus " << busNo << " (line " << lineNum << ")" << endl;
				continue;
			}
			bool allStopsValid = true;
			for (int i = 0; i < stopCount; i++) {
				stops[i] = StringUtils::trim(stops[i]);
				if (!graph->nodeExists(stops[i])) {
					cout << "Warning: Stop " << stops[i] << " in route does not exist for bus "
						<< busNo << " (line " << lineNum << ")" << endl;
					allStopsValid = false;
					break;
				}
			}

			if (!allStopsValid) {
				delete[] stops;
				continue;
			}

			Route* route = createRoute(stops, stopCount);

			addBus(busNo, company, route, currentStop);
			loadCount++;

			delete[] stops;
		}

		file.close();
		cout << "Loaded " << loadCount << " buses from " << filename << endl;
		return loadCount > 0;
	}

	void addBus(string number, string comp, Route* route, string stop = "") {
		if (number.empty() || currentBus >= maxBuses) return;

		Bus* newBus = new Bus(number, comp, stop);

		// Detect if it's a school bus
		bool isSchoolBus = detectSchoolBus(route, comp);
		newBus->setIsSchoolBus(isSchoolBus);

		if (isSchoolBus) {
			cout << "\n=== SETTING UP SCHOOL BUS " << number << " ===" << endl;

			string assignedSchool = findSchoolFromRoute(route);
			if (!assignedSchool.empty()) {
				newBus->setAssignedSchool(assignedSchool);
				cout << "Assigned School: " << assignedSchool << endl;
			}
			else {

				string* sectors = new string[50];
				int sectorCount = 0;

				RouteNode* current = route->getHead();
				while (current && sectorCount < 50) {
					string sector = extractSectorFromStop(current->getId());
					if (!sector.empty()) {
						bool exists = false;
						for (int i = 0; i < sectorCount; i++) {
							if (sectors[i] == sector) {
								exists = true;
								break;
							}
						}
						if (!exists) {
							sectors[sectorCount] = sector;
							sectorCount++;
						}
					}
					current = current->getNext();
				}

				if (sectorCount > 0) {
					string schoolName = "Islamabad Model School for ";
					for (int i = 0; i < sectorCount; i++) {
						schoolName += sectors[i];
						if (i < sectorCount - 1) schoolName += ", ";
					}
					newBus->setAssignedSchool(schoolName);
					cout << "Assigned School: " << schoolName << endl;
				}

				delete[] sectors;
			}

			findResidentialSectors(newBus, route);

			// Add to school buses array
			if (schoolBusCount < maxSchoolBuses) {
				schoolBuses[schoolBusCount] = newBus;
				schoolBusCount++;
				cout << "Added to school buses list" << endl;
			}

			cout << "School Bus setup complete!" << endl;
		}

		newBus->setCurrentRoute(route);
		buses[currentBus] = newBus;
		busTable->insertBus(newBus);
		busTracker->addBus(newBus);
		currentBus++;

		cout << "Bus " << number << " added successfully" << endl;
	}

	bool detectSchoolBus(Route* route, string company) {
		if (!route) return false;

		cout << "\n=== Checking if bus is school bus ===" << endl;
		cout << "Company: " << company << endl;

		RouteNode* current = route->getHead();
		int schoolStopCount = 0;
		int houseStopCount = 0;
		int totcount = 0;
		RouteNode* temp = current;
		while (temp) {
			totcount++;
			temp = temp->getNext();
		}

		cout << "Checking route stops:" << endl;
		while (current) {
			string stopID = current->getId();
			GraphNode* stopNode = graph->getNode(stopID);
			string stopName = stopNode ? stopNode->name : stopID;

			cout << "  Stop: " << stopID << " (" << stopName << ")";

			// Check for school stops (SCH_ prefix)
			if (stopID.length() >= 4 && stopID.substr(0, 4) == "SCH_") {
				cout << "  SCHOOL STOP";
				schoolStopCount++;
			}
			// Check for house stops (HOU_ prefix)
			else if (stopID.length() >= 4 && stopID.substr(0, 4) == "HOU_") {
				cout << "  HOUSE STOP";
				houseStopCount++;
			}

			cout << endl;
			current = current->getNext();
		}

		// If route has at least 1 school stop, it's a school bus
		//if (schoolStopCount > 0) {
		//	cout << " Detected as school bus (serves " << schoolStopCount << " school stops)" << endl;
		//	return true;
		//}

		//// If route has 2 or more house stops, likely a school bus
		//if (houseStopCount >= 1) {
		//	cout << " Detected as school bus (serves " << houseStopCount << " residential houses)" << endl;
		//	return true;
		//}

		int totalschoolct = schoolStopCount + houseStopCount;
		if ((schoolStopCount > 0 && houseStopCount > 0) && (totalschoolct == totcount)) {
			cout << " Detected as school bus (serves " << schoolStopCount << " school stops and "
				<< houseStopCount << " residential houses)" << endl;
			return true;
		}
		cout << " Not a school bus" << endl;
		return false;
	}

	string findSchoolFromRoute(Route* route) {
		if (!route) return "";

		RouteNode* current = route->getHead();
		while (current) {
			string stopID = current->getId();

			if (stopID.length() >= 4 && stopID.substr(0, 4) == "SCH_") {
				// Extract school name from stop name
				GraphNode* stopNode = graph->getNode(stopID);
				if (stopNode) {
					string stopName = stopNode->name;

					if (stopName.find("School Stop") != string::npos) {
						return stopName.substr(0, stopName.find("School Stop"));
					}
					return stopName;
				}


				return stopID.substr(4);
			}

			current = current->getNext();
		}

		return "";
	}

	void findResidentialSectors(Bus* bus, Route* route) {
		if (!bus || !route || !graph) return;

		RouteNode* current = route->getHead();
		const int MAX_SECTORS = 50;
		string sectors[MAX_SECTORS];
		int sectorCount = 0;

		cout << "\nFinding residential sectors for bus " << bus->getNumber() << ":" << endl;

		while (current && sectorCount < MAX_SECTORS) {
			string stopID = current->getId();

			if (stopID.length() >= 4 && stopID.substr(0, 4) == "HOU_") {

				string remaining = stopID.substr(4);

				int underscorePos = -1;
				for (int i = 0; i < remaining.length(); i++) {
					if (remaining[i] == '_') {
						underscorePos = i;
						break;
					}
				}

				if (underscorePos > 0) {
					string sector = remaining.substr(0, underscorePos);

					bool exists = false;
					for (int i = 0; i < sectorCount; i++) {
						if (sectors[i] == sector) {
							exists = true;
							break;
						}
					}

					if (!exists) {
						sectors[sectorCount] = sector;
						sectorCount++;
						bus->addResidentialSector(sector);
						cout << "  Added sector: " << sector << " from stop: " << stopID << endl;
					}
				}
			}

			current = current->getNext();
		}

		cout << "Total residential sectors found for bus " << bus->getNumber() << ": " << sectorCount << endl;
	}

	string extractSectorFromStop(string stopID) {
		if (!graph) return "";

		GraphNode* node = graph->getNode(stopID);
		if (!node) return "";

		string name = node->name;

		if (stopID.length() >= 4 && stopID.substr(0, 4) == "HOU_") {

			string remaining = stopID.substr(4); // Remove "HOU_"

			int underscorePos = -1;
			for (int i = 0; i < remaining.length(); i++) {
				if (remaining[i] == '_') {
					underscorePos = i;
					break;
				}
			}

			if (underscorePos > 0) {
				string sector = remaining.substr(0, underscorePos);
				cout << "  Extracted sector from HOU stop: " << stopID << " -> " << sector << endl;
				return sector;
			}

			return ""; // No sector found in pattern
		}


		if (stopID.length() >= 4 && stopID.substr(0, 4) == "SCH_") {
			return "School Area";
		}


		return ""; // No sector found
	}

	void displaySchoolBuses() {
		cout << "\n=== SCHOOL BUS TRACKING SYSTEM ===" << endl;
		cout << "Total School Buses: " << schoolBusCount << endl;
		cout << "===================================" << endl;

		if (schoolBusCount == 0) {
			cout << "No school buses registered." << endl;
			return;
		}

		for (int i = 0; i < schoolBusCount; i++) {
			if (schoolBuses[i]) {
				cout << "\n[" << (i + 1) << "] ";
				schoolBuses[i]->displaySchoolBusInfo();

				cout << "Current Location: "
					<< (schoolBuses[i]->getCurrentStop().empty() ? "Not available" : schoolBuses[i]->getCurrentStop()) << endl;

				// Show route
				Route* route = schoolBuses[i]->getCurrentRoute();
				if (route) {
					cout << "Route: ";
					route->display();
				}
				cout << "-----------------------------------" << endl;
			}
		}
	}

	Bus* findSchoolBusBySchool(string schoolID) {
		for (int i = 0; i < schoolBusCount; i++) {
			if (schoolBuses[i] && schoolBuses[i]->getAssignedSchool() == schoolID) {
				return schoolBuses[i];
			}
		}
		return nullptr;
	}

	void findSchoolBusesBySector(string sector) {
		cout << "\n=== SCHOOL BUSES SERVING SECTOR " << sector << " ===" << endl;
		int found = 0;

		for (int i = 0; i < schoolBusCount; i++) {
			if (schoolBuses[i] && schoolBuses[i]->servesSector(sector)) {
				found++;
				cout << "\n[" << found << "] ";
				schoolBuses[i]->displaySchoolBusInfo();
			}
		}

		if (found == 0) {
			cout << "No school buses serve sector " << sector << endl;
		}
		else {
			cout << "\nTotal: " << found << " school bus(es) found." << endl;
		}
	}

	void trackSchoolBusJourney(string busNumber) {
		Bus* bus = busTable->searchBus(busNumber);
		if (!bus) {
			cout << "Bus " << busNumber << " not found!" << endl;
			return;
		}

		if (!bus->getIsSchoolBus()) {
			cout << "Bus " << busNumber << " is not a school bus!" << endl;
			return;
		}

		cout << "\n=== SCHOOL BUS JOURNEY TRACKING ===" << endl;
		cout << "Bus: " << busNumber << endl;
		cout << "School: " << (bus->getAssignedSchool().empty() ? "Not assigned" : bus->getAssignedSchool()) << endl;
		cout << "====================================" << endl;

		Route* route = bus->getCurrentRoute();
		if (!route || !route->getHead()) {
			cout << "No route assigned to this bus!" << endl;
			return;
		}

		// Simulate the journey
		RouteNode* currentStop = route->getHead();
		int stopNumber = 1;

		cout << "\nStarting journey simulation..." << endl;
		cout << "Press Enter to move to next stop (or 'q' to quit):" << endl;

		// Clear input buffer
		cin.ignore(1000, '\n');

		while (currentStop) {
			string stopID = currentStop->getId();
			GraphNode* stopNode = graph->getNode(stopID);
			string stopName = stopNode ? stopNode->name : stopID;

			// Update bus location
			bus->setCurrentStop(stopID);

			cout << "\n[" << stopNumber << "] CURRENT STOP: " << stopID;
			cout << " (" << stopName << ")" << endl;

			// Check if this is a residential area
			string sector = extractSectorFromStop(stopID);
			if (!sector.empty()) {
				cout << "    RESIDENTIAL AREA: Sector " << sector << " - Student pickup/dropoff" << endl;
			}

			// Check if stop name has school-related keywords
			string nameLower = stopName;
			for (int i = 0; i < nameLower.length(); i++) {
				if (nameLower[i] >= 'A' && nameLower[i] <= 'Z') {
					nameLower[i] = nameLower[i] + ('a' - 'A');
				}
			}

			if (nameLower.find("school") != string::npos ||
				nameLower.find("college") != string::npos) {
				cout << "    SCHOOL STOP - Students getting on/off" << endl;
			}

			// Show next stop
			if (currentStop->getNext()) {
				string nextStopID = currentStop->getNext()->getId();
				GraphNode* nextStopNode = graph->getNode(nextStopID);
				string nextStopName = nextStopNode ? nextStopNode->name : nextStopID;
				cout << "   Next: " << nextStopID << " (" << nextStopName << ")" << endl;
			}
			else {
				cout << "    END OF ROUTE" << endl;
			}

			// Wait for user input
			cout << "\nPress Enter to continue...";
			string input;
			getline(cin, input);

			if (input == "q" || input == "Q") {
				cout << "Journey simulation stopped." << endl;
				break;
			}

			currentStop = currentStop->getNext();
			stopNumber++;
		}

		cout << "\n=== JOURNEY COMPLETED ===" << endl;

		// Record the travel
		if (route->getHead()) {
			string firstStop = route->getHead()->getId();
			string lastStop = getLastStopID(route);
			double dist = -1;
			string* path = graph->dijkstra(firstStop, lastStop, dist);// shortestPath(from, to, dist);

			if (!path || dist < 0) {
				cout << "Cannot move forward as no path found" << endl;
				if (path) delete[]path;
				return;
			}

			long timeStamp = time(0);
			string timeStr = format(timeStamp);

			recordTravel(busNumber, firstStop, lastStop, timeStr, dist);
		}
	}

	string getLastStopID(Route* route) {
		if (!route || !route->getHead()) return "";

		RouteNode* current = route->getHead();
		while (current->getNext()) {
			current = current->getNext();
		}
		return current->getId();
	}

	void addStop(string id, string name, double x, double y) {

		if (!graph || id.empty() || name.empty()) return;

		if (graph->nodeExists(id)) {
			cout << "Stop " << id << " already exists" << endl;
			return;
		}

		if (isReUsed(x, y)) {
			cout << "A stop already exists at (" << x << ", " << y << ")" << endl;
			return;
		}

		graph->addNode(id, name, x, y);
		cout << "Stop " << id << " (" << name << ") added" << endl;
	}

	void connectStops(string stop1, string stop2, double dist, bool undirected = true) {

		if (!graph || dist < 0) return;
		if (!graph->nodeExists(stop1) || !graph->nodeExists(stop2)) {
			cout << "One of the stops do not exist" << endl;
			return;
		}
		if (undirected) {
			graph->addUndirectedEdge(stop1, stop2, dist);
			cout << "Connected " << stop1 << " <-> " << stop2 << " (bidirectional)" << endl;
		}
		else {
			graph->addEdge(stop1, stop2, dist);
			cout << "Connected " << stop1 << " -> " << stop2 << " (directional)" << endl;
		}
	}
	void updateBusLocation(string number, string stop) {
		Bus* bus = busTable->searchBus(number);
		if (!bus || !graph->nodeExists(stop)) return;
		bus->setCurrentStop(stop);
		cout << "Bus " << number << " location updated to " << stop << endl;
	}

	string getNearestBus(string stop) {
		return busTracker->findNearest(stop);
	}
	string getShortestPath(string one, string two) {

		if (!graph) return "";

		double lenth = -1;
		string* path = graph->dijkstra(one, two, lenth);

		if (!path || lenth < 0) {
			cout << "No path found between " << one << " and " << two << endl;
			if (path) delete[]path;
			return "";
		}

		cout << "Shortest Path from " << one << " to " << two << ": ";
		string pathStr = "";
		for (int i = 0; i < 100 && !path[i].empty(); i++) {
			cout << path[i];
			pathStr += path[i];
			if (!path[i + 1].empty()) {
				cout << " ->";
				pathStr += " ->";
			}
		}
		cout << endl << "Total Distance: " << lenth << " km" << endl;
		delete[]path;
		return pathStr;
	}

	Bus* getBusDetails(string number) {

		Bus* bus = busTable->searchBus(number);

		if (!bus) return nullptr;
		cout << "Bus Details:" << endl;
		cout << "(Bus Number, Bus Company, Current Stop) = (" << bus->getNumber() << ", " << bus->getCompany() << ", "
			<< (bus->getCurrentStop().empty() ? "Not assigned" : bus->getCurrentStop()) << endl;

		if (bus->getCurrentRoute()) {
			cout << "Route: ";
			bus->getCurrentRoute()->display();
		}
		return bus;
	}

	int getStopsCount() {
		return graph->getNodeCount();
	}
	GraphNode* getStopAt(int idx) {

		if (!graph) return nullptr;

		int count = 0;
		string* ids = graph->getAllNodeIDs(count);
		if (idx < 0 || idx >= count) {
			delete[]ids;
			return nullptr;
		}

		GraphNode* node = graph->getNode(ids[idx]);
		delete[]ids;
		return node;
	}

	void displayBuses() {

		cout << "All Buses: " << endl;
		if (currentBus == 0) {
			cout << "No buses registered yet" << endl;
			return;
		}

		for (int i = 0; i < currentBus; i++) {
			cout << "(Bus, Bus Number, Bus Company, Bus Stop) = (" << i << ", " << buses[i]->getNumber() << ", "
				<< buses[i]->getCompany() << ", " << buses[i]->getCurrentStop() << ")" << endl;
		}
	}
	void displayGraph() {
		if (graph) graph->display();
		else cout << "No graph exists" << endl;
	}
	void displayHashState() {
		busTable->display();
	}

	Route* createRoute(string* stops, int count) {
		if (count <= 0 || currentRoute >= maxRoutes) return nullptr;
		Route* newRoute = new Route;
		for (int i = 0; i < count; i++) newRoute->insert(stops[i]);
		routes[currentRoute++] = newRoute;
		return newRoute;
	}

	void initializeQueue(string stopId, int cap = 50) {

		int stopIdx = -1;
		for (int i = 0; i < currentBus; i++) {
			if (!passengerQueue[i]) {
				stopIdx = i;
				break;
			}
		}

		if (stopIdx != -1 && !passengerQueue[stopIdx]) {
			passengerQueue[stopIdx] = new PassengerQueue(cap);
			cout << "Passenger Queue created for stop " << stopId << " (Capacity: " << cap << ")" << endl;
		}

	}
	void addPassenger(string stopId, string name, string cnic, string destination) {

		if (!graph->nodeExists(stopId)) {
			cout << "Stop " << stopId << " does not exists" << endl;
			return;
		}

		if (!passengerQueue[0]) {
			passengerQueue[0] = new PassengerQueue(50);
		}

		passengerQueue[0]->enqueue(name, cnic, destination, nextTicketNumber++);
	}
	void boardPassengers(string stopId, int num = 1) {

		if (!graph->nodeExists(stopId)) {
			cout << "Stop " << stopId << " does not exists" << endl;
			return;
		}

		if (!passengerQueue[0]) {
			cout << "No passenger queue at this stop" << endl;
			return;
		}

		cout << "Boarding " << num << " Passengers" << endl;
		for (int i = 0; i < num; i++) {
			if (passengerQueue[0]->isEmpty()) {
				cout << "No more waiting passengers" << endl;
				break;
			}
			passengerQueue[0]->dequeue();
		}
	}
	void displayPassengersQueue(string stop) {
		if (!graph->nodeExists(stop)) {
			cout << "Stop " << stop << " does not exists" << endl;
			return;
		}

		if (!passengerQueue[0]) {
			cout << "No passenger queue at this stop" << endl;
			return;
		}

		passengerQueue[0]->display();
	}
	int getQueueSize(string stop) {
		if (!passengerQueue[0]) return 0;
		return passengerQueue[0]->getCapacity();
	}

	void recordTravel(string busNumber, string from, string to, string time, double dist) {
		//TravelRecord* newRecord = new TravelRecord(busNumber, from, to, time, dist);
		travelHistory->push(busNumber, from, to, time, dist);
		cout << "Travel recorded in history" << endl;
	}
	void displayTravelHistory() {
		travelHistory->display();
	}
	void removeLastTravel() {
		if (travelHistory->isEmpty()) {
			cout << "Travel History is empty" << endl;
			return;
		}

		TravelRecord lastTravel = travelHistory->pop();
		cout << "Last Travel: ";
		lastTravel.display();
	}
	void clearTravelHistory() {
		travelHistory->clear();
	}

	string format(long timestamp) {

		timestamp += 5 * 3600; //for GMT+5

		long rSec = 0;
		long days = timestamp / 86400;
		rSec = timestamp % 86400;;

		long hours = rSec / 3600;
		rSec = rSec % 3600;

		long minutes = rSec / 60;
		long seconds = rSec % 60;

		long year = 1970;
		long rDays = days;
		while (true) {

			long daysInYear = 365;
			if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) daysInYear++;

			if (rDays >= daysInYear) {
				rDays -= daysInYear;
				year++;
			}
			else break;
		}

		long daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) daysInMonth[1]++;

		long month = 1;
		long day = rDays + 1;
		for (int i = 0; i < 12; i++) {
			if (day <= daysInMonth[i]) {
				month = i + 1;
				break;
			}
			day -= daysInMonth[i];
		}

		string result = addZeroes(hours) + ":" + addZeroes(minutes) + ":" + addZeroes(seconds);
		return result;
	}
	string longToString(long n) {

		if (n == 0) return "0";
		string result = "";
		while (n > 0) {
			char d = '0' + (n % 10);
			result = d + result;
			n /= 10;
		}
		return result;
	}
	string addZeroes(long n) {
		if (n < 10) return "0" + longToString(n);
		return longToString(n);
	}

	void journeySimulation(string number, string from, string to) {
		Bus* bus = busTable->searchBus(number);
		if (!bus) {
			cout << "Bus " << number << " doesn't exist" << endl;
			return;
		}

		if (!graph) {
			cout << "No graph available for simulation" << endl;
			return;
		}

		double dist = -1;
		string* path = graph->dijkstra(from, to, dist);// shortestPath(from, to, dist);

		if (!path || dist < 0) {
			cout << "Cannot move forward as no path found" << endl;
			if (path) delete[]path;
			return;
		}

		cout << "Journey Simulation" << endl;
		cout << "Bus " << number << " route: ";

		string pathStr = "";
		for (int i = 0; i < 100 && !path[i].empty(); i++) {
			cout << path[i];
			pathStr += path[i];
			if (!path[i + 1].empty()) {
				cout << " -> ";
				pathStr += " -> ";
			}
		}
		cout << endl << "Total Distance: " << dist << " km" << endl;
		long timeStamp = time(0);
		string timeStr = format(timeStamp);

		recordTravel(number, from, to, timeStr, dist);

		bus->setCurrentStop(to);
		cout << "Bus is now at " << to << " stop" << endl;
		delete[]path;
	}
	void realTimeRouteSimulation(string busNumber) {
		Bus* bus = busTable->searchBus(busNumber);
		if (!bus) {
			cout << "Bus " << busNumber << " not found!" << endl;
			return;
		}

		Route* route = bus->getCurrentRoute();
		if (!route || !route->getHead()) {
			cout << "Bus " << busNumber << " has no assigned route!" << endl;
			return;
		}

		cout << "========================================" << endl;
		cout << "  REAL-TIME ROUTE SIMULATION" << endl;
		cout << "========================================" << endl;
		cout << "Bus: " << bus->getNumber() << " (" << bus->getCompany() << ")" << endl;
		cout << "Starting simulation..." << endl;
		cout << "========================================" << endl;

		RouteNode* currentStop = route->getHead();
		int stopNumber = 1;
		time_t startTime = time(0);

		while (currentStop) {
			time_t currentTime = time(0);
			double elapsed = difftime(currentTime, startTime);

			cout << "\n[" << elapsed << "s] Stop " << stopNumber << ": "
				<< currentStop->getId();

			GraphNode* stopNode = graph->getNode(currentStop->getId());
			if (stopNode) {
				cout << " (" << stopNode->name << ")";
			}

			bus->setCurrentStop(currentStop->getId());

			if (passengerQueue[0] && !passengerQueue[0]->isEmpty()) {
				cout << " - [" << passengerQueue[0]->getOccupied()
					<< " passengers waiting]";
			}

			cout << endl;

			currentStop = currentStop->getNext();

			if (currentStop) {
				cout << "   Moving to next stop";
				for (int i = 0; i < 3; i++) {
					cout << ".";
					cout.flush();
					time_t waitStart = time(0);
					while (difftime(time(0), waitStart) < 2.0) {

					}
				}
				cout << endl;
			}

			stopNumber++;
		}

		time_t endTime = time(0);
		double totalTime = difftime(endTime, startTime);

		cout << "\n========================================" << endl;
		cout << "  SIMULATION COMPLETE" << endl;
		cout << "========================================" << endl;
		cout << "Total stops: " << (stopNumber - 1) << endl;
		cout << "Total time: " << totalTime << " seconds" << endl;
		cout << "Bus " << bus->getNumber() << " has completed its route!" << endl;
		cout << "========================================" << endl;

		if (route->getHead()) {
			RouteNode* firstStop = route->getHead();
			RouteNode* lastStop = route->getHead();
			while (lastStop->getNext()) {
				lastStop = lastStop->getNext();
			}

			recordTravel(bus->getNumber(), firstStop->getId(),
				lastStop->getId(), "Real-time simulation",
				(int)totalTime);
		}
	}
	void emergencyRouteToHospital(string currentStopID, MedicalSystem* medicalSystem = nullptr) {
		if (!graph || currentStopID.empty()) {
			cout << "Invalid input" << endl;
			return;
		}

		if (!graph->nodeExists(currentStopID)) {
			cout << "Stop " << currentStopID << " does not exist!" << endl;
			return;
		}

		cout << endl;
		cout << "==== EMERGENCY ROUTE FINDER (WITH BED CHECK) ====" << endl;
		cout << "Current Location: " << currentStopID << endl;

		// Get current stop name
		GraphNode* currentStopNode = graph->getNode(currentStopID);
		if (currentStopNode) {
			cout << "Location: " << currentStopNode->name << endl;
		}

		if (medicalSystem) {
			cout << "Checking hospital bed availability..." << endl;
		}

		cout << "==================================================" << endl;

		int nodeCount = 0;
		string* allNodes = graph->getAllNodeIDs(nodeCount);

		if (nodeCount == 0) {
			cout << "No stops in the network!" << endl;
			return;
		}

		const int MAX_RESULTS = 50;
		string hospitalIDs[MAX_RESULTS];
		double distances[MAX_RESULTS];
		string* paths[MAX_RESULTS];
		int emergencyBeds[MAX_RESULTS];
		int resultCount = 0;

		// Check each hospital stop
		for (int i = 0; i < nodeCount && resultCount < MAX_RESULTS; i++) {
			string nodeID = allNodes[i];

			// Check for hospital stops only
			if (nodeID.length() >= 4 && nodeID.substr(0, 4) == "HOS_") {
				// extract hospital ID
				string hospitalID = nodeID.substr(4);

				// Check bed availability if medical system is available
				int beds = 0;
				bool hasEnoughBeds = true;

				if (medicalSystem) {
					HashTable& hospitalTable = medicalSystem->getHospitalTable();
					Hospital* hospital = (Hospital*)hospitalTable.search(hospitalID);

					if (hospital) {
						beds = hospital->emergencyBeds;
						//  at least 15 emergency beds
						hasEnoughBeds = (beds >= 15);
					}
					else {
						// Hospital not found in medical system
						hasEnoughBeds = false;
					}
				}

				if (hasEnoughBeds) {
					double dist = 0.0;
					string* path = graph->dijkstra(currentStopID, nodeID, dist);

					if (path && dist > 0 && dist < 999999.0) {
						hospitalIDs[resultCount] = hospitalID;
						distances[resultCount] = dist;
						paths[resultCount] = path;
						emergencyBeds[resultCount] = beds;
						resultCount++;
					}
					else {
						if (path) delete[] path;
					}
				}
			}
		}

		delete[] allNodes;

		if (resultCount == 0) {
			cout << "\nERROR: No suitable hospital found!" << endl;
			if (medicalSystem) {
				cout << "Reasons:" << endl;
				cout << "1. No hospitals with at least 15 emergency beds" << endl;
				cout << "2. No reachable hospitals from your location" << endl;
			}
			else {
				cout << "No reachable hospitals found from your location!" << endl;
			}
			return;
		}

		// Sort by distance (bubble sort)
		for (int i = 0; i < resultCount - 1; i++) {
			for (int j = 0; j < resultCount - i - 1; j++) {
				if (distances[j] > distances[j + 1]) {
					// Swap all arrays
					string tempID = hospitalIDs[j];
					hospitalIDs[j] = hospitalIDs[j + 1];
					hospitalIDs[j + 1] = tempID;

					double tempDist = distances[j];
					distances[j] = distances[j + 1];
					distances[j + 1] = tempDist;

					string* tempPath = paths[j];
					paths[j] = paths[j + 1];
					paths[j + 1] = tempPath;

					int tempBeds = emergencyBeds[j];
					emergencyBeds[j] = emergencyBeds[j + 1];
					emergencyBeds[j + 1] = tempBeds;
				}
			}
		}

		// Display top 3 results
		cout << "\nSUITABLE HOSPITALS FOUND (Minimum 15 Emergency Beds):" << endl;
		cout << "==================================================" << endl;

		int displayCount = (resultCount < 3) ? resultCount : 3;

		for (int i = 0; i < displayCount; i++) {
			// Get hospital info from medical system 
			string hospitalName = "";
			string hospitalSector = "";

			if (medicalSystem) {
				HashTable& hospitalTable = medicalSystem->getHospitalTable();
				Hospital* hospital = (Hospital*)hospitalTable.search(hospitalIDs[i]);
				if (hospital) {
					hospitalName = hospital->name;
					hospitalSector = hospital->sector;
				}
			}

			// Get stop from graph
			string stopID = "HOS_" + hospitalIDs[i];
			GraphNode* hospitalStop = graph->getNode(stopID);
			string stopName = hospitalStop ? hospitalStop->name : stopID;

			cout << "\n" << (i + 1) << ". " << (hospitalName.empty() ? stopName : hospitalName) << endl;
			cout << "   Hospital ID: " << hospitalIDs[i] << endl;

			if (!hospitalSector.empty()) {
				cout << "   Sector: " << hospitalSector << endl;
			}

			cout << "   Distance: " << distances[i] << " km" << endl;
			cout << "   Emergency Beds Available: " << emergencyBeds[i] << endl;

			// Show bed status
			if (emergencyBeds[i] >= 20) {
				cout << "   Status:  HIGH CAPACITY" << endl;
			}
			else if (emergencyBeds[i] >= 15) {
				cout << "   Status:   MODERATE CAPACITY" << endl;
			}
			else {
				cout << "   Status:LOW CAPACITY (not recommended)" << endl;
			}

			if (i == 0 && paths[i]) {
				cout << "\n  ROUTE:" << endl;
				cout << "   ";
				for (int j = 0; j < 100 && paths[i][j] != ""; j++) {
					GraphNode* routeStop = graph->getNode(paths[i][j]);
					string routeStopName = routeStop ? routeStop->name : paths[i][j];

					cout << paths[i][j] << " (" << routeStopName << ")";

					if (paths[i][j + 1] != "") {
						cout << " -> ";
					}
				}
				cout << endl;
			}
		}
		// Clean up 
		for (int i = 0; i < resultCount; i++) {
			if (paths[i]) {
				delete[] paths[i];
			}
		}

		cout << "\n==================================================" << endl;

	}

	void addNewStopAtEnd(string busNumber, string newStopID, string newStopName, double lat, double lon) {
		Bus* bus = busTable->searchBus(busNumber);
		if (!bus) {
			cout << "Bus " << busNumber << " not found" << endl;
			return;
		}

		Route* route = bus->getCurrentRoute();
		if (!route) {
			cout << "Bus has no route" << endl;
			return;
		}
		// Check if bus is a school bus
		if (bus->getIsSchoolBus()) {
			cout << "\n=== SCHOOL BUS DETECTED ===" << endl;
			cout << "Bus " << busNumber << " is a school bus assigned to: "
				<< (bus->getAssignedSchool().empty() ? "Not assigned" : bus->getAssignedSchool()) << endl;

			// Check if new stop is a school or house stop
			bool isSchoolStop = (newStopID.length() >= 4 && newStopID.substr(0, 4) == "SCH_");
			bool isHouseStop = (newStopID.length() >= 4 && newStopID.substr(0, 4) == "HOU_");

			if (!isSchoolStop && !isHouseStop) {
				cout << "ERROR: Cannot add stop " << newStopID << " to school bus route!" << endl;
				cout << "Reason: School buses can only have school stops (SCH_) or house stops (HOU_)" << endl;
				cout << "Stop " << newStopID << " does not start with SCH_ or HOU_" << endl;
				return;
			}
		}
		if (!graph->nodeExists(newStopID)) {
			addStop(newStopID, newStopName, lat, lon);
		}

		RouteNode* lastNode = route->getHead();
		if (!lastNode) {
		
			route->insert(newStopID);
			cout << "New stop " << newStopID << " added as first stop to empty route" << endl;
			return;
		}

		// Traverse to the last node
		while (lastNode->getNext()) {
			lastNode = lastNode->getNext();
		}

		string lastStopID = lastNode->getId();

		route->insert(newStopID);

		GraphNode* lastStopNode = graph->getNode(lastStopID);
		GraphNode* newStopNode = graph->getNode(newStopID);

		if (lastStopNode && newStopNode) {
			double dx = newStopNode->latitude - lastStopNode->latitude;
			double dy = newStopNode->longitude - lastStopNode->longitude;
			double distance = sqrt(dx * dx + dy * dy) * 111.0; // Convert to km

			connectStops(lastStopID, newStopID, distance, true);
			cout << "Connected " << lastStopID << " <-> " << newStopID
				<< " (distance: " << distance << " km)" << endl;
		}

		cout << "New stop " << newStopID << " added at the end of the route" << endl;
		cout << "Updated route: ";
		route->display();
	}

	bool removeStopFromBusRoute(string busNumber, string stopID) {
		Bus* bus = busTable->searchBus(busNumber);
		if (!bus) {
			cout << "Bus " << busNumber << " not found." << endl;
			return false;
		}

		Route* currentRoute = bus->getCurrentRoute();
		if (!currentRoute) {
			cout << "Bus " << busNumber << " has no route." << endl;
			return false;
		}

		RouteNode* prev = nullptr;
		RouteNode* current = currentRoute->getHead();
		RouteNode* stopToRemove = nullptr;
		string prevStopID = "";
		string nextStopID = "";

		while (current != nullptr) {
			if (current->getId() == stopID) {
				stopToRemove = current;

				if (prev != nullptr) {
					prevStopID = prev->getId();
				}

				if (current->getNext() != nullptr) {
					nextStopID = current->getNext()->getId();
				}

				break;
			}
			prev = current;
			current = current->getNext();
		}

		if (!stopToRemove) {
			cout << "Stop " << stopID << " not found in bus " << busNumber << "'s route." << endl;
			return false;
		}

		string currentStop = bus->getCurrentStop();
		if (currentStop == stopID) {
			if (!nextStopID.empty()) {
				bus->setCurrentStop(nextStopID);
			}
			else if (!prevStopID.empty()) {
				bus->setCurrentStop(prevStopID);
			}
			else {
				bus->setCurrentStop("");
			}
		}

		bool removed = currentRoute->removeStop(stopID);

		if (removed) {
			cout << "Stop " << stopID << " successfully removed from bus " << busNumber << "'s route." << endl;

			if (!prevStopID.empty() && !nextStopID.empty()) {
				GraphNode* prevNode = graph->getNode(prevStopID);
				GraphNode* nextNode = graph->getNode(nextStopID);

				if (prevNode && nextNode) {
					double dx = nextNode->latitude - prevNode->latitude;
					double dy = nextNode->longitude - prevNode->longitude;
					double directDistance = sqrt(dx * dx + dy * dy) * 111.0;


					double existingDist = -1;
					string* path = graph->dijkstra(prevStopID, nextStopID, existingDist);

					bool alreadyConnected = false;
					if (path && existingDist > 0 && existingDist < 999999.0) {
						int stopCount = 0;
						for (int i = 0; i < 100 && !path[i].empty(); i++) {
							stopCount++;
						}

						if (stopCount == 2) {
							alreadyConnected = true;
						}
					}

					if (path) delete[] path;

					if (!alreadyConnected) {
						graph->addUndirectedEdge(prevStopID, nextStopID, directDistance);
						cout << "  Connected " << prevStopID << " <-> " << nextStopID
							<< " (direct distance: " << directDistance << " km)" << endl;
					}
					else {
						cout << "  " << prevStopID << " and " << nextStopID << " are already connected" << endl;
					}
				}
			}

			if (bus->getIsSchoolBus()) {
				bus->updateSchoolBusStatus();

				if (!bus->getIsSchoolBus()) {
					cout << "Bus " << busNumber << " is no longer a school bus." << endl;
				}
			}

			cout << "Updated route: ";
			currentRoute->display();
		}
		else {
			cout << "Failed to remove stop " << stopID << " from route." << endl;
		}

		return removed;
	}

	void diplayAllStops() {
		if (!graph) {
			cout << "No graph available" << endl;
			return;
		}
		cout << "\n=== ALL STOPS IN THE NETWORK ===" << endl;
		int nodeCount = 0;
		string* allNodes = graph->getAllNodeIDs(nodeCount);
		for (int i = 0; i < nodeCount; i++) {
			GraphNode* node = graph->getNode(allNodes[i]);
			if (node) {
				cout << "[" << (i + 1) << "] Stop ID: " << node->id
					<< ", Name: " << node->name
					<< ", Coordinates: (" << node->latitude << ", " << node->longitude << ")" << endl;
			}
		}
		delete[] allNodes;
	}
};
