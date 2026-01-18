#pragma once
#include "Graph.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "Utilities.h"
#include <fstream>
#include <string>
#include <iostream>
using namespace std;

struct Journey {
	string journeynumber;
	string railcompany;
	string origin;
	string destination;
	double departureTime;
	double arrivalTime;
	double latitude;
	double longitude;
	Journey(string jn, string rc, string org, string dest,
		double dep, double arr, double lat, double lon) {
		journeynumber = jn;
		railcompany = rc;
		origin = org;
		destination = dest;
		departureTime = dep;
		arrivalTime = arr;
		latitude = lat;
		longitude = lon;
	}
};
struct RailwayStation {
	string id;
	string name;
	string city;
	double latitude;
	double longitude;
	LinkedList journeys;
	RailwayStation(string i, string n, string c, double lat, double lon) {
		id = i;
		name = n;
		city = c;
		latitude = lat;
		longitude = lon;
	}
};
class RailwaySystem {
private:
	HashTable JourneyTable;
	HashTable RailwayStationTable;
	Graph* cityGraph;
	Graph* railwayGraph;
	int StationCount;
	const double min_lat = 33.60;
	const double max_lat = 33.80;
	const double min_lon = 72.90;
	const double max_lon = 73.20;

public:
	RailwaySystem(Graph* graph) : JourneyTable(100), RailwayStationTable(100), cityGraph(graph) {
		StationCount = 0;
		railwayGraph = new Graph;
	}
	void displayAllRailwayStations() {
		cout << endl << "--- All Railway Stations ---" << endl;
		cout << "-----------------------------" << endl;
		int keycount = 0;
		string* keys = RailwayStationTable.getAllKeys(keycount);
		if (keycount == 0) {
			cout << "No Railway Stations available." << endl;
			delete[]keys;
			return;
		}
		for (int i = 0; i < keycount; i++) {
			RailwayStation* railwaystation = (RailwayStation*)RailwayStationTable.search(keys[i]);
			if (railwaystation) {
				cout << endl << i + 1 << ". " << railwaystation->name << endl;
				cout << "   ID: " << railwaystation->id << endl;
				cout << "   Journeys Available: " << railwaystation->journeys.getSize() << endl;
				cout << "   City: " << railwaystation->city << endl;
				cout << "   Location: (" << StringUtils::toString(railwaystation->latitude, 6)
					<< ", " << StringUtils::toString(railwaystation->longitude, 6) << ")" << endl;
				cout << "Journey List:" << endl;
				if (railwaystation->journeys.isEmpty()) {
					cout << "   No journeys available for this railway station." << endl;
				}
				else {
					Node* temp = railwaystation->journeys.getHead();
					int count = 1;
					while (temp) {
						Journey* journey = (Journey*)temp->data;
						cout << "   " << count++ << ". Journey Number: " << journey->journeynumber
							<< ", Company: " << journey->railcompany
							<< ", Origin: " << journey->origin
							<< ", Destination: " << journey->destination << endl;
						temp = temp->next;
					}
				}
			}
		}
		cout << "Total Railway Stations: " << keycount << endl;
		cout << "------------------------------" << endl;
		delete[]keys;
	}
	void displayAllJourneys() {
		cout << "--- All Journeys ---" << endl;
		cout << "-----------------------------" << endl;
		int keycount = 0;
		string* keys = JourneyTable.getAllKeys(keycount);
		if (keycount == 0) {
			cout << "No journeys available." << endl;
			delete[]keys;
			return;
		}
		for (int i = 0; i < keycount; i++) {
			Journey* journey = (Journey*)JourneyTable.search(keys[i]);
			if (journey) {
				cout << endl << i + 1 << ". Journey Number: " << journey->journeynumber << endl;
				cout << "   RailwayCompany: " << journey->railcompany << endl;
				cout << "   Origin: " << journey->origin << endl;
				cout << "   Destination: " << journey->destination << endl;
				cout << "   Departure Time: " << StringUtils::toString(journey->departureTime, 2) << endl;
				cout << "   Arrival Time: " << StringUtils::toString(journey->arrivalTime, 2) << endl;
			}
		}
		cout << "Total Journeys: " << keycount << endl;
		cout << "------------------------------" << endl;
		delete[]keys;
	}
	void displayJourneysByRailwayStation(const string& railstationID) {
		RailwayStation* railwaystation = (RailwayStation*)RailwayStationTable.search(railstationID);
		if (!railwaystation) {
			cout << "RailwayStation with ID " << railstationID << " not found!" << endl;
			return;
		}
		cout << endl << "--- Journeys for Railway Station: " << railwaystation->name << " ---" << endl;
		cout << "-----------------------------" << endl;
		if (railwaystation->journeys.isEmpty()) {
			cout << "No Journeys available for this RailwayStation." << endl;
			return;
		}
		Node* temp = railwaystation->journeys.getHead();
		int count = 1;
		while (temp) {
			Journey* journey = (Journey*)temp->data;
			cout << count++ << ". Journey Number: " << journey->journeynumber << endl;
			cout << "   Railway Company: " << journey->railcompany << endl;
			cout << "   Origin: " << journey->origin << endl;
			cout << "   Destination: " << journey->destination << endl;
			cout << "   Departure Time: " << StringUtils::toString(journey->departureTime, 2) << endl;
			cout << "   Arrival Time: " << StringUtils::toString(journey->arrivalTime, 2) << endl;
			temp = temp->next;
		}
		cout << "------------------------------" << endl;
	}
	void addRailwayStation() {

		if (!cityGraph) {
			cout << "Error: City graph is not initialized yet" << endl;
			return;
		}

		string id, name, city;
		double lat, lon;

		cout << "--- Add New Railway Station ---" << endl;
		cout << "Enter Railway Station ID: ";
		cin >> id;
		cin.ignore();

		if (!InputValidator::isValidString(id)) {
			cout << "Error: Invalid Railway Station ID!" << endl;
			return;
		}
		if (RailwayStationTable.contains(id)) {
			cout << "Error: Railway Station with ID " << id << " already exists!" << endl;
			return;
		}

		cout << "Enter Railway Station Name: ";
		if (!SafeInput::getString(name)) {
			cout << "Error: Invalid Name!" << endl;
			return;
		}

		cout << "Enter City: ";
		if (!SafeInput::getString(city)) {
			cout << "Error: Invalid City!" << endl;
			return;
		}

		cout << "Enter Latitude (-90 to 90) or 0 for auto assign: ";
		if (!SafeInput::getDouble(lat, -90.0, 90.0)) {
			cout << "Invalid latitude! Auto-assigning." << endl;
			lat = randomDoubleInRange(min_lat, max_lat);
		}
		if (lat == 0)
			lat = randomDoubleInRange(min_lat, max_lat);

		cout << "Enter Longitude (-180 to 180) or 0 for auto assign: ";
		if (!SafeInput::getDouble(lon, -180.0, 180.0)) {
			cout << "Invalid longitude! Auto-assigning." << endl;
			lon = randomDoubleInRange(min_lon, max_lon);
		}
		if (lon == 0) lon = randomDoubleInRange(min_lon, max_lon);

		if (!SafeInput::getConfirmation("Add this Railway Station?")) {
			cout << "Operation cancelled." << endl;
			return;
		}

		if (!cityGraph->hasFreeSlots()) {
			cout << "Error: Cannot have more buildings in the city" << endl;
			return;
		}
		if (cityGraph->containsLocationId(id)) {
			cout << "Error: City already has a building with ID " << id << endl;
			return;
		}
		if (cityGraph->hasNonStopAtLocation(lat, lon)) {
			cout << "Error: City already has a building at (" << lat << ", " << lon << ")" << endl;
			return;
		}
		if (!cityGraph->addLocation(id, name, lat, lon)) {
			cout << "Error: Railway Station could not be added to the city" << endl;
			return;
		}

		RailwayStation* railwaystation = new RailwayStation(id, name, city, lat, lon);
		RailwayStationTable.insert(id, (void*)railwaystation);
		StationCount++;
		if (cityGraph) {
			string stopId = "RS_" + id;
			string stopName = name + " Rail Station";
			cityGraph->addNode(stopId, stopName, lat, lon);
			cout << "Railway Station added to city graph"<< endl;
		}
		// Add to graph and connect to existing railwaystations
		railwayGraph->addNode(id, name, lat, lon);

		int existingCount = 0;
		string* existingKeys = RailwayStationTable.getAllKeys(existingCount);

		for (int i = 0; i < existingCount; i++) {
			if (existingKeys[i] != id) {
				RailwayStation* existingStation = (RailwayStation*)RailwayStationTable.search(existingKeys[i]);
				if (existingStation) {
					double dist = MathUtils::calculateDistance(lat, lon, existingStation->latitude, existingStation->longitude);
					if (dist > 0 && dist < 50.0) {
						railwayGraph->addUndirectedEdge(id, existingStation->id, dist);
					}
				}
			}
		}
		delete[] existingKeys;

		cout << endl << "Railway Station added successfully!" << endl;
	}
	void addJourney() {
		string JourneyNum, RailComp, originID, destID;
		double depTime, arrTime;
		cout << "--- Add New Journey ---" << endl;
		cout << "Enter Journey Number: ";
		cin.ignore();
		if (!SafeInput::getString(JourneyNum)) {
			cout << "Error: Invalid Journey Number!" << endl;
			return;
		}
		if (JourneyTable.contains(JourneyNum)) {
			cout << "Error: Journey with Number " << JourneyNum << " already exists!" << endl;
			return;
		}
		cout << "Enter Railway Company: ";
		if (!SafeInput::getString(RailComp)) {
			cout << "Error: Invalid Railway Company!" << endl;
			return;
		}
		cout << "Enter Origin Railway Station ID: ";
		if (!SafeInput::getString(originID) || !RailwayStationTable.contains(originID)) {
			cout << "Error: Invalid Origin RailwayStation ID!" << endl;
			return;
		}
		cout << "Enter Destination RailwayStation ID: ";
		if (!SafeInput::getString(destID) || !RailwayStationTable.contains(destID)) {
			cout << "Error: Invalid Destination RailwayStation ID!" << endl;
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
		if (!SafeInput::getConfirmation("Add this Joueney?")) {
			cout << "Operation cancelled." << endl;
			return;
		}
		Journey* journey = new Journey(JourneyNum, RailComp, originID, destID, depTime, arrTime,
			((RailwayStation*)RailwayStationTable.search(originID))->latitude,
			((RailwayStation*)RailwayStationTable.search(originID))->longitude);
		JourneyTable.insert(JourneyNum, (void*)journey);
		// Add flight to origin station's journey list
		RailwayStation* originStation = (RailwayStation*)RailwayStationTable.search(originID);
		if (originStation) {
			originStation->journeys.insert((void*)journey);
		}
		cout << "\nJourney added successfully!" << endl;
	}
	void findNearestRailwayStation(double userLat, double userLon) {

		if (!railwayGraph) {
			cout << "Graph not initialized!" << endl;
			return;
		}

		string nearestRailwayStationID;
		double minDistance = 1e9;
		int stationKeyCount = 0;
		string* stationKeys = RailwayStationTable.getAllKeys(stationKeyCount);
		for (int i = 0; i < stationKeyCount; i++) {
			RailwayStation* station = (RailwayStation*)RailwayStationTable.search(stationKeys[i]);
			if (station) {
				double dist = MathUtils::calculateDistance(userLat, userLon, station->latitude, station->longitude);
				if (dist < minDistance) {
					minDistance = dist;
					nearestRailwayStationID = station->id;
				}
			}
		}
		delete[] stationKeys;
		if (nearestRailwayStationID.empty()) {
			cout << "No Railway Stations found!" << endl;
			return;
		}
		cout << endl << "Nearest Railway Station: " << nearestRailwayStationID << endl;
		cout << "Name: " << ((RailwayStation*)RailwayStationTable.search(nearestRailwayStationID))->name << endl;
		cout << "Location: (" << StringUtils::toString(
			((RailwayStation*)RailwayStationTable.search(nearestRailwayStationID))->latitude, 6)
			<< ", " << StringUtils::toString(
				((RailwayStation*)RailwayStationTable.search(nearestRailwayStationID))->longitude, 6) << ")" << endl;
		cout << "Distance: " << StringUtils::toString(minDistance, 2) << " km" << endl;
		cout << "-----------------------------------" << endl;
	}
};