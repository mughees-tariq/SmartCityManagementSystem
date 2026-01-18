

#pragma once
#include <iostream>
#include "Graph.h"
#include "HashTable.h"
#include "MinHeap.h"
#include "MaxHeap.h"
#include "Utilities.h"
#include <fstream>
#include <string>

using namespace std;

//Hospital
struct Hospital {
	string id;
	string name;
	string sector;
	int emergencyBeds;
	LinkedList specialization;
	double latitude;
	double longitude;

	Hospital() {
		emergencyBeds = 0;
		latitude = longitude = 0.0;
	}
	Hospital(string i, string n, string s, int beds) {
		id = i;
		name = n;
		sector = s;
		emergencyBeds = beds;
		latitude = longitude = 0.0;
	}

};

struct Medicine {
	string name;
	string formula;
	int price;
	string pharmacyID;
	string pharmacyName;
	string pharmacySector;

	Medicine() {
		price = 0;
	}
	Medicine(string n, string f, int p, string pid, string pn, string ps) {
		name = n;
		formula = f;
		price = p;
		pharmacyID = pid;
		pharmacyName = pn;
		pharmacySector = ps;
	}
};
struct Pharmacy {

	string id;
	string name;
	string sector;
	LinkedList medicines;
	double latitude;
	double longitude;
	Pharmacy(string i, string n, string s) {
		id = i;
		name = n;
		sector = s;
		latitude = longitude = 0.0;
	}
	Pharmacy(string i, string n, string s, double lat, double lon) {
		id = i;
		name = n;
		sector = s;
		latitude = lat;
		longitude = lon;
	}
};
//Doctor 
struct Doctor {
	string id;
	string name;
	string specialization;
	string hospitalId;
	int experience;
	string contact;
	Doctor() {
		experience = 0;
	}
	Doctor(string i, string n, string s, string h, int exp, string c) {
		id = i;
		name = n;
		specialization = s;
		hospitalId = h;
		experience = exp;
		contact = c;
	}
};

// Patient 
struct Patient {
	string cnic;
	string name;
	int age;
	string doctorID;
	string diagnosis;
	string admissionDate;

	Patient() {
		age = 0;
	}

	Patient(string c, string n, int a, string doc, string diag, string date) {
		cnic = c;
		name = n;
		age = a;
		doctorID = doc;
		diagnosis = diag;
		admissionDate = date;
	}
};

class MedicalSystem {
private:
	HashTable hospitalTable;
	HashTable medicineByname;
	HashTable medicineByFormula;
	HashTable pharmacyTable;
	HashTable doctorsTable;
	HashTable patientsTable;
	HashTable doctorsBySpecialization;
	Graph* medicalGraph;
	Graph* cityGraph;
	int hospitalcount;
	int pharmacycount;
	int doctorCount;
	int patientCount;

	// Islamabad approximate bounds
	 double min_lat = 33.60;
	 double max_lat = 33.80;
	 double min_lon = 72.90;
	 double max_lon = 73.20;

public:
	MedicalSystem(Graph* gp) : hospitalTable(100),
		medicineByname(100), medicineByFormula(100),
		pharmacyTable(100), doctorsTable(100), patientsTable(100),
		doctorsBySpecialization(100)
	{
		cityGraph = gp;
		hospitalcount = pharmacycount = doctorCount = patientCount = 0;
		medicalGraph = new Graph;
	}

	//load from csv with validation
	bool loadHospitals(string filename) {
		ifstream file(filename.c_str());
		if (!file.is_open()) {
			cout << "Error: Could not open " << filename << endl;
			return false;
		}

		if (!cityGraph) {
			cout << "Error: City Graph has not been initialized yet" << endl;
			file.close();
			return false;
		}

		string line;
		getline(file, line);
		int loadcount = 0;
		int linenum = 1;
		while (getline(file, line)) {
			linenum++;
			if (line.empty()) continue;

			int comma1 = -1, comma2 = -1, comma3 = -1, comma4 = -1;
			int commaCount = 0;
			bool inQuotes = false;

			for (int i = 0; i < line.length(); i++) {
				if (line[i] == '"') inQuotes = !inQuotes;
				if (line[i] == ',' && !inQuotes) {
					commaCount++;
					if (commaCount == 1) comma1 = i;
					else if (commaCount == 2) comma2 = i;
					else if (commaCount == 3) comma3 = i;
					else if (commaCount == 4) comma4 = i;
				}
			}

			if (comma3 == -1) {
				cout << "Warning: Invalid hospital data at line " << linenum << endl;
				continue;
			}

			string id = StringUtils::trim(line.substr(0, comma1));
			string name = StringUtils::trim(line.substr(comma1 + 1, comma2 - comma1 - 1));
			string sector = StringUtils::trim(line.substr(comma2 + 1, comma3 - comma2 - 1));

			string bedsStr, specializationsStr;
			if (comma4 != -1) {
				bedsStr = StringUtils::trim(line.substr(comma3 + 1, comma4 - comma3 - 1));
				specializationsStr = StringUtils::trim(line.substr(comma4 + 1));
			}
			else {
				bedsStr = StringUtils::trim(line.substr(comma3 + 1));
				specializationsStr = "";
			}

			// Validate data
			if (!InputValidator::isValidString(id) || !InputValidator::isValidString(name)) {
				cout << "Warning: Invalid hospital data at line " << linenum << endl;
				continue;
			}

			int beds = StringUtils::toInt(bedsStr);
			if (!InputValidator::isPositive(beds)) {
				cout << "Warning: Invalid bed count at line " << linenum << endl;
				continue;
			}

			specializationsStr = StringUtils::removeQuotes(specializationsStr);

			double lat = randomDoubleInRange(min_lat, max_lat);
			double lon = randomDoubleInRange(min_lon, max_lon);

			if (!cityGraph->hasFreeSlots()) {
				cout << "Error: Cannot have more buildings in the city" << endl;
				continue;
			}
			if (cityGraph->containsLocationId(id)) {
				cout << "Error: City already has a building with ID " << id << endl;
				continue;
			}
			if (cityGraph->hasNonStopAtLocation(lat, lon)) {
				cout << "Error: City already has a building at (" << lat << ", " << lon << ")" << endl;
				continue;
			}
			string transportID = "HOS_" + id;
			if (!cityGraph->addLocation(transportID, name+ " Hospital Stop", lat, lon)) {
				cout << "Error: Hopsital could not be added to the city" << endl;
				continue;
			}

			Hospital* hospital = new Hospital(id, name, sector, beds);
			hospital->latitude = lat;
			hospital->longitude = lon;

			if (!specializationsStr.empty()) {
				int specCount = 0;
				string* specs = StringUtils::split(specializationsStr, ',', specCount);
				if (specs) {
					for (int i = 0; i < specCount; i++) {
						string spec = StringUtils::trim(specs[i]);
						if (!spec.empty()) {
							hospital->specialization.insert((void*)new string(spec));
						}
					}
					delete[] specs;
				}
			}

			hospitalTable.insert(id, (void*)hospital);
			hospitalcount++;
			loadcount++;

			medicalGraph->addNode(id, name, lat, lon);
		}
		file.close();
		cout << "Loaded " << loadcount << " hospitals" << endl;
		return loadcount > 0;
	}

