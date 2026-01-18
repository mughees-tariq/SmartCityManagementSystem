#pragma once
#include "Graph.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "Utilities.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

// Facility structure
struct Facility {
	string id;
	string name;
	string type;
	string sector;
	double latitude;
	double longitude;

	Facility(string i, string n, string t, string s, double lat, double lon) {
		id = i;
		name = n;
		type = t;
		sector = s;
		latitude = lat;
		longitude = lon;
	}
};

class FacilitySystem {
private:
	HashTable facilityTable;
	HashTable facilityByType;
	Graph* cityGraph;
	Graph* facilityGraph;
	int facilityCount;

	const double min_lat = 33.60;
	const double max_lat = 33.80;
	const double min_lon = 72.90;
	const double max_lon = 73.20;
public:
	FacilitySystem(Graph* graph): facilityTable(100),facilityByType(100),
	cityGraph(graph){
		facilityCount = 0;
        facilityGraph = new Graph();
	}
    ~FacilitySystem() {
        delete facilityGraph;
	}
    Graph* getFacilityGraph() {
        return facilityGraph;
    }

	void displayAllFacilities() {
		cout << "--- All Facilities ---" << endl;
		cout << "-----------------------------" << endl;
		int keycount = 0;
		string* keys = facilityTable.getAllKeys(keycount);

		if (keycount == 0) {
			cout << "No facilities available." << endl;
			delete[]keys;
			return;
		}
		for (int i = 0; i < keycount; i++) {
			Facility* facility = (Facility*)facilityTable.search(keys[i]);
			if (facility) {
				cout << "" << i + 1 << ". " << facility->name << endl;
				cout << "   Type: " << facility->type << endl;
				cout << "   Sector: " << facility->sector << endl;
				cout << "   Location: (" << StringUtils::toString(facility->latitude,6)
					<< ", " << StringUtils::toString(facility->longitude, 6) << ")" << endl;
			}
		}
		cout << "Total Facilities: " << keycount << endl;
		cout << "------------------------------" << endl;
		delete[]keys;
	}
    // Find nearest facility by type
    void findNearestFacility(double lat, double lon, string type) {
        if (!cityGraph) {
            cout << "Error: City graph not initialized!" << endl;
            return;
        }

        if (!InputValidator::isValidLatitude(lat) ||
            !InputValidator::isValidLongitude(lon)) {
            cout << "Error: Invalid coordinates!" << endl;
            return;
        }

        if (!InputValidator::isValidString(type)) {
            cout << "\nError: Invalid facility type!" << endl;
            return;
        }

        LinkedList* facilities = (LinkedList*)facilityByType.search(type);

        if (!facilities || facilities->isEmpty()) {
            cout << "\nNo " << type << " facilities found!" << endl;
            return;
        }

        cout << "========================================" << endl;
        cout << "    NEAREST " << type << " FINDER (Using Dijkstra)" << endl;
        cout << "========================================" << endl;
        cout << "Your Location: (" << lat << ", " << lon << ")" << endl;
        cout << "----------------------------------------" << endl;

        // Add user location as temporary node
        string userNodeID = "My_Loc";
        cityGraph->addNode(userNodeID, "Your Location", lat, lon);

        // Store results in arrays
        const int MAX_RESULTS = 100;
        Facility* foundFacilities[MAX_RESULTS];
        double distances[MAX_RESULTS];
        string* paths[MAX_RESULTS];
        int resultCount = 0;

        // Check each facility
        Node* temp = facilities->getHead();
        while (temp && resultCount < MAX_RESULTS) {
            Facility* facility = (Facility*)temp->data;
            if (facility) {
                string facilityStopID = "FAC_" + facility->id;

                if (cityGraph->nodeExists(facilityStopID)) {
                    double totalDist = 0;
                    string* path = cityGraph->dijkstra(userNodeID, facilityStopID, totalDist);

                    if (totalDist > 0 && totalDist < 999999.0) {
                        foundFacilities[resultCount] = facility;
                        distances[resultCount] = totalDist;
                        paths[resultCount] = path;
                        resultCount++;
                    }
                    else {
                        if (path) delete[] path;
                    }
                }
            }
            temp = temp->next;
        }

        // Clean up temporary node
        cityGraph->removeNode(userNodeID);

        if (resultCount == 0) {
            cout << "\nNo reachable " << type << " facilities found!" << endl;
            cout << "========================================" << endl;
            return;
        }

        // Sort by distance (simple bubble sort)
        for (int i = 0; i < resultCount - 1; i++) {
            for (int j = 0; j < resultCount - i - 1; j++) {
                if (distances[j] > distances[j + 1]) {
                    // Swap facilities
                    Facility* tempFac = foundFacilities[j];
                    foundFacilities[j] = foundFacilities[j + 1];
                    foundFacilities[j + 1] = tempFac;

                    // Swap distances
                    double tempDist = distances[j];
                    distances[j] = distances[j + 1];
                    distances[j + 1] = tempDist;

                    // Swap paths
                    string* tempPath = paths[j];
                    paths[j] = paths[j + 1];
                    paths[j + 1] = tempPath;
                }
            }
        }

        // Display top 5
        cout << "\nNearest " << type  << endl;
        int displayCount = (resultCount < 5) ? resultCount : 5;

        for (int i = 0; i < displayCount; i++) {
            Facility* facility = foundFacilities[i];
            cout << (i + 1) << ". " << facility->name << endl;
            cout << "   Sector: " << facility->sector << endl;
            cout << "   Distance: "
                << StringUtils::toString(distances[i], 2) << " km" << endl;

            // Display route if it's the nearest one
            if (i == 0 && paths[i]) {
                cout << "   Route from your location:" << endl;
                cout << "   ";
                for (int j = 0; j < 100 && paths[i][j] != ""; j++) {
                    cout << paths[i][j];
                    if (paths[i][j + 1] != "") {
                        cout << " -> ";
                    }
                }
                cout << endl;
            }
            cout << endl;
        }

        // Clean up path memory
        for (int i = 0; i < resultCount; i++) {
            if (paths[i]) {
                delete[] paths[i];
            }
        }

        cout << "========================================" << endl;
    }

