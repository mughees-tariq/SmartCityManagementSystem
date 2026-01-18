#pragma once
#include "Graph.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "Utilities.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

struct Product {
    string name;
    string category;
    int price;
    string mallID;
    string mallName;
    string mallSector;
    Product(string n, string c, int p, string id, string mn, string ms) {
        name = n;
        category = c;
        price = p;
        mallID = id;
        mallName = mn;
        mallSector = ms;
    }
};

struct Mall {
    string id;
    string name;
    string sector;
    double latitude;
    double longitude;
    LinkedList products;

    Mall(string i, string n, string s, double lat, double lon) {
        id = i;
        name = n;
        sector = s;
        latitude = lat;
        longitude = lon;
    }
};

class CommercialSystem {
private:
    HashTable mallTable;
    HashTable productByName;
    HashTable productByCategory;
    Graph* cityGraph;
    Graph* commercialGraph;
    int mallCount;

    const double min_lat = 33.60;
    const double max_lat = 33.80;
    const double min_lon = 72.90;
    const double max_lon = 73.20;

public:
    CommercialSystem(Graph* graph) : mallTable(100), productByName(100), productByCategory(100), cityGraph(graph) {
        mallCount = 0;
        commercialGraph = new Graph;
    }

    void displayAllMalls() {
        cout << endl << "   All Shopping Malls" << endl;
        cout << "-------------------------" << endl;
        int keycount = 0;
        string* keys = mallTable.getAllKeys(keycount);
        if (keycount == 0) {
            cout << "No Malls In the System" << endl;
            delete[]keys;
            return;
        }
        for (int i = 0; i < keycount; i++) {
            Mall* mall = (Mall*)mallTable.search(keys[i]);
            if (mall) {
                cout << endl << i + 1 << ". " << mall->name << endl;
                cout << "   ID: " << mall->id << endl;
                cout << "   Sector: " << mall->sector << endl;
                cout << "   Location: (" << StringUtils::toString(mall->latitude, 6)
                    << ", " << StringUtils::toString(mall->longitude, 6) << ")" << endl;
                cout << "   Products Available: " << mall->products.getSize() << endl;
            }
        }
        cout << endl<< "Total Malls : " << keycount << endl;
        cout << "---------------------------------" << endl;
        delete[]keys;
    }

    void displayProductsInMall(const string& mallid) {
        if (!InputValidator::isValidString(mallid)) {
            cout <<endl << "Error: Invalid mall Id" << endl;
            return;
        }
        Mall* mall = (Mall*)mallTable.search(mallid);
        if (!mall) {
            cout << endl << "Mall not found" << endl;
            return;
        }
        cout << endl<< "Products in The Mall: " << mall->name << endl;
        Node* temp = mall->products.getHead();
        int idx = 1;
        if (!temp) {
            cout << "No Products Listed" << endl;
            return;
        }
        while (temp) {
            Product* p = (Product*)temp->data;
            if (p) {
                cout << endl << idx << ". " << p->name << " | Category: " << p->category
                    << " | Price: Rs. " << p->price << endl;
                idx++;
            }
            temp = temp->next;
        }
    }

    void SearchProductByName(string name) {
        if (!InputValidator::isValidString(name)) {
            cout << endl<< " Invalid Name Entered" << endl;
            return;
        }

        LinkedList* products = (LinkedList*)productByName.search(name);

        if (!products || products->isEmpty()) {
            cout << endl <<"Product not found!" << endl;
            return;
        }
        cout << endl << "   Product Search Results" << endl;
        cout << "------------------------" << endl;

        Node* temp = products->getHead();
        int count = 1;
        while (temp) {
            Product* prod = (Product*)temp->data;
            if (prod) {
                cout << endl << count++ << ". Available at: " << prod->mallName << endl;
                cout << "  Mall ID: " << prod->mallID << endl;
                cout << "  Location: " << prod->mallSector << endl;
                cout << "  Category: " << prod->category << endl;
                cout << "  Price: Rs." << prod->price << endl;
            }
            temp = temp->next;
        }
        cout << "----------------------------------" << endl;
    }

    void searchByCategory(string category) {
        if (!InputValidator::isValidString(category)) {
            cout << endl << "Error: Invalid category!" << endl;
            return;
        }

        LinkedList* products = (LinkedList*)productByCategory.search(category);

        if (!products || products->isEmpty()) {
            cout << endl << "No products in category: " << category << endl;
            return;
        }

        cout << endl << "   CATEGORY: " << category << endl;
        cout << "========================================" << endl;

        Node* temp = products->getHead();
        int count = 1;
        while (temp) {
            Product* prod = (Product*)temp->data;
            if (prod) {
                cout << endl << count++ << ". " << prod->name << endl;
                cout << "   Mall: " << prod->mallName << " (" << prod->mallSector << ")" << endl;
                cout << "   Price: Rs. " << prod->price << endl;
            }
            temp = temp->next;
        }

        cout << "========================================" << endl;
    }