	//load from csv with validations
	bool loadPharmacies(string filename) {
		ifstream file(filename.c_str());
		if (!file.is_open()) {
			cout << "Error: Could not open " << filename << endl;
			return false;
		}

		string line;
		getline(file, line);

		int medicineCount = 0;
		int lineNum = 1;

		while (getline(file, line)) {
			lineNum++;
			if (line.empty()) continue;

			int comma1 = -1, comma2 = -1, comma3 = -1, comma4 = -1, comma5 = -1;
			int commaCount = 0;

			for (int i = 0; i < line.length(); i++) {
				if (line[i] == ',') {
					commaCount++;
					if (commaCount == 1) comma1 = i;
					else if (commaCount == 2) comma2 = i;
					else if (commaCount == 3) comma3 = i;
					else if (commaCount == 4) comma4 = i;
					else if (commaCount == 5) comma5 = i;
				}
			}

			if (comma5 == -1) {
				cout << "Warning: Invalid pharmacy data at line " << lineNum << endl;
				continue;
			}

			string pharmacyID = StringUtils::trim(line.substr(0, comma1));
			string pharmacyName = StringUtils::trim(line.substr(comma1 + 1, comma2 - comma1 - 1));
			string sector = StringUtils::trim(line.substr(comma2 + 1, comma3 - comma2 - 1));
			string medicineName = StringUtils::trim(line.substr(comma3 + 1, comma4 - comma3 - 1));
			string formula = StringUtils::trim(line.substr(comma4 + 1, comma5 - comma4 - 1));
			string priceStr = StringUtils::trim(line.substr(comma5 + 1));

			// Validate
			if (!InputValidator::isValidString(pharmacyID) ||
				!InputValidator::isValidString(medicineName)) {
				cout << "Warning: Invalid data at line " << lineNum << endl;
				continue;
			}
			double lat = randomDoubleInRange(min_lat, max_lat);
			double lon = randomDoubleInRange(min_lon, max_lon);
			int price = StringUtils::toInt(priceStr);
			if (price < 0) price = 0;

			Pharmacy* pharmacy = (Pharmacy*)pharmacyTable.search(pharmacyID);
			if (!pharmacy) {
				pharmacy = new Pharmacy(pharmacyID, pharmacyName, sector);
				pharmacyTable.insert(pharmacyID, (void*)pharmacy);
				pharmacycount++;
				if (cityGraph) {

					string stopId = "PHA_" + pharmacyID;
					string stopName = pharmacyName + " Pharmacy Stop";
					cityGraph->addNode(stopId, stopName, lat, lon);
				}
				if (medicalGraph) {
					medicalGraph->addNode(pharmacyID, pharmacyName, lat, lon);
				}
			}

			Medicine* medicine = new Medicine(medicineName, formula, price,
				pharmacyID, pharmacyName, sector);
			pharmacy->medicines.insert((void*)medicine);
			medicineCount++;

			LinkedList* nameList = (LinkedList*)medicineByname.search(medicineName);
			if (!nameList) {
				nameList = new LinkedList();
				medicineByname.insert(medicineName, (void*)nameList);
			}
			nameList->insert((void*)medicine);

			LinkedList* formulaList = (LinkedList*)medicineByFormula.search(formula);
			if (!formulaList) {
				formulaList = new LinkedList();
				medicineByFormula.insert(formula, (void*)formulaList);
			}
			formulaList->insert((void*)medicine);
		}

		file.close();
		cout << "Loaded " << pharmacycount << " pharmacies with "
			<< medicineCount << " total medicines successfully!" << endl;
		return pharmacycount > 0;
	}

	//load from csv with validity
	bool loadDoctors(string filename) {
		ifstream file(filename.c_str());
		if (!file.is_open()) {
			cout << "Error: Could not open " << filename << endl;
			return false;
		}

		string line;
		getline(file, line); // Skip header

		int loadCount = 0;
		int lineNum = 1;

		while (getline(file, line)) {
			lineNum++;
			if (line.empty()) continue;

			int commaCount = 0;
			int comma1 = -1, comma2 = -1, comma3 = -1, comma4 = -1, comma5 = -1;
			bool inQuotes = false;

			for (int i = 0; i < line.length(); i++) {
				if (line[i] == '"') inQuotes = !inQuotes;
				if (line[i] == ',' && !inQuotes) {
					commaCount++;
					if (commaCount == 1) comma1 = i;
					else if (commaCount == 2) comma2 = i;
					else if (commaCount == 3) comma3 = i;
					else if (commaCount == 4) comma4 = i;
					else if (commaCount == 5) comma5 = i;
				}
			}

			if (comma5 == -1) {
				cout << "Warning: Invalid doctor data at line " << lineNum << endl;
				continue;
			}

			string id = StringUtils::trim(line.substr(0, comma1));
			string name = StringUtils::trim(line.substr(comma1 + 1, comma2 - comma1 - 1));
			string specialization = StringUtils::trim(line.substr(comma2 + 1, comma3 - comma2 - 1));
			string hospitalID = StringUtils::trim(line.substr(comma3 + 1, comma4 - comma3 - 1));
			string expStr = StringUtils::trim(line.substr(comma4 + 1, comma5 - comma4 - 1));
			string contact = StringUtils::trim(line.substr(comma5 + 1));

			// Validate data
			if (!InputValidator::isValidString(id) || !InputValidator::isValidString(name)) {
				cout << "Warning: Invalid doctor data at line " << lineNum << endl;
				continue;
			}

			int experience = StringUtils::toInt(expStr);
			if (experience < 0) experience = 0;

			Doctor* doctor = new Doctor(id, name, specialization, hospitalID, experience, contact);
			doctorsTable.insert(id, (void*)doctor);
			doctorCount++;
			loadCount++;

			// Add to specialization index
			LinkedList* specList = (LinkedList*)doctorsBySpecialization.search(specialization);
			if (!specList) {
				specList = new LinkedList();
				doctorsBySpecialization.insert(specialization, (void*)specList);
			}
			specList->insert((void*)doctor);
		}

		file.close();
		cout << "Loaded " << loadCount << " doctors" << endl;
		return loadCount > 0;
	}

	bool loadPatients(string filename) {
		ifstream file(filename.c_str());
		if (!file.is_open()) {
			cout << "Error: Could not open " << filename << endl;
			return false;
		}

		string line;
		getline(file, line); // Skip header

		int loadCount = 0;
		int lineNum = 1;

		while (getline(file, line)) {
			lineNum++;
			if (line.empty()) continue;

			int commaCount = 0;
			int comma1 = -1, comma2 = -1, comma3 = -1, comma4 = -1, comma5 = -1;
			bool inQuotes = false;

			for (int i = 0; i < line.length(); i++) {
				if (line[i] == '"') inQuotes = !inQuotes;
				if (line[i] == ',' && !inQuotes) {
					commaCount++;
					if (commaCount == 1) comma1 = i;
					else if (commaCount == 2) comma2 = i;
					else if (commaCount == 3) comma3 = i;
					else if (commaCount == 4) comma4 = i;
					else if (commaCount == 5) comma5 = i;
				}
			}

			if (comma5 == -1) {
				cout << "Warning: Invalid patient data at line " << lineNum << endl;
				continue;
			}

			string cnic = StringUtils::trim(line.substr(0, comma1));
			string name = StringUtils::trim(line.substr(comma1 + 1, comma2 - comma1 - 1));
			string ageStr = StringUtils::trim(line.substr(comma2 + 1, comma3 - comma2 - 1));
			string doctorID = StringUtils::trim(line.substr(comma3 + 1, comma4 - comma3 - 1));
			string diagnosis = StringUtils::trim(line.substr(comma4 + 1, comma5 - comma4 - 1));
			string admissionDate = StringUtils::trim(line.substr(comma5 + 1));

			// Validate data
			if (!InputValidator::isValidCNIC(cnic) || !InputValidator::isValidString(name)) {
				cout << "Warning: Invalid patient data at line " << lineNum << endl;
				continue;
			}

			int age = StringUtils::toInt(ageStr);
			if (!InputValidator::isValidAge(age)) {
				cout << "Warning: Invalid age at line " << lineNum << endl;
				continue;
			}

			Patient* patient = new Patient(cnic, name, age, doctorID, diagnosis, admissionDate);
			patientsTable.insert(cnic, (void*)patient);
			patientCount++;
			loadCount++;
		}

		file.close();
		cout << "Loaded " << loadCount << " patients" << endl;
		return loadCount > 0;
	}

	//displaying hospitals by traversing hashtable of hospital
	void displayAllHospitals() {
		cout << "  Displaying All Hospitals   " << endl;
		int keycount = 0;
		string* keys = hospitalTable.getAllKeys(keycount);
		if (keycount == 0) {
			cout << "No hospitals in the system " << endl;
			delete[]keys;
			return;
		}
		for (int i = 0; i < keycount; i++) {
			Hospital* hospital = (Hospital*)hospitalTable.search(keys[i]);
			if (hospital) {
				cout << i + 1 << ". " << hospital->name << endl;
				cout << "  ID: " << hospital->id;
				cout << "   Sector: " << hospital->sector << endl;
				cout << "    Emergency Beds: " << hospital->emergencyBeds << endl;
				cout << "Location: (" << hospital->latitude << ", " << hospital->longitude << ")" << endl;
				cout << "     Specializations: ";
				Node* temp = hospital->specialization.getHead();
				bool first = true;
				int specCount = 0;
				while (temp) {
					if (!first) {
						cout << ", ";
					}
					cout << *(string*)temp->data;
					first = false;
					specCount++;
					temp = temp->next;
				}
				if (specCount == 0) {
					cout << "(No Specializations)";
				}
				cout << endl;
			}
		}
		cout << " Total Hospitals: " << keycount << endl;
		cout << "---------------------------------" << endl;
		delete[]keys;
	}

	//hospitals by emergencybed availability with min heap
	void displayEmergencyBeds() {
		cout << " Emergency Bed Availability" << endl;
		cout << endl;

		MaxHeap bedHeap(100);
		int keycount = 0;
		string* keys = hospitalTable.getAllKeys(keycount);

		if (keycount == 0) {
			cout << "No Hospitals available" << endl;
			delete[]keys;
			return;
		}
		for (int i = 0; i < keycount; i++) {
			Hospital* hospital = (Hospital*)hospitalTable.search(keys[i]);
			if (hospital) {
				bedHeap.insert(hospital->emergencyBeds, (void*)hospital);
			}
		}
		cout << " Hospitals ranked by bed availability: " << endl;
		int rank = 1;
		while (!bedHeap.isEmpty()) {
			MaxHeapNode node = bedHeap.extractMax();
			Hospital* hospital = (Hospital*)node.data;

			if (hospital) {
				cout << rank << ". " << hospital->name << endl;
				rank++;
				cout << "  Sector: " << hospital->sector << endl;
				cout << "   Available Beds: " << hospital->emergencyBeds << endl;
				if (hospital->emergencyBeds >= 15) {
					cout << " Status: HIGH CAPACITY" << endl;
				}
				else if (hospital->emergencyBeds >= 10) {
					cout << "Status: MODERATE CAPACITY" << endl;
				}
				else {
					cout << "Status: LOW CAPACITY" << endl;
				}

			}
		}
		cout << "----------------------------" << endl;
		delete[]keys;
	}

	//searc medicine by hashtable then display full data of pharmacy having it
	void searchMedicineByName(string name) {
	
		if (name.empty()) {
			cout << "Error: Medicine name cannot be empty" << endl;
			return;
		}

		// Convert search term to lowercase for case-insensitive search
		string searchLower = StringUtils::toLower(name);

		LinkedList* medicines = (LinkedList*)medicineByname.search(name);

		if (!medicines || medicines->isEmpty()) {
			cout << "\nExact match not found. Searching for partial matches..." << endl;

			int keyCount = 0;
			string* allKeys = medicineByname.getAllKeys(keyCount);

			LinkedList* foundMedicines = nullptr;

			for (int i = 0; i < keyCount; i++) {
				string currentKey = allKeys[i];
				string currentLower = StringUtils::toLower(currentKey);

				// Check if search term is contained in the medicine name
				if (StringUtils::contains(currentLower, searchLower)) {
					if (!foundMedicines) {
						foundMedicines = new LinkedList();
					}

					LinkedList* medList = (LinkedList*)medicineByname.search(currentKey);
					if (medList) {
						Node* temp = medList->getHead();
						while (temp) {
							foundMedicines->insert(temp->data);
							temp = temp->next;
						}
					}
				}
			}

			delete[] allKeys;

			if (!foundMedicines || foundMedicines->isEmpty()) {
				cout << " Medicine not found: " << name << endl;
				if (foundMedicines) delete foundMedicines;
				return;
			}

			medicines = foundMedicines;
		}

		cout << "\n=========================================" << endl;
		cout << "      MEDICINE SEARCH RESULTS" << endl;
		cout << "=========================================" << endl;
		cout << "Search Term: " << name << endl;
		cout << "Found: " << medicines->getSize() << " result(s)" << endl;
		cout << "=========================================" << endl;

		Node* temp = medicines->getHead();
		int count = 1;

		while (temp) {
			Medicine* med = (Medicine*)temp->data;
			if (med) {
				cout << "\n" << count << ". MEDICINE: " << med->name << endl;
				cout << "   Formula: " << med->formula << endl;
				cout << "   Price: Rs. " << med->price << endl;
				cout << "   Available at:" << endl;
				cout << "   - Pharmacy: " << med->pharmacyName << endl;
				cout << "   - Pharmacy ID: " << med->pharmacyID << endl;
				cout << "   - Location: " << med->pharmacySector << endl;
				cout << "   ------------------------------------";
				count++;
			}
			temp = temp->next;
		}

		cout << "\n=========================================" << endl;

		if (medicines != (LinkedList*)medicineByname.search(name)) {
			delete medicines;
		}
	}

	//search medicine by formula through hashtable and print pharmacy data
	void searchMedicineByFormula(string formula) {
		if (!InputValidator::isValidString(formula)) {
			cout << " Error: Invalid Formula" << endl;
			return;
		}
		LinkedList* medicines = (LinkedList*)medicineByFormula.search(formula);
		if (!medicines || medicines->isEmpty()) {
			cout << " No Medicines By this Formula " << endl;
			return;
		}
		cout << "  Medicines With Formula: " << formula << endl;
		cout << "--------------------------------------------" << endl;
		Node* temp = medicines->getHead();
		int count = 1;
		while (temp) {
			Medicine* med = (Medicine*)temp->data;
			if (med) {
				cout << "" << count << ". " << med->name << endl;
				cout << "   Pharmacy: " << med->pharmacyName << endl;
				cout << "   Location: " << med->pharmacySector << endl;
				cout << "   Price: Rs. " << med->price << endl;
				count++;
			}
			temp = temp->next;
		}
		cout << "-------------------------------------------" << endl;
	}

	//display data through pharmacy hash table
	void displayAllPharmacies() {
		cout << "    All Pharmacies" << endl;
		int keycount = 0;
		string* keys = pharmacyTable.getAllKeys(keycount);

		if (keycount == 0) {
			cout << "No pharmacies in System" << endl;
			delete[]keys;
			return;
		}
		for (int i = 0; i < keycount; i++) {
			Pharmacy* pharmacy = (Pharmacy*)pharmacyTable.search(keys[i]);
			if (pharmacy) {
				cout  << i + 1 << ". " << pharmacy->name << endl;
				cout << "  ID: " << pharmacy->id << endl;
				cout << "   Sector: " << pharmacy->sector << endl;
				cout << "    Medicines Available: " << pharmacy->medicines.getSize() << endl;
				cout << "Medicines: ";
				Node* temp = pharmacy->medicines.getHead();
				bool first = true;
				while (temp) {
					Medicine* med = (Medicine*)temp->data;
					if (med) {
						if (!first) {
							cout << ", ";
						}
						cout << med->name;
						first = false;
					}
					temp = temp->next;
				}
				cout << endl;
			}
		}
		cout << " Total Pharmacies: " << keycount << endl;
		cout << "-----------------------------------" << endl;
		delete[]keys;
	}

	// find hospital with the specialization (case insensitive)
	void findHospitalsBySpecialization(string specialization) {
		if (!InputValidator::isValidString(specialization)) {
			cout << " Error: Invalid Specialization" << endl;
			return;
		}
		cout << " Hospitals With " << specialization << endl;
		cout << "--------------------------------------" << endl;

		int keyCount = 0;
		string* keys = hospitalTable.getAllKeys(keyCount);
		int found = 0;
		string searchlower = StringUtils::toLower(specialization);
		for (int i = 0; i < keyCount; i++) {
			Hospital* hospital = (Hospital*)hospitalTable.search(keys[i]);
			if (hospital) {
				Node* temp = hospital->specialization.getHead();
				bool hasSpec = false;
				while (temp) {
					string* spec = (string*)temp->data;
					if (spec && StringUtils::contains(*spec, specialization)) {
						hasSpec = true;
						break;
					}
					temp = temp->next;
				}
				if (hasSpec) {
					found++;
					cout  << found << ". " << hospital->name << endl;
					cout << "  Sector: " << hospital->sector << endl;
					cout << "   Emergency Beds: " << hospital->emergencyBeds << endl;
					cout << "All Specializations: ";
					Node* specTemp = hospital->specialization.getHead();
					bool first = true;
					while (specTemp) {
						if (!first) {
							cout << ", ";
						}
						cout << *(string*)specTemp->data;
						first = false;
						specTemp = specTemp->next;
					}
					cout << endl;
				}
			}
		}
		if (found == 0) {
			cout << " No Hospitals Found with this specialization" << endl;
		}
		else {
			cout << " Total Hospitals Found: " << found << endl;
		}
		cout << "-----------------------------------------" << endl;
		delete[]keys;
	}

	void findNearestHospital(double lat, double lon) {
		if (!InputValidator::isValidLatitude(lat) || !InputValidator::isValidLongitude(lon)) {
			cout << "Error: Invalid coordinates!" << endl;
			return;
		}

		cout << "     NEAREST HOSPITAL FINDER" << endl;
		cout << "========================================" << endl;
		cout << "Your Location: (" << lat << ", " << lon << ")" << endl;
		cout << "----------------------------------------" << endl;

		int keyCount = 0;
		string* keys = hospitalTable.getAllKeys(keyCount);

		if (keyCount == 0) {
			cout << "\nNo hospitals available!" << endl;
			delete[] keys;
			return;
		}

		struct HospDist {
			Hospital* hosp;
			double dist;
		};

		HospDist* results = new HospDist[keyCount];
		int resultCount = 0;

		// Calculate direct distances to all hospitals
		for (int i = 0; i < keyCount; i++) {
			Hospital* hospital = (Hospital*)hospitalTable.search(keys[i]);
			if (hospital) {
				double dist = MathUtils::calculateDistance(lat, lon,
					hospital->latitude,
					hospital->longitude);
				if (dist >= 0) {
					results[resultCount].hosp = hospital;
					results[resultCount].dist = dist;
					resultCount++;
				}
			}
		}

		for (int i = 0; i < resultCount - 1; i++) {
			for (int j = 0; j < resultCount - i - 1; j++) {
				if (results[j].dist > results[j + 1].dist) {
					HospDist temp = results[j];
					results[j] = results[j + 1];
					results[j + 1] = temp;
				}
			}
		}

		// Display top 5
		cout << "Top 5 Nearest Hospitals:" << endl;
		int displayCount = (resultCount < 5) ? resultCount : 5;

		for (int i = 0; i < displayCount; i++) {
			Hospital* h = results[i].hosp;
			cout << (i + 1) << ". " << h->name << endl;
			cout << "   Sector: " << h->sector << endl;
			cout << "   Distance: " << StringUtils::toString(results[i].dist, 2) << " km" << endl;
			cout << "   Emergency Beds: " << h->emergencyBeds << endl;
			cout << "   Location: (" << h->latitude << ", " << h->longitude << ")" << endl;
			cout << endl;
		}

		delete[] results;
		delete[] keys;

		if (resultCount == 0) {
			cout << "No reachable hospitals found!" << endl;
		}

		cout << "========================================" << endl;
	}

	void addDoctor() {
		string id, name, specialization, hospitalID, contact;
		int experience;

		cout << " ---Add New Doctor---" << endl;
		cout << "Enter Doctor ID: ";
		//cin.ignore();
		if (!SafeInput::getString(id)) {
			cout << "Error: Invalid doctor ID!" << endl;
			return;
		}

		if (doctorsTable.contains(id)) {
			cout << "Error: Doctor with ID " << id << " already exists" << endl;
			return;
		}

		cout << "Enter Doctor Name: ";
		if (!SafeInput::getString(name)) {
			cout << "Error: Invalid doctor name!" << endl;
			return;
		}

		cout << "Enter Specialization: ";
		if (!SafeInput::getString(specialization)) {
			cout << "Error: Invalid specialization!" << endl;
			return;
		}

		cout << "Enter Hospital ID: ";
		if (!SafeInput::getString(hospitalID)) {
			cout << "Error: Invalid hospital ID!" << endl;
			return;
		}

		cout << "Enter Experience (years): ";
		if (!SafeInput::getInt(experience, 0, 50)) {
			cout << "Error: Invalid experience!" << endl;
			return;
		}

		cout << "Enter Contact: ";
		//cin.ignore();
		if (!SafeInput::getString(contact)) {
			cout << "Error: Invalid contact!" << endl;
			return;
		}

		if (!SafeInput::getConfirmation("Add this doctor?")) {
			cout << "Operation cancelled." << endl;
			return;
		}

		Doctor* doctor = new Doctor(id, name, specialization, hospitalID, experience, contact);
		doctorsTable.insert(id, (void*)doctor);
		doctorCount++;

		// Add to specialization index
		LinkedList* specList = (LinkedList*)doctorsBySpecialization.search(specialization);
		if (!specList) {
			specList = new LinkedList();
			doctorsBySpecialization.insert(specialization, (void*)specList);
		}
		specList->insert((void*)doctor);

		cout << " Doctor added successfully!" << endl;
	}

	void addPatient() {
		string cnic, name, doctorID, diagnosis, admissionDate;
		int age;

		cout << "---Add New Patient---" << endl;
		cout << "Enter CNIC (xxxxx-xxxxxxx-x): ";
		//cin.ignore();
		if (!SafeInput::getString(cnic)) {
			cout << "Error: Invalid CNIC!" << endl;
			return;
		}

		if (!InputValidator::isValidCNIC(cnic)) {
			cout << "Error: Invalid CNIC format!" << endl;
			return;
		}

		if (patientsTable.contains(cnic)) {
			cout << "Error: Patient with CNIC " << cnic << " already exists" << endl;
			return;
		}

		cout << "Enter Patient Name: ";
		if (!SafeInput::getString(name)) {
			cout << "Error: Invalid patient name!" << endl;
			return;
		}

		cout << "Enter Age: ";
		if (!SafeInput::getInt(age, 0, 150)) {
			cout << "Error: Invalid age!" << endl;
			return;
		}

		//cin.ignore();
		cout << "Enter Doctor ID: ";
		if (!SafeInput::getString(doctorID)) {
			cout << "Error: Invalid doctor ID!" << endl;
			return;
		}
		if (doctorCount <= 0 || !doctorsTable.contains(doctorID)) {
			cout << "Warning: No doctor found with ID " << doctorID << endl;
			return;
		}
		cout << "Enter Diagnosis: ";
		if (!SafeInput::getString(diagnosis)) {
			cout << "Error: Invalid diagnosis!" << endl;
			return;
		}

		cout << "Enter Admission Date: ";
		if (!SafeInput::getString(admissionDate)) {
			cout << "Error: Invalid admission date!" << endl;
			return;
		}

		if (!SafeInput::getConfirmation("Add this patient?")) {
			cout << "Operation cancelled." << endl;
			return;
		}

		Patient* patient = new Patient(cnic, name, age, doctorID, diagnosis, admissionDate);
		patientsTable.insert(cnic, (void*)patient);
		patientCount++;

		cout << " Patient added successfully!" << endl;
	}

	void findDoctorsBySpecialization(string specialization) {
		if (!InputValidator::isValidString(specialization)) {
			cout << " Error: Invalid specialization" << endl;
			return;
		}

		cout << "Doctors with Specialization: " << specialization << endl;
		cout << "--------------------------------------" << endl;

		LinkedList* doctorsList = (LinkedList*)doctorsBySpecialization.search(specialization);
		if (!doctorsList || doctorsList->isEmpty()) {
			cout << "No doctors found with specialization: " << specialization << endl;
			return;
		}

		Node* temp = doctorsList->getHead();
		int count = 1;
		while (temp) {
			Doctor* doctor = (Doctor*)temp->data;
			if (doctor) {
				cout  << count << ". " << doctor->name << endl;
				cout << "    ID: " << doctor->id << endl;
				cout << "    Hospital ID: " << doctor->hospitalId << endl;
				cout << "    Experience: " << doctor->experience << " years" << endl;
				cout << "    Contact: " << doctor->contact << endl;
				count++;
			}
			temp = temp->next;
		}

		cout << " Total doctors found: " << count - 1 << endl;
		cout << "--------------------------------------" << endl;
	}
	void viewAllPatients() {
		cout << " All Patients in System" << endl;
		cout << "-----------------------" << endl;
		int keyCount = 0;
		string* keys = patientsTable.getAllKeys(keyCount);
		if (keyCount == 0) {
			cout << "No patients in the system" << endl;
			return;
		}
		for (int i = 0; i < keyCount; i++) {
			Patient* patient = (Patient*)patientsTable.search(keys[i]);
			if (patient) {
				cout  << i + 1 << ". " << patient->name << endl;
				cout << "    CNIC: " << patient->cnic << endl;
				cout << "    Age: " << patient->age << endl;
				cout << "    Doctor ID: " << patient->doctorID << endl;
				cout << "    Diagnosis: " << patient->diagnosis << endl;
				cout << "    Admission Date: " << patient->admissionDate << endl;
			}
		}
		cout << " Total Patients: " << keyCount << endl;
		cout << "-----------------------" << endl;
		delete[] keys;
	}

	void displayPatientRecord(string cnic) {
		if (!InputValidator::isValidCNIC(cnic)) {
			cout << " Error: Invalid CNIC format" << endl;
			return;
		}

		Patient* patient = (Patient*)patientsTable.search(cnic);
		if (!patient) {
			cout << " Patient record not found for CNIC: " << cnic << endl;
			return;
		}

		cout << " Patient Medical Record" << endl;
		cout << "-----------------------" << endl;
		cout << "CNIC: " << patient->cnic << endl;
		cout << "Name: " << patient->name << endl;
		cout << "Age: " << patient->age << endl;
		cout << "Doctor ID: " << patient->doctorID << endl;
		cout << "Diagnosis: " << patient->diagnosis << endl;
		cout << "Admission Date: " << patient->admissionDate << endl;

		// Display doctor info if available
		if (!patient->doctorID.empty()) {
			Doctor* doctor = (Doctor*)doctorsTable.search(patient->doctorID);
			if (doctor) {
				cout << "Attending Doctor: " << doctor->name << " (" << doctor->specialization << ")" << endl;
			}
		}

		cout << "-----------------------" << endl;
	}

	void displayAllDoctors() {
		cout << " All Doctors in System" << endl;
		cout << "-----------------------" << endl;

		int keyCount = 0;
		string* keys = doctorsTable.getAllKeys(keyCount);

		if (keyCount == 0) {
			cout << "No doctors in the system" << endl;
			return;
		}

		for (int i = 0; i < keyCount; i++) {
			Doctor* doctor = (Doctor*)doctorsTable.search(keys[i]);
			if (doctor) {
				cout  << i + 1 << ". " << doctor->name << endl;
				cout << "    ID: " << doctor->id << endl;
				cout << "    Specialization: " << doctor->specialization << endl;
				cout << "    Hospital ID: " << doctor->hospitalId << endl;
				cout << "    Experience: " << doctor->experience << " years" << endl;
				cout << "    Contact: " << doctor->contact << endl;
			}
		}

		cout << " Total Doctors: " << keyCount << endl;
		cout << "-----------------------" << endl;
		delete[] keys;
	}

	void addHospital() {

		if (!cityGraph) {
			cout << "Error: City graph has not been initialized yet" << endl;
			return;
		}

		string id, name, sector;
		int beds;
		double lat, lon;
		cout << " ---Add New Hospital---" << endl;
		cout << "Enter Hospital Id:" << endl;
		cin >> id;
		cin.ignore();
		if (!InputValidator::isValidString(id)) {
			cout << "Error: Invalid Hospital Id" << endl;
			return;
		}
		if (hospitalTable.contains(id)) {
			cout << "Error: Hospital with id " << id << " already exists" << endl;
			return;
		}
		cout << "Enter Hospital Name: ";
		if (!SafeInput::getString(name)) {
			cout << "Error: Invalid hospital name!" << endl;
			return;
		}

		cout << "Enter Sector: ";
		if (!SafeInput::getString(sector)) {
			cout << "Error: Invalid sector!" << endl;
			return;
		}

		cout << "Enter Emergency Beds: ";
		if (!SafeInput::getInt(beds, 0, 1000)) {
			cout << "Error: Invalid bed count!" << endl;
			return;
		}
		cout << "Enter Latitude (-90 to 90): ";
		if (!SafeInput::getDouble(lat, -90.0, 90.0)) {
			cout << "Error: Invalid latitude!" << endl;
			return;
		}
		cout << "Enter Longitude (-180 to 180): ";
		if (!SafeInput::getDouble(lon, -180.0, 180.0)) {
			cout << "Error: Invalid longitude!" << endl;
			return;
		}
		if (!SafeInput::getConfirmation("Add this hospital?")) {
			cout << "Operation cancelled." << endl;
			return;
		}

		if (!cityGraph->hasFreeSlots()) {
			cout << "Error: Cannot have more buildings in the city" << endl;
			return;
		}
		string transportID = "HOS_" + id;
		if (cityGraph->containsLocationId(id)) {
			cout << "Error: City already has a building with ID " << id << endl;
			return;
		}
		if (cityGraph->hasNonStopAtLocation(lat, lon)) {
			cout << "Error: City already has a building at (" << lat << ", " << lon << ")" << endl;
			return;
		}

		if (!cityGraph->addLocation(transportID, name + " Hospital Stop", lat, lon)) {
			cout << "Error: Hospital could not be added to the city transport network" << endl;
			return;
		}

		Hospital* hospital = new Hospital(id, name, sector, beds);
		hospital->latitude = lat;
		hospital->longitude = lon;
		hospitalTable.insert(id, (void*)hospital);
		hospitalcount++;

		//Add to graph
		if (medicalGraph) {
			medicalGraph->addNode(id, name, lat, lon);
			cout << " Hospital added successfully" << endl;
		}
	}
	void addpharmacy() {
		string id, name, sector;
		double lat, lon;
		cout << " ---Add New Pharmacy---" << endl;
		cout << "Enter Pharmacy Id:" << endl;
		cin >> id;
		cin.ignore();
		if (!InputValidator::isValidString(id)) {
			cout << "Error: Invalid Pharmacy Id" << endl;
			return;
		}
		if (pharmacyTable.contains(id)) {
			cout << "Error: Pharmacy with id " << id << " already exists" << endl;
			return;
		}
		cout << "Enter Pharmacy Name: ";
		if (!SafeInput::getString(name)) {
			cout << "Error: Invalid pharmacy name!" << endl;
			return;
		}
		cout << "Enter Sector: ";
		if (!SafeInput::getString(sector)) {
			cout << "Error: Invalid sector!" << endl;
			return;
		}
		cout << "Enter Latitude (-90 to 90): ";
		if (!SafeInput::getDouble(lat, -90.0, 90.0)) {
			cout << "Error: Invalid latitude!" << endl;
			return;
		}
		cout << "Enter Longitude (-180 to 180): ";
		if (!SafeInput::getDouble(lon, -180.0, 180.0)) {
			cout << "Error: Invalid longitude!" << endl;
			return;
		}
		if (!SafeInput::getConfirmation("Add this pharmacy?")) {
			cout << "Operation cancelled." << endl;
			return;
		}
		Pharmacy* pharmacy = new Pharmacy(id, name, sector);
		pharmacyTable.insert(id, (void*)pharmacy);
		pharmacycount++;
		cout << " Pharmacy added successfully" << endl;
		if (cityGraph) {
			string stopId = "PHA_" + id;
			string stopName = name + " Pharmacy Stop";
			cityGraph->addNode(stopId, stopName, lat, lon);

			cout << "Pharmacy added to transport network as: " << stopId << endl;
		}
		if (medicalGraph) {
			medicalGraph->addNode(id, name, lat, lon);
		}
	}
	void addMedicineToPharmacy() {
		string pharmacyID, medName, formula;
		int price;
		cout << " ---Add Medicine to Pharmacy---" << endl;
		cout << "Enter Pharmacy Id:" << endl;
		cin >> pharmacyID;
		cin.ignore();
		Pharmacy* pharmacy = (Pharmacy*)pharmacyTable.search(pharmacyID);
		if (!pharmacy) {
			cout << "Error: Pharmacy with id " << pharmacyID << " not found" << endl;
			return;
		}
		cout << "Enter Medicine Name: ";
		if (!SafeInput::getString(medName)) {
			cout << "Error: Invalid medicine name!" << endl;
			return;
		}
		cout << "Enter Formula: ";
		if (!SafeInput::getString(formula)) {
			cout << "Error: Invalid formula!" << endl;
			return;
		}
		cout << "Enter Price: ";
		if (!SafeInput::getInt(price, 0, 100000)) {
			cout << "Error: Invalid price!" << endl;
			return;
		}
		if (!SafeInput::getConfirmation("Add this medicine to the pharmacy?")) {
			cout << "Operation cancelled." << endl;
			return;
		}
		Medicine* medicine = new Medicine(medName, formula, price,
			pharmacyID, pharmacy->name, pharmacy->sector);
		pharmacy->medicines.insert((void*)medicine);
		LinkedList* nameList = (LinkedList*)medicineByname.search(medName);
		if (!nameList) {
			nameList = new LinkedList();
			medicineByname.insert(medName, (void*)nameList);
		}
		nameList->insert((void*)medicine);
		LinkedList* formulaList = (LinkedList*)medicineByFormula.search(formula);
		if (!formulaList) {
			formulaList = new LinkedList();
			medicineByFormula.insert(formula, (void*)formulaList);
		}
		formulaList->insert((void*)medicine);
		cout << " Medicine added successfully to the pharmacy" << endl;
	}

	int getTotalHospitals()const {
		return hospitalcount;
	}
	int getTotalPharmacies()const {
		return pharmacycount;
	}
	int getTotalDoctors()const {
		return doctorCount;
	}
	int getTotalPatients()const {
		return patientCount;
	}

	HashTable& getHospitalTable() {
		return hospitalTable;
	}
};