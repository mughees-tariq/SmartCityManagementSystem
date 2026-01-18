#pragma once

#include "Graph.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "Utilities.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

struct Flight {
	string flightNumber;
	string airline;
	string origin;
	string destination;
	double departureTime;
	double arrivalTime;
	Flight(string fn, string al, string dest,
		double dep, double arr) {
		flightNumber = fn;
		airline = al;
		destination = dest;
		departureTime = dep;
		arrivalTime = arr;
	}
};
struct Airport {
	string id;
	string name;
	string city;
	double latitude;
	double longitude;
	LinkedList flights;
	Airport(string i, string n, string c, double lat, double lon) {
		id = i;
		name = n;
		city = c;
		latitude = lat;
		longitude = lon;
	}
};
class AirportSystem {
private:
	HashTable flightTable;
	HashTable airportTable;
	Graph* cityGraph;
	int airportCount;
	double min_lat = 33.60;
	double max_lat = 33.80;
	double min_lon = 72.90;
	double max_lon = 73.20;

public:
	AirportSystem(Graph* graph) : flightTable(100), airportTable(100), cityGraph(graph) {
		airportCount = 0;

		string id = "A01";
		string name = "Islamabad International Airport";
		string city = "Islamabad";
		double lat = 33.6167;
		double lon = 72.9167;
		Airport* airport = new Airport(id, name, city, lat, lon);
		airportTable.insert(id, (void*)airport);
		airportCount++;

		if (cityGraph) {
			
			cityGraph->addNode(id, name, lat, lon);
		}
		cout << "Airport System Initialized with " << airportCount << " airport(s)." << endl;
		cout << "----------------------------------------" << endl;
	}
	void displayAllAirports() {
		cout << endl << "--- All Airports ---" << endl;
		cout << "-----------------------------" << endl;
		int keycount = 0;
		string* keys = airportTable.getAllKeys(keycount);
		if (keycount == 0) {
			cout << "No airports available." << endl;
			delete[]keys;
			return;
		}
		for (int i = 0; i < keycount; i++) {
			Airport* airport = (Airport*)airportTable.search(keys[i]);
			if (airport) {
				cout << i + 1 << ". " << airport->name << endl;
				cout << "   ID: " << airport->id << endl;
				cout << "   Flights Available: " << airport->flights.getSize() << endl;
				cout << "   City: " << airport->city << endl;
				cout << "   Location: (" << StringUtils::toString(airport->latitude, 6)
					<< ", " << StringUtils::toString(airport->longitude, 6) << ")" << endl;
				cout << "Flight List:" << endl;
				if (airport->flights.isEmpty()) {
					cout << "   No flights available for this airport." << endl;
				}
				else {
					Node* temp = airport->flights.getHead();
					int count = 1;
					while (temp) {
						Flight* flight = (Flight*)temp->data;
						cout << "   " << count++ << ". Flight Number: " << flight->flightNumber
							<< ", Airline: " << flight->airline
							<< ", Origin: " << flight->origin
							<< ", Destination: " << flight->destination << endl;
						temp = temp->next;
					}
				}
			}
		}
		cout << "Total Airports: " << keycount << endl;
		cout << "------------------------------" << endl;
		delete[]keys;
	}
	void displayAllFlights() {
		cout << "--- All Flights ---" << endl;
		cout << "-----------------------------" << endl;
		int keycount = 0;
		string* keys = flightTable.getAllKeys(keycount);
		if (keycount == 0) {
			cout << "No flights available." << endl;
			delete[]keys;
			return;
		}
		for (int i = 0; i < keycount; i++) {
			Flight* flight = (Flight*)flightTable.search(keys[i]);
			if (flight) {
				cout  << i + 1 << ". Flight Number: " << flight->flightNumber << endl;
				cout << "   Airline: " << flight->airline << endl;
				cout << "   Destination: " << flight->destination << endl;
				cout << "   Departure Time: " << StringUtils::toString(flight->departureTime, 2) << endl;
				cout << "   Arrival Time: " << StringUtils::toString(flight->arrivalTime, 2) << endl;
			}
		}
		cout << "Total Flights: " << keycount << endl;
		cout << "------------------------------" << endl;
		delete[]keys;
	}
	void displayFlightsByAirport(const string& airportID) {
		Airport* airport = (Airport*)airportTable.search(airportID);
		if (!airport) {
			cout << "Airport with ID " << airportID << " not found!" << endl;
			return;
		}
		cout << "--- Flights for Airport: " << airport->name << " ---" << endl;
		cout << "-----------------------------" << endl;
		if (airport->flights.isEmpty()) {
			cout << "No flights available for this airport." << endl;
			return;
		}
		Node* temp = airport->flights.getHead();
		int count = 1;
		while (temp) {
			Flight* flight = (Flight*)temp->data;
			cout  << count++ << ". Flight Number: " << flight->flightNumber << endl;
			cout << "   Airline: " << flight->airline << endl;
			cout << "   Destination: " << flight->destination << endl;
			cout << "   Departure Time: " << StringUtils::toString(flight->departureTime, 2) << endl;
			cout << "   Arrival Time: " << StringUtils::toString(flight->arrivalTime, 2) << endl;
			temp = temp->next;
		}
		cout << "------------------------------" << endl;
	}
	void addFlight() {
		string flightNum, airline, destID;
		double depTime, arrTime;
		cout << "--- Add New Flight ---" << endl;
		cout << "Enter Flight Number: ";
		cin.ignore();
		if (!SafeInput::getString(flightNum)) {
			cout << "Error: Invalid Flight Number!" << endl;
			return;
		}
		if (flightTable.contains(flightNum)) {
			cout << "Error: Flight with Number " << flightNum << " already exists!" << endl;
			return;
		}
		cout << "Enter Airline: ";
		if (!SafeInput::getString(airline)) {
			cout << "Error: Invalid Airline!" << endl;
			return;
		}
		cout << "Enter Destination City: ";
		if (!SafeInput::getString(destID)) {
			cout << "Error: Invalid Destination City!" << endl;
			return;
		}
		cout << "Enter Departure Time (24hr format): ";
		if (!SafeInput::getDouble(depTime, 0.0, 23.99)) {
			cout << "Error: Invalid Departure Time!" << endl;
			return;
		}
		cout << "Enter Arrival Time (24hr format): ";
		if (!SafeInput::getDouble(arrTime, 0.0, 23.99)) {
			cout << "Error: Invalid Arrival Time!" << endl;
			return;
		}
		if (!SafeInput::getConfirmation("Add this Flight?")) {
			cout << "Operation cancelled." << endl;
			return;
		}
		Flight* flight = new Flight(flightNum, airline, destID, depTime, arrTime);
		string originID = "A01"; // Assuming all flights originate from Islamabad International Airport
		flightTable.insert(flightNum, (void*)flight);
		// Add flight to origin airport's flight list
		Airport* originAirport = (Airport*)airportTable.search(originID);
		if (originAirport) {
			originAirport->flights.insert((void*)flight);
		}
		cout << "\nFlight added successfully!" << endl;
	}