    void findNearestMall(double lat, double lon) {
        if (!InputValidator::isValidLatitude(lat) || !InputValidator::isValidLongitude(lon)) {
            cout << endl << "Error: Invalid coordinates!" << endl;
            return;
        }

        if (!cityGraph) {
            cout << "Error: City graph is not initialized yet" << endl;
            return;
        }

        cout << endl << "========================================" << endl;
        cout << "    NEAREST MALL FINDER (Dijkstra)" << endl;
        cout << "========================================" << endl;
        cout << "Your Location: (" << lat << ", " << lon << ")" << endl;
        cout << "----------------------------------------" << endl;

        int keyCount = 0;
        string* keys = mallTable.getAllKeys(keyCount);

        if (keyCount == 0) {
            cout << endl << "No malls in system! Please add malls first." << endl;
            delete[]keys;
            return;
        }

        // Add user location to graph
        string userNodeID = "USER_COMM_LOC";
        commercialGraph->addNode(userNodeID, "Your Location", lat, lon);

        // Connect user to all malls
        int connectedCount = 0;
        for (int i = 0; i < keyCount; i++) {
            Mall* mall = (Mall*)mallTable.search(keys[i]);
            if (mall) {
                // Ensure mall is in graph
                GraphNode* mallNode = commercialGraph->getNode(mall->id);
                if (!mallNode) {
                    //commercialGraph->addNode(mall->id, mall->name, mall->latitude, mall->longitude);
                    continue;
                }

                double dist = MathUtils::calculateDistance(lat, lon, mall->latitude, mall->longitude);
                if (dist >= 0 && dist < 1000.0) {
                    commercialGraph->addEdge(userNodeID, mall->id, dist);
                    commercialGraph->addEdge(mall->id, userNodeID, dist);
                    connectedCount++;
                }
            }
        }

        if (connectedCount == 0) {
            cout << endl << "Could not connect to any malls!" << endl;
            delete[] keys;
            return;
        }

        // Run Dijkstra for each mall
        struct MallDist {
            Mall* mall;
            double distance;
            string* path;
        };

        int processLimit = (keyCount < 10) ? keyCount : 10;
        MallDist* results = new MallDist[processLimit];
        int resultCount = 0;

        for (int i = 0; i < keyCount && resultCount < processLimit; i++) {
            Mall* mall = (Mall*)mallTable.search(keys[i]);
            if (mall) {
                double totalDist = 0.0;
                string* path = commercialGraph->dijkstra(userNodeID, mall->id, totalDist);

                if (totalDist > 0 && totalDist < 999999.0) {
                    results[resultCount].mall = mall;
                    results[resultCount].distance = totalDist;
                    results[resultCount].path = path;
                    resultCount++;
                }
                else {
                    // Fallback to straight-line distance
                    double straightDist = MathUtils::calculateDistance(lat, lon,
                        mall->latitude, mall->longitude);
                    if (straightDist >= 0) {
                        results[resultCount].mall = mall;
                        results[resultCount].distance = straightDist;
                        results[resultCount].path = new string[100];
                        for (int j = 0; j < 100; j++) results[resultCount].path[j] = "";
                        results[resultCount].path[0] = userNodeID;
                        results[resultCount].path[1] = mall->id;
                        resultCount++;
                    }
                    if (path) delete[] path;
                }
            }
        }

        // Sort by distance
        for (int i = 0; i < resultCount - 1; i++) {
            for (int j = 0; j < resultCount - i - 1; j++) {
                if (results[j].distance > results[j + 1].distance) {
                    MallDist temp = results[j];
                    results[j] = results[j + 1];
                    results[j + 1] = temp;
                }
            }
        }

        // Display results
        cout << endl << "Top 5 Nearest Malls:" << endl;
        cout << "========================================" << endl;
        int displayCount = (resultCount < 5) ? resultCount : 5;

        for (int i = 0; i < displayCount; i++) {
            Mall* mall = results[i].mall;
            if (mall) {
                cout << endl << (i + 1) << ". " << mall->name << endl;
                cout << "   Sector: " << mall->sector << endl;
                cout << "   Distance: " << StringUtils::toString(results[i].distance, 2) << " km" << endl;
                cout << "   Products Available: " << mall->products.getSize() << " items" << endl;
            }
        }

        if (resultCount == 0) {
            cout << endl << "No reachable malls found!" << endl;
        }

        cout << "========================================" << endl;

        // Cleanup
        for (int i = 0; i < resultCount; i++) {
            if (results[i].path) {
                delete[] results[i].path;
            }
        }
        delete[] results;
        delete[] keys;
        commercialGraph->removeNode("USER_COMM_LOC");
    }