    // Find nearest facility (any type) using Dijkstra
    void findNearestAnyFacility(double lat, double lon) {
        if (!cityGraph) {
            cout << "Error: City graph not initialized!" << endl;
            return;
        }

        if (!InputValidator::isValidLatitude(lat) ||
            !InputValidator::isValidLongitude(lon)) {
            cout << "\nError: Invalid coordinates!" << endl;
            return;
        }

        cout << "========================================" << endl;
        cout << "    NEAREST FACILITY FINDER (Any Type, Using Dijkstra)" << endl;
        cout << "========================================" << endl;
        cout << "Your Location: (" << lat << ", " << lon << ")" << endl;
        cout << "----------------------------------------" << endl;

        // Add user location as temporary node
        string userNodeID = "MyLoc";
        cityGraph->addNode(userNodeID, "Your Location", lat, lon);

        // Get all facility keys
        int keyCount = 0;
        string* keys = facilityTable.getAllKeys(keyCount);

        if (keyCount == 0) {
            cout << "\nNo facilities in system!" << endl;
            cityGraph->removeNode(userNodeID);
            return;
        }

        // Store results
        const int MAX_RESULTS = 100;
        Facility* foundFacilities[MAX_RESULTS];
        double distances[MAX_RESULTS];
        string* paths[MAX_RESULTS];
        int resultCount = 0;

        // Check each facility
        for (int i = 0; i < keyCount && resultCount < MAX_RESULTS; i++) {
            Facility* facility = (Facility*)facilityTable.search(keys[i]);
            if (facility) {
                string facilityStopID = "FAC_" + facility->id;

                if (cityGraph->nodeExists(facilityStopID)) {
                    double totalDist = 0;
                    string* path = cityGraph->dijkstra(userNodeID, facilityStopID, totalDist);

                    if (totalDist > 0 && totalDist < 999999.0) {
                        foundFacilities[resultCount] = facility;
                        distances[resultCount] = totalDist;
                        paths[resultCount] = path;
                        resultCount++;
                    }
                    else {
                        if (path) delete[] path;
                    }
                }
            }
        }

        // Clean up
        cityGraph->removeNode(userNodeID);
        delete[] keys;

        if (resultCount == 0) {
            cout << "\nNo reachable facilities found!" << endl;
            cout << "========================================" << endl;
            return;
        }

        // Sort by distance (simple bubble sort)
        for (int i = 0; i < resultCount - 1; i++) {
            for (int j = 0; j < resultCount - i - 1; j++) {
                if (distances[j] > distances[j + 1]) {
                    // Swap facilities
                    Facility* tempFac = foundFacilities[j];
                    foundFacilities[j] = foundFacilities[j + 1];
                    foundFacilities[j + 1] = tempFac;

                    // Swap distances
                    double tempDist = distances[j];
                    distances[j] = distances[j + 1];
                    distances[j + 1] = tempDist;

                    // Swap paths
                    string* tempPath = paths[j];
                    paths[j] = paths[j + 1];
                    paths[j + 1] = tempPath;
                }
            }
        }

        // Display top 5
        cout << "\nNearest Facilities:" << endl;
        int displayCount = (resultCount < 5) ? resultCount : 5;

        for (int i = 0; i < displayCount; i++) {
            Facility* facility = foundFacilities[i];
            cout << (i + 1) << ". " << facility->name
                << " (" << facility->type << ")" << endl;
            cout << "   Sector: " << facility->sector << endl;
            cout << "   Distance: "
                << StringUtils::toString(distances[i], 2) << " km" << endl;

            // Display route if it's the nearest one
            if (i == 0 && paths[i]) {
                cout << "   Route from your location:" << endl;
                cout << "   ";
                for (int j = 0; j < 100 && paths[i][j] != ""; j++) {
                    cout << paths[i][j];
                    if (paths[i][j + 1] != "") {
                        cout << " -> ";
                    }
                }
                cout << endl;
            }
            cout << endl;
        }

        // Clean up path memory
        for (int i = 0; i < resultCount; i++) {
            if (paths[i]) {
                delete[] paths[i];
            }
        }

        cout << "========================================" << endl;
    }