	void FindSuitableFlightByTime(const string& destID) {
		MinHeap pq(1000);
		int keycount = 0;
		string* keys = flightTable.getAllKeys(keycount);
		for (int i = 0; i < keycount; i++) {
			Flight* flight = (Flight*)flightTable.search(keys[i]);
			if (flight && flight->destination == destID) {
				double flightDuration = flight->arrivalTime - flight->departureTime;
				if (flightDuration < 0) flightDuration += 24.0; // Adjust for overnight flights
				pq.insert(flightDuration, (void*)flight);
			}
		}
		if (pq.isEmpty()) {
			cout << "No suitable flights found from Islamabad " << "to" << destID << "." << endl;
			delete[]keys;
			return;
		}
		cout << endl << "--- Suitable Flights from Islamabad " << " to " << destID << " ---" << endl;
		cout << "-----------------------------" << endl;
		int count = 1;
		while (!pq.isEmpty()) {
			HeapNode node = pq.extractMin();
			Flight* flight = (Flight*)node.data;
			cout << endl << count++ << ". Flight Number: " << flight->flightNumber << endl;
			cout << "   Airline: " << flight->airline << endl;
			cout << "   Departure Time: " << StringUtils::toString(flight->departureTime, 2) << endl;
			cout << "   Arrival Time: " << StringUtils::toString(flight->arrivalTime, 2) << endl;
			double duration = flight->arrivalTime - flight->departureTime;
			cout << "   Duration: " << StringUtils::toString(duration, 2) << " hours" << endl;
		}
		cout << "------------------------------" << endl;
		delete[]keys;
	}
};