    void addMall() {

        if (!cityGraph) {
            cout << "Error: City graph is not initialized yet" << endl;
            return;
        }

        string id, name, sector;
        double lat, lon;
        cout << endl << "--- Add New Mall ---" << endl;
        cout << "Enter Mall ID: ";
        cin >> id;
        cin.ignore();

        if (!InputValidator::isValidString(id)) {
            cout << "Error: Invalid Mall ID!" << endl;
            return;
        }
        if (mallTable.contains(id)) {
            cout << "Error: Mall with ID " << id << " already exists!" << endl;
            return;
        }

        cout << "Enter Mall Name: ";
        if (!SafeInput::getString(name)) {
            cout << "Error: Invalid Mall Name!" << endl;
            return;
        }

        cout << "Enter Sector: ";
        if (!SafeInput::getString(sector)) {
            cout << "Error: Invalid Sector!" << endl;
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

        if (!SafeInput::getConfirmation("Add this mall?")) {
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
            cout << "Error: Mall could not be added to the city" << endl;
            return;
        }

        Mall* mall = new Mall(id, name, sector, lat, lon);
        mallTable.insert(id, (void*)mall);
        mallCount++;
        if (cityGraph) {
			string stopID = "MALL_" + id;
			string stopName = name + " Mall";
			cityGraph->addNode(stopID, stopName, lat, lon);
			cout << "Mall added to city graph as stop: " << stopName << " [" << stopID << "]" << endl;
        }
        // Add to graph and auto-connect to existing malls
        commercialGraph->addNode(id, name, lat, lon);

        // Auto-connect to existing malls
        int existingCount = 0;
        string* existingKeys = mallTable.getAllKeys(existingCount);

        for (int i = 0; i < existingCount; i++) {
            if (existingKeys[i] != id) {
                Mall* existingMall = (Mall*)mallTable.search(existingKeys[i]);
                if (existingMall) {
                    double dist = MathUtils::calculateDistance(
                        lat, lon,
                        existingMall->latitude, existingMall->longitude
                    );
                    if (dist > 0 && dist < 50.0) {
                        cityGraph->addUndirectedEdge(id, existingMall->id, dist);
                    }
                }
            }
        }
        delete[] existingKeys;
        cout << "\nMall added successfully! Total malls: " << mallCount << endl;
    }

    void addProduct() {
        string mallID;
        cout << "\nEnter Mall ID to add product to: ";
        cin >> mallID;
        cin.ignore();

        Mall* mall = (Mall*)mallTable.search(mallID);
        if (!mall) {
            cout << "Mall not found!" << endl;
            return;
        }

        string pname, category;
        int price;
        cout << "Enter Product Name: ";
        if (!SafeInput::getString(pname)) {
            cout << "Error: Invalid product name!" << endl;
            return;
        }

        cout << "Enter Category: ";
        if (!SafeInput::getString(category)) {
            cout << "Error: Invalid category!" << endl;
            return;
        }

        cout << "Enter Price (Rs): ";
        if (!SafeInput::getInt(price, 0, 100000000)) {
            cout << "Error: Invalid price!" << endl;
            return;
        }

        if (!SafeInput::getConfirmation("Add this product?")) {
            cout << "Operation cancelled." << endl;
            return;
        }

        Product* product = new Product(pname, category, price, mall->id, mall->name, mall->sector);
        mall->products.insert((void*)product);

        // Index by product name
        LinkedList* nameList = (LinkedList*)productByName.search(pname);
        if (!nameList) {
            nameList = new LinkedList();
            productByName.insert(pname, (void*)nameList);
        }
        nameList->insert((void*)product);

        // Index by category
        LinkedList* catList = (LinkedList*)productByCategory.search(category);
        if (!catList) {
            catList = new LinkedList();
            productByCategory.insert(category, (void*)catList);
        }
        catList->insert((void*)product);

        cout << "\nProduct added successfully to " << mall->name << "!" << endl;
    }

    int getTotalMalls()const {
        return mallCount;
    }

    ~CommercialSystem() {}
};