    void addFacility(const string& csvFilename = "Data/facilities.csv") {
        string id, name, type, sector;
        double lat, lon;

        cout << "--- Add New Facility ---" << endl;
        cout << "Enter Facility ID: ";
        cin >> id;
        cin.ignore();

        if (!InputValidator::isValidString(id)) {
            cout << "Error: Invalid Facility ID!" << endl;
            return;
        }
        if (facilityTable.contains(id)) {
            cout << "Error: Facility with ID " << id << " already exists!" << endl;
            return;
        }

        cout << "Enter Facility Name: ";
        if (!SafeInput::getString(name)) {
            cout << "Error: Invalid name!" << endl;
            return;
        }

        cout << "Enter Facility Type (Mosque/Park/Water): ";
        if (!SafeInput::getString(type)) {
            cout << "Error: Invalid type!" << endl;
            return;
        }

        cout << "Enter Sector: ";
        if (!SafeInput::getString(sector)) {
            cout << "Error: Invalid sector!" << endl;
            return;
        }

        cout << "Enter Latitude (-90 to 90) or 0 for auto assign: ";
        if (!SafeInput::getDouble(lat, -90.0, 90.0)) {
            cout << "Invalid latitude! Auto-assigning." << endl;
            lat = randomDoubleInRange(min_lat, max_lat);
        }
        if (lat == 0) lat = randomDoubleInRange(min_lat, max_lat);

        cout << "Enter Longitude (-180 to 180) or 0 for auto assign: ";
        if (!SafeInput::getDouble(lon, -180.0, 180.0)) {
            cout << "Invalid longitude! Auto-assigning." << endl;
            lon = randomDoubleInRange(min_lon, max_lon);
        }
        if (lon == 0) lon = randomDoubleInRange(min_lon, max_lon);

        if (!SafeInput::getConfirmation("Add this facility?")) {
            cout << "Operation cancelled." << endl;
            return;
        }

        Facility* facility = new Facility(id, name, type, sector, lat, lon);
        facilityTable.insert(id, (void*)facility);

        LinkedList* typeList = (LinkedList*)facilityByType.search(type);
        if (!typeList) {
            typeList = new LinkedList();
            facilityByType.insert(type, (void*)typeList);
        }
        typeList->insert((void*)facility);

        if (cityGraph) {
            
            string StopID = "FAC_" + id;
            string StopName = name + " (" + type + ") Stop";
            cityGraph->addNode(StopID, StopName, lat, lon);
        }
        facilityGraph->addNode(id, name, lat, lon);

        facilityCount++;
        cout << "\nFacility added successfully!" << endl;
    }

    int getTotalFacilities() const { return facilityCount; }
    
};