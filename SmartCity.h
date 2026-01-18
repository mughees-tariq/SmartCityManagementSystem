#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

#include "Graph.h"
#include "TransportSector.h"
#include "EducationSector.h"
#include "MedicalSector.h"
#include "CommercialSector.h"
#include "Facilities.h"
#include "AirportSector.h"
#include "RailwaySector.h"
#include "PopulationSector.h"


class SmartCity {
private:
    Graph* cityGraph;

    TransportSector* transportSector;
    SchoolSystem* educationSector;
    MedicalSystem* medicalSector;
    CommercialSystem* commercialSector;
    FacilitySystem* publicFacilities;
    AirportSystem* airportSector;
    RailwaySystem* railwaySector;
    HousingSystem* populationSector;

    bool isActive;
    bool loadData;

    void mainMenu() {
        cout << endl;
        cout << "========================================" << endl;
        cout << "   SMART CITY MANAGEMENT SYSTEM " << endl;
        cout << "========================================" << endl;
        cout << "1. Medical Sector" << endl;
        cout << "2. Facility Sector" << endl;
        cout << "3. Commercial Sector" << endl;
        cout << "4. Airport Sector" << endl;
        cout << "5. Railway Sector" << endl;
        cout << "6. Education Sector" << endl;
        cout << "7. Population Sector" << endl;
        cout << "8. Transport Sector" << endl;
        cout << "9. Exit" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice (1-9): ";
    }
    void transportMenu() {
        cout << endl;
        cout << "--- TRANSPORT SECTOR---" << endl;
        cout << "1.  Add Bus Stop" << endl;
        cout << "2.  Connect Stops" << endl;
        cout << "3.  Add Bus" << endl;
        cout << "4.  Update Bus Location" << endl;
        cout << "5.  Find Nearest Bus to a Stop" << endl;
        cout << "6.  Shortest Path between Two Stops" << endl;
        cout << "7.  Display Transport Graph" << endl;
        cout << "8.  Display All Buses" << endl;
        cout << "9.  Display Bus Hash Table State" << endl;
        cout << "10. Initialize Passenger Queue at Stop" << endl;
        cout << "11. Add Passengers to Queue" << endl;
        cout << "12. Board Passengers from Stop" << endl;
        cout << "13. Display Passenger Queue at Stop" << endl;
        cout << "14. Display Travel History" << endl;
        cout << "15. Remove last Travel Record" << endl;
        cout << "16. Clear Travel History" << endl;
        cout << "17. Journey Simulation (Bus from point A to B)" << endl;
        cout << "18. Real-Time Route Simulation" << endl;
        cout << "19. Emergency Route to Nearest Hospital" << endl;
        cout << "20. Display School Buses" << endl;
        cout << "21. Find School Bus by School" << endl;
        cout << "22. Find School Buses by Sector" << endl;
        cout << "23. Track School Bus Journey" << endl;
        cout << "24. Display All Stops" << endl;
        cout << "25. Add New Stop To A Bus" << endl;
        cout << "26. Remove Stop from Bus Route" << endl;  
        cout << "27. Back to main menu" << endl;        
        cout << "========================================" << endl;
        cout << "Enter your choice (1-27): ";             
    }
    void educationMenu() {
        cout << endl;
        cout << "--- EDUCATION SECTOR MANAGEMENT ---" << endl;
        cout << "1.  Display Full Organogram" << endl;
        cout << "2.  Search Student by ID" << endl;
        cout << "3.  Search Faculty by ID" << endl;
        cout << "4.  Search School by ID" << endl;
        cout << "5.  Search Schools by Subject" << endl;
        cout << "6.  Find Nearest School (by ID)" << endl;
        cout << "7.  Find Nearest School (by Location)" << endl;
        cout << "8.  Show Sector Coordinate Ranges" << endl;
        cout << "9.  Add New School" << endl;
        cout << "10. Add New Student" << endl;
        cout << "11. Add New Faculty" << endl;
        cout << "12. Show all Schools" << endl;
        cout << "13. Display Schools by ranking" << endl;
        cout << "14. Back to main menu" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice (1-13): ";
    }
    void medicalMenu() {
        cout << endl;
        cout << "--- MEDICAL SECTOR ---" << endl;
        cout << "1.  View All Hospitals" << endl;
        cout << "2.  Check Emergency Bed Availability (Max-Heap)" << endl;
        cout << "3.  Search Medicine by Name" << endl;
        cout << "4.  Search Medicine by Formula" << endl;
        cout << "5.  View All Pharmacies" << endl;
        cout << "6.  Find Hospital by Specialization" << endl;
        cout << "7.  Find Nearest Hospital" << endl;
        cout << "8.  View All Doctors" << endl;
        cout << "9.  View All Patients" << endl;
        cout << "10. Find Doctors by Specialization" << endl;
        cout << "11. View Patient Record" << endl;
        cout << "12. Add New Hospital" << endl;
        cout << "13. Add New Pharmacy" << endl;
        cout << "14. Add New Medicine To A Pharmacy" << endl;
        cout << "15. Add New Doctor" << endl;
        cout << "16. Add New Patient" << endl;
        cout << "17. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice (1-17): ";
    }
    void commercialMenu() {
        cout << endl;
        cout << "--- COMMERCIAL SECTOR ---" << endl;
        cout << "1. Display All Malls" << endl;
        cout << "2. Display Products in a Mall" << endl;
        cout << "3. Search Product by Name" << endl;
        cout << "4. Search Products by Category" << endl;
        cout << "5. Find Nearest Mall" << endl;
        cout << "6. Add New Mall" << endl;
        cout << "7. Add Product to Mall" << endl;
        cout << "8. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice (1-8): ";
    }
    void facilityMenu() {
        cout << endl;
        cout << "--- Facility Sector ---" << endl;
        cout << "1. Display All Facilities" << endl;
        cout << "2. Add Facility" << endl;
        cout << "3. Find Nearest Facility By Type" << endl;
        cout << "4. Find Nearest Facility (Any Type)" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice (1-5): ";
    }
    void airportMenu() {
        cout << endl;
        cout << "--- Airport Sector ---" << endl;
        cout << "1. Display All Airports " << endl;
        cout << "2. Display All Flights" << endl;
        cout << "3. Add Flight" << endl;
        cout << "4. Find Shortest Flight" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice (1-5): ";
    }
    void railwayMenu() {
        cout << endl;
        cout << "--- Railway Sector ---" << endl;
        cout << "1. Display All Railway Stations " << endl;
        cout << "2.  Display All Journeys" << endl;
        cout << "3.  Display Journeys By Railway Station" << endl;
        cout << "4.  Add Railway Station" << endl;
        cout << "5.  Add Journey" << endl;
        cout << "6.  Find Nearest Station" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice (1-7): ";
    }
    void populationMenu() {
        cout << endl;
        cout << "=== SMART CITY HOUSING MANAGEMENT SYSTEM ===" << endl;
        cout << "1.  Add New House" << endl;
        cout << "2.  Add Family to House" << endl;
        cout << "3.  Add Member to Family" << endl;
        cout << "4.  Search Citizen by CNIC" << endl;
        cout << "5.  Search Citizen by Name" << endl;
        cout << "6.  View Houses in Sector" << endl;
        cout << "7.  Display Housing Hierarchy" << endl;
        cout << "8.  Load Data from File" << endl;  // NEW OPTION
        cout << "9.  Generate Survey Reports" << endl;
        cout << "10. Display Statistics" << endl;
        cout << "11. Back to Main Menu" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice (1-11): ";
    }

    void loadSectorsData() {
        if (!loadData) return;

        // ===== TRANSPORT SECTOR =====
        cout << "=== Loading Transport Data ===" << endl;
        transportSector->loadBusStops("Data/stops.csv");
        transportSector->loadBuses("Data/buses.csv");

        // ===== EDUCATION SECTOR =====
        educationSector->LoadFromFile("Data/schools.csv");
        cout << "Education Sector: Loaded " << educationSector->GetSchoolCount() << " schools from file" << endl;
        for (int i = 0; i < educationSector->GetSchoolCount(); i++) {
            SchoolNode* school = educationSector->GetSchool(i);
            if (!school) continue;

            string originalId = school->GetSchoolID();
            string stopId = "SCH_" + school->GetSchoolID();
            if (!cityGraph->containsLocationId(originalId) &&
                !cityGraph->containsLocationId(stopId)) {

                string stopName = school->GetName() + " School Stop";
                transportSector->addStop(stopId, stopName,
                    school->GetLatitude(), school->GetLongitude());
            }
        }

        // ===== MEDICAL SECTOR =====
        medicalSector->loadHospitals("Data/hospitals.csv");
        medicalSector->loadPharmacies("Data/pharmacies.csv");
        medicalSector->loadPatients("Data/patients.csv");
        medicalSector->loadDoctors("Data/doctors.csv");
        cout << "Medical Sector: Loaded " << medicalSector->getTotalHospitals() << " hospitals, "
            << medicalSector->getTotalPharmacies() << " pharmacies, " << medicalSector->getTotalDoctors() << " doctors, "
            << medicalSector->getTotalPatients() << " patients" << endl;

        int keyCount = 0;
        string* keys = medicalSector->getHospitalTable().getAllKeys(keyCount);
        for (int i = 0; i < keyCount; i++) {
            Hospital* hospital = (Hospital*)medicalSector->getHospitalTable().search(keys[i]);
            if (!hospital) continue;
            string originalId = hospital->id;
            string stopId = "HOS_" + hospital->id;
            if (!cityGraph->containsLocationId(originalId) &&
                !cityGraph->containsLocationId(stopId)) {

				string stopName = hospital->name + " Hospital Stop";
                transportSector->addStop(stopId, stopName,
					hospital->latitude, hospital->longitude);
            }
        }
        if (keys) delete[] keys;

        // ===== POPULATION SECTOR =====  ADD THIS SECTION
        cout << "=== Loading Population Data ===" << endl;
        populationSector->LoadFromFile("Data/population.csv");
        cout << "=== Loading Population Houses to Graph ===" << endl;
        populationSector->AddAllHousesToGraph();

        // Add house stops to transport graph

        HouseNode* currentHouse = populationSector->GetHead();  // You need to add GetHead() method to HousingSystem
        int houseStopCount = 0;
        while (currentHouse != nullptr) {
            string stopId = currentHouse->GetHouseID();
            if (stopId != "") {
                string stopName = "House " + to_string(currentHouse->GetHouseNo()) +
                    " - " + currentHouse->GetStreet();
                transportSector->addStop(stopId, stopName,
                    currentHouse->GetLatitude(), currentHouse->GetLongitude());
                houseStopCount++;
            }
            currentHouse = currentHouse->GetNext();
        }
        cout << "Added " << houseStopCount << " house stops to transport graph" << endl;

        loadData = false;
    }
public:

    SmartCity() {
        srand(time(0));
        isActive = true;
        loadData = true;
        cityGraph = new Graph;

        transportSector = new TransportSector(cityGraph);
        educationSector = new SchoolSystem(cityGraph);
        medicalSector = new MedicalSystem(cityGraph);
        commercialSector = new CommercialSystem(cityGraph);
        publicFacilities = new FacilitySystem(cityGraph);
        airportSector = new AirportSystem(cityGraph);
        railwaySector = new RailwaySystem(cityGraph);
        populationSector = new HousingSystem(cityGraph); 

        loadSectorsData();
    }
    ~SmartCity() {
        delete transportSector;
        delete educationSector;
        delete medicalSector;
        delete commercialSector;
        delete publicFacilities;
        delete airportSector;
        delete railwaySector;
        delete populationSector; 
        delete cityGraph;
    }

    void run() {

        while (isActive) {
            mainMenu();
            int choice; cin >> choice;

            if (choice == 9) {
                isActive = false;
                cout << "Exiting the system..." << endl;
            }
            else if (choice == 1) {

                //Medical Sector:
                bool back = false;
                while (!back) {

                    medicalMenu();
                    int choice; cin >> choice;
                    cin.ignore();

                    if (choice == 17) back = true;
                    else if (choice == 1) {

                        medicalSector->displayAllHospitals();
                    }
                    else if (choice == 2) {

                        medicalSector->displayEmergencyBeds();
                    }
                    else if (choice == 3) {

                        string name;
                        cout << endl << "Enter Medicine Name: ";
                        cin.ignore();
                        getline(cin, name);
                        medicalSector->searchMedicineByName(name);
                    }
                    else if (choice == 4) {

                        string formula;
                        cout << endl << "Enter Medicine Formula: ";
                        cin.ignore();
                        getline(cin, formula);
                        medicalSector->searchMedicineByFormula(formula);
                    }
                    else if (choice == 5) {

                        medicalSector->displayAllPharmacies();
                    }
                    else if (choice == 6) {

                        string specialization;
                        cout << endl << "Enter Specialization: ";
                        cin.ignore();
                        getline(cin, specialization);
                        medicalSector->findHospitalsBySpecialization(specialization);
                    }
                    else if (choice == 7) {

                        double lat, lon;
                        cout << endl << "Enter Your Latitude (-90 to 90): ";
                        if (!SafeInput::getDouble(lat, -90, 90)) {
                            cout << "Invalid latitude!" << endl;
                            continue;
                        }
                        cout << "Enter Your Longitude (-180 to 180): ";
                        if (!SafeInput::getDouble(lon, -180, 180)) {
                            cout << "Invalid longitude!" << endl;
                            continue;
                        }
                        cin.ignore();
                        medicalSector->findNearestHospital(lat, lon);
                    }
                    else if (choice == 8) {

                        medicalSector->displayAllDoctors();
                    }
                    else if (choice == 9) {

                        medicalSector->viewAllPatients();
                    }
                    else if (choice == 10) {

                        string specialization;
                        cout << endl << "Enter Doctor Specialization: ";
                        cin.ignore();
                        getline(cin, specialization);
                        medicalSector->findDoctorsBySpecialization(specialization);
                    }
                    else if (choice == 11) {

                        string cnic;
                        cout << endl << "Enter Patient CNIC (xxxxx-xxxxxxx-x): ";
                        cin.ignore();
                        getline(cin, cnic);
                        medicalSector->displayPatientRecord(cnic);
                    }
                    else if (choice == 12) {

                        medicalSector->addHospital();
                    }
                    else if (choice == 13) {

                        medicalSector->addpharmacy();
                    }
                    else if (choice == 14) {

                        medicalSector->addMedicineToPharmacy();
                    }
                    else if (choice == 15) {

                        medicalSector->addDoctor();
                    }
                    else if (choice == 16) {

                        medicalSector->addPatient();
                    }
                    else {
                        cout << "Invalid Input" << endl;
                    }
                }
            }
            else if (choice == 2) {

                //Facilities Sector:
                bool back = false;
                while (!back) {

                    facilityMenu();
                    int choice; cin >> choice;
                    cin.ignore();

                    if (choice == 5) back = true;
                    else if (choice == 1) {

                        publicFacilities->displayAllFacilities();
                    }
                    else if (choice == 2) {

                        publicFacilities->addFacility();
                    }
                    else if (choice == 3) {

                        string type;
                        double lat, lon;
                        cout << "Enter facility type (e.g., Mosque, Park, Water): ";
                        cin.ignore();
                        getline(cin, type);

                        cout << "Enter latitude: ";
                        if (!SafeInput::getDouble(lat, -90, 90)) {
                            cout << "Invalid latitude" << endl;
                            continue;
                        }
                        cout << "Enter longitude: ";
                        if (!SafeInput::getDouble(lon, -180, 180)) {
                            cout << "Invalid longitude" << endl;
                            continue;
                        }
                        publicFacilities->findNearestFacility(lat, lon, type);
                    }
                    else if (choice == 4) {

                        double lat, lon;
                        cout << "Enter latitude: ";
                        if (!SafeInput::getDouble(lat, -90, 90)) {
                            cout << "Invalid latitude" << endl;
                            continue;
                        }
                        cout << "Enter longitude: ";
                        if (!SafeInput::getDouble(lon, -180, 180)) {
                            cout << "Invalid longitude" << endl;
                            continue;
                        }
                        publicFacilities->findNearestAnyFacility(lat, lon);
                    }
                    else {
                        cout << "Invalid Input" << endl;
                    }
                }
            }
            else if (choice == 3) {

                //Commercial Sector:
                bool back = false;
                while (!back) {

                    commercialMenu();
                    int choice; cin >> choice;
                    cin.ignore();

                    if (choice == 8) back = true;
                    else if (choice == 1) {

                        commercialSector->displayAllMalls();
                    }
                    else if (choice == 2) {

                        string mallID;
                        cout << endl << "Enter Mall ID: ";
                        cin.ignore();
                        getline(cin, mallID);
                        commercialSector->displayProductsInMall(mallID);
                    }
                    else if (choice == 3) {

                        string productName;
                        cout << endl << "Enter Product Name: ";
                        cin.ignore();
                        getline(cin, productName);
                        commercialSector->SearchProductByName(productName);
                    }
                    else if (choice == 4) {

                        string category;
                        cout << endl << "Enter Category: ";
                        cin.ignore();
                        getline(cin, category);
                        commercialSector->searchByCategory(category);
                    }
                    else if (choice == 5) {

                        double lat, lon;
                        cout << endl << "Enter Your Latitude (-90 to 90): ";
                        if (!SafeInput::getDouble(lat, -90, 90)) {
                            cout << "Invalid latitude!" << endl;
                            continue;
                        }
                        cout << "Enter Your Longitude (-180 to 180): ";
                        if (!SafeInput::getDouble(lon, -180, 180)) {
                            cout << "Invalid longitude!" << endl;
                            continue;
                        }
                        cin.ignore();
                        commercialSector->findNearestMall(lat, lon);
                    }
                    else if (choice == 6) {

                        commercialSector->addMall();
                    }
                    else if (choice == 7) {

                        commercialSector->addProduct();
                    }
                    else {
                        cout << "Invalid Input" << endl;
                    }
                }
            }
            else if (choice == 4) {

                //Airport Sector:
                bool back = false;
                while (!back) {

                    airportMenu();
                    int choice; cin >> choice;
                    cin.ignore();

                    if (choice == 5) back = true;
                    else if (choice == 1) {
                        airportSector->displayAllAirports();
                    }
                    else if (choice == 2) {
                        airportSector->displayAllFlights();
                    }
                    else if (choice == 3) {
                        airportSector->addFlight();
                    }
                    else if (choice == 4) {
                        string destcity;
						cout << "Enter Destination City: ";
                        getline(cin, destcity);
                        airportSector->FindSuitableFlightByTime(destcity);
                    }
                    else {
                        cout << "Invalid Input" << endl;
                    }
                }
            }
            else if (choice == 5) {

                //Railway Sector:
                bool back = false;
                while (!back) {

                    railwayMenu();
                    int choice; cin >> choice;
                    cin.ignore();

                    if (choice == 7) back = true;
                    else if (choice == 1) {

                        railwaySector->displayAllRailwayStations();
                    }
                    else if (choice == 2) {

                        railwaySector->displayAllJourneys();
                    }
                    else if (choice == 3) {

                        string id;
                        cout << "Enter Railway Station ID: "; cin >> id;
                        railwaySector->displayJourneysByRailwayStation(id);
                    }
                    else if (choice == 4) {

                        railwaySector->addRailwayStation();
                    }
                    else if (choice == 5) {

                        railwaySector->addJourney();
                    }
                    else if (choice == 6) {

                        double lat, lon;
                        cout << "Enter current latitudes: "; cin >> lat;
                        cout << "Enter current longitudes: "; cin >> lon;
                        railwaySector->findNearestRailwayStation(lat, lon);
                    }
                    else {
                        cout << "Invalid Input" << endl;
                    }
                }
            }
            else if (choice == 6) {

                //Education Sector:
                bool back = false;
                while (!back) {

                    educationMenu();
                    int choice; cin >> choice;
                    cin.ignore();

                    if (choice == 14)
                        back = true;
                    else if (choice == 1) {

                        educationSector->DisplayOrganogram();
                    }
                    else if (choice == 2) {

                        cout << "Enter Student ID: ";
                        string id;
                        getline(cin, id);

                        if (id == "") {
                            cout << "Student ID cannot be empty." << endl;
                            continue;
                        }

                        StudentTableNode* student = educationSector->FindStudent(id);
                        if (student) {
                            cout << "Student Found." << endl;
                            cout << "Name: " << student->GetName() << endl;
                            cout << "Age: " << student->GetAge() << endl;
                            cout << "ID: " << student->GetID() << endl;
                            cout << "School ID: " << student->GetSchoolID() << endl;
                            cout << "Department: " << student->GetDeptName() << endl;
                            cout << "Class: " << student->GetClassName() << endl;
                        }
                        else cout << "Student not found." << endl;
                    }
                    else if (choice == 3) {

                        cout << "Enter Faculty ID: ";
                        string id;
                        getline(cin, id);

                        if (id == "") {
                            cout << "Faculty ID cannot be empty." << endl;
                            continue;
                        }

                        FacultyTableNode* faculty = educationSector->FindFaculty(id);
                        if (faculty) {
                            cout << "Faculty Found." << endl;
                            cout << "Name: " << faculty->GetName() << endl;
                            cout << "ID: " << faculty->GetID() << endl;
                            cout << "Specialization: " << faculty->GetSpecialization() << endl;
                            cout << "School ID: " << faculty->GetSchoolID() << endl;
                            cout << "Department: " << faculty->GetDeptName() << endl;
                        }
                        else cout << "Faculty not found." << endl;
                    }
                    else if (choice == 4) {

                        cout << "Enter School ID: ";
                        string id;
                        getline(cin, id);

                        if (id == "") {
                            cout << "School ID cannot be empty." << endl;
                            continue;
                        }

                        SchoolNode* school = educationSector->FindSchool(id);
                        if (school) {
                            cout << "School Found." << endl;
                            cout << "Name: " << school->GetName() << endl;
                            cout << "ID: " << school->GetSchoolID() << endl;
                            cout << "Sector: " << school->GetSector() << endl;
                            cout << "Rating: " << school->GetRating() << endl;
                            cout << "Location: " << school->GetLatitude() << ", " << school->GetLongitude() << endl;
                        }
                        else cout << "School not found." << endl;
                    }
                    else if (choice == 5) {

                        cout << "Enter subject name: ";
                        string subject;
                        getline(cin, subject);

                        if (subject == "") {
                            cout << "Subject cannot be empty." << endl;
                            continue;
                        }
                        educationSector->SearchBySubject(subject);
                    }
                    else if (choice == 6) {

                        cout << "Enter School ID: ";
                        string id;
                        getline(cin, id);

                        if (id == "") {
                            cout << "School ID cannot be empty." << endl;
                            continue;
                        }

                        SchoolNode* school = educationSector->FindSchool(id);
                        if (!school) {
                            cout << "School not found." << endl;
                            continue;
                        }

                        SchoolNode* nearest = educationSector->FindNearestSchoolByID(id);
                        if (nearest) {
                            cout << "Nearest school: " << nearest->GetName() << endl;
                            cout << "ID: " << nearest->GetSchoolID() << endl;
                            cout << "Sector: " << nearest->GetSector() << endl;
                            cout << "Rating: " << nearest->GetRating() << endl;
                        }
                        else cout << "No other schools found." << endl;
                    }
                    else if (choice == 7) {

                        cout << "Enter latitude: ";
                        float lat;
                        if (!(cin >> lat)) {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "Invalid latitude." << endl;
                            continue;
                        }
                        cout << "Enter longitude: ";
                        float lon;
                        if (!(cin >> lon)) {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "Invalid longitude." << endl;
                            continue;
                        }
                        cin.ignore(1000, '\n');

                        SchoolNode* nearest = educationSector->FindNearestSchoolByLocation(lat, lon);
                        if (nearest) {
                            cout << "Nearest school: " << nearest->GetName() << endl;
                            cout << "ID: " << nearest->GetSchoolID() << endl;
                            cout << "Sector: " << nearest->GetSector() << endl;
                            cout << "Rating: " << nearest->GetRating() << endl;
                            cout << "Location: " << nearest->GetLatitude() << ", " << nearest->GetLongitude() << endl;
                        }
                        else cout << "No schools found near this location." << endl;
                    }
                    else if (choice == 8) {

                        educationSector->ShowSectorCoordinateRanges();
                    }
                    else if (choice == 9) {

                        string id, name, sector;
                        float lat, lon, rating;

                        cout << "Enter School ID: ";
                        getline(cin, id);
                        if (id == "") {
                            cout << "School ID cannot be empty." << endl;
                            continue;
                        }

                        if (educationSector->FindSchool(id)) {
                            cout << "School with this ID already exists." << endl;
                            continue;
                        }

                        cout << "Enter School Name: ";
                        getline(cin, name);
                        if (name == "") {
                            cout << "School name cannot be empty." << endl;
                            continue;
                        }

                        cout << "Enter Sector: ";
                        getline(cin, sector);
                        if (sector == "") {
                            cout << "Sector cannot be empty." << endl;
                            continue;
                        }

                        cout << "Enter Latitude: ";
                        if (!(cin >> lat)) {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "Invalid latitude." << endl;
                            continue;
                        }

                        cout << "Enter Longitude: ";
                        if (!(cin >> lon)) {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "Invalid longitude." << endl;
                            continue;
                        }

                        cout << "Enter Rating: ";
                        if (!(cin >> rating)) {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "Invalid rating." << endl;
                            continue;
                        }
                        cin.ignore(1000, '\n');

                        if (rating < 0 || rating > 10) {
                            cout << "Rating must be between 0 and 10." << endl;
                            continue;
                        }

                        educationSector->AddSchool(id, name, sector, lat, lon, rating);
                    }
                    else if (choice == 10) {

                        string schoolID, deptName, className, studentName, studentID;
                        int age;

                        cout << "Enter School ID: ";
                        getline(cin, schoolID);
                        if (schoolID == "") {
                            cout << "School ID cannot be empty." << endl;
                            continue;
                        }

                        SchoolNode* school = educationSector->FindSchool(schoolID);
                        if (!school) {
                            cout << "School not found." << endl;
                            continue;
                        }

                        cout << "School: " << school->GetName() << endl;
                        cout << "Enter Department Name: ";
                        getline(cin, deptName);
                        if (deptName == "") {
                            cout << "Department name cannot be empty." << endl;
                            continue;
                        }

                        cout << "Enter Class Name: ";
                        getline(cin, className);
                        if (className == "") {
                            cout << "Class name cannot be empty." << endl;
                            continue;
                        }

                        cout << "Enter Student Name: ";
                        getline(cin, studentName);
                        if (studentName == "") {
                            cout << "Student name cannot be empty." << endl;
                            continue;
                        }

                        cout << "Enter Student Age: ";
                        if (!(cin >> age)) {
                            cin.clear();
                            cin.ignore(1000, '\n');
                            cout << "Invalid age." << endl;
                            continue;
                        }
                        cin.ignore(1000, '\n');

                        if (age < 3 || age > 60) {
                            cout << "Age must be between 3 and 60." << endl;
                            continue;
                        }

                        cout << "Enter Student ID: ";
                        getline(cin, studentID);
                        if (studentID == "") {
                            cout << "Student ID cannot be empty." << endl;
                            continue;
                        }

                        if (educationSector->FindStudent(studentID)) {
                            cout << "Student with this ID already exists." << endl;
                            continue;
                        }

                        educationSector->AddStudent(schoolID, deptName, className, studentName, age, studentID);
                        cout << "Student added." << endl;
                    }
                    else if (choice == 11) {

                        string schoolID, deptName, facultyName, facultyID, specialization;

                        cout << "Enter School ID: ";
                        getline(cin, schoolID);
                        if (schoolID == "") {
                            cout << "School ID cannot be empty." << endl;
                            continue;
                        }

                        SchoolNode* school = educationSector->FindSchool(schoolID);
                        if (!school) {
                            cout << "School not found." << endl;
                            continue;
                        }

                        cout << "School: " << school->GetName() << endl;
                        cout << "Enter Department Name: ";
                        getline(cin, deptName);
                        if (deptName == "") {
                            cout << "Department name cannot be empty." << endl;
                            continue;
                        }

                        cout << "Enter Faculty Name: ";
                        getline(cin, facultyName);
                        if (facultyName == "") {
                            cout << "Faculty name cannot be empty." << endl;
                            continue;
                        }

                        cout << "Enter Faculty ID: ";
                        getline(cin, facultyID);
                        if (facultyID == "") {
                            cout << "Faculty ID cannot be empty." << endl;
                            continue;
                        }

                        if (educationSector->FindFaculty(facultyID)) {
                            cout << "Faculty with this ID already exists." << endl;
                            continue;
                        }

                        cout << "Enter Specialization: ";
                        getline(cin, specialization);
                        if (specialization == "") {
                            cout << "Specialization cannot be empty." << endl;
                            continue;
                        }

                        educationSector->AddFaculty(schoolID, deptName, facultyName, facultyID, specialization);
                        cout << "Faculty added." << endl;
                    }
                    else if (choice == 12) {

                        educationSector->DisplayAllSchoolsFromHash();
                    }
                    else if(choice == 13){
                        educationSector->DisplaySchoolsByRanking();
                    }
                    else
                    {
                        cout << "Invalid input" << endl;
                    }
                }
            }
            else if (choice == 7) {  // Population Sector

                //Population Sector:
                bool back = false;
                while (!back) {

                    populationMenu();
                    int choice; cin >> choice;
                    cin.ignore();

                    if (choice == 11) back = true;  
                    else if (choice == 1) {
                        string street, sector;
                        int houseNo;
                        cout << "Enter Sector: "; cin >> sector;
                        cout << "Enter Street: "; cin >> street;
                        cout << "Enter House Number: "; cin >> houseNo;

                        // gett coordinates from user or generate random
                        double lat = 0.0, lon = 0.0;
                        char coordChoice;
                        cout << "Enter coordinates manually? (y/n): "; cin >> coordChoice;
                        if (coordChoice == 'y' || coordChoice == 'Y') {
                            cout << "Enter Latitude (33.60 to 33.80): ";
                            if (!SafeInput::getDouble(lat, 33.60, 33.80)) {
                                cout << "Invalid latitude for Islamabad!" << endl;
                                continue;
                            }
                            cout << "Enter Longitude (72.90 to 73.20): ";
                            if (!SafeInput::getDouble(lon, 72.90, 73.20)) {
                                cout << "Invalid longitude for Islamabad!" << endl;
                                continue;
                            }
                            populationSector->AddHouse(street, houseNo, sector, lat, lon);
                        }
                        else {
                            populationSector->AddHouse(street, houseNo, sector);  // Will generate random
                        }
                    }
                    else if (choice == 2) {

                        string street, sector;
                        int houseNo;
                        cout << "Enter Sector: "; cin >> sector;
                        cout << "Enter Street: "; cin >> street;
                        cout << "Enter House Number: "; cin >> houseNo;

                        //Check if house exists
                        HouseNode* existingHouse = populationSector->FindHouse(street, houseNo, sector);

                        if (!existingHouse) {
                            char choice;
                            cout << "House does not exist. create it first? (y/n): ";
                            cin >> choice;

                            if (choice == 'y' || choice == 'Y')
                            {
                                populationSector->AddHouse(street, houseNo, sector);
                                existingHouse = populationSector->FindHouse(street, houseNo, sector);

                                if (existingHouse != nullptr) {
                                    existingHouse->AddFamily();
                                    cout << "house created and first family added." << endl;
                                }
                            }
                            else {
                                cout << "Operation Cancelled." << endl;
                            }
                        }
                        else {
                            existingHouse->AddFamily();
                            cout << "New Family added to existing House." << endl;
                        }
                    }
                    else if (choice == 3) {

                        string street, sector, cnic, name, occupation, gender;
                        int houseNo, age, familyChoice;

                        cout << "Enter Sector: "; cin >> sector;
                        cout << "Enter Street: "; cin >> street;
                        cout << "Enter House Number: "; cin >> houseNo;

                        HouseNode* houseNode = populationSector->FindHouse(street, houseNo, sector);

                        if (houseNode == nullptr) {
                            cout << "Error: House does not exist." << endl;
                            break;
                        }

                        int familyCount = 0;
                        FamilyNode* familyTemp = houseNode->GetChild();
                        while (familyTemp != nullptr)
                        {
                            familyCount++;
                            familyTemp = familyTemp->GetNext();
                        }

                        if (familyCount == 0)
                        {
                            cout << "No families in this house." << endl;
                            break;
                        }

                        cout << "This house has " << familyCount << " family/families." << endl;
                        cout << "Enter family number to add member to (1 to " << familyCount << "): ";
                        cin >> familyChoice;

                        if (familyChoice < 1 || familyChoice > familyCount) {
                            cout << "Invalid family number." << endl;
                            break;
                        }

                        cout << "Enter CNIC ";
                        cin >> cnic;
                        cout << "Enter Name: ";
                        cin.ignore();
                        getline(cin, name);
                        cout << "Enter Age: ";
                        cin >> age;
                        cout << "Enter Occupation: ";
                        cin.ignore();
                        getline(cin, occupation);
                        cout << "Enter Gender (male/female): ";
                        cin >> gender;

                        populationSector->AddMemberToHouse(street, houseNo, sector, cnic, name, age, occupation, gender, familyChoice);
                    }
                    else if (choice == 4) {

                        string cnic;
                        cout << "Enter CNIC to search: "; cin >> cnic;
                        populationSector->SearchCitizenByCNIC(cnic);
                    }
                    else if (choice == 5) {

                        string name;
                        cout << "Enter name to search: ";
                        cin.ignore();
                        getline(cin, name);
                        populationSector->SearchCitizenByName(name);
                    }
                    else if (choice == 6) {

                        string sector;
                        cout << "Enter sector to view houses: "; cin >> sector;
                        populationSector->DisplayHousesInSector(sector);
                    }
                    else if (choice == 7) {

                        populationSector->DisplayHousingHierarchy();
                    }
                    else if (choice == 8) {  

                        string filename;
                        cout << "Enter filename (e.g., population.csv): ";
                        getline(cin, filename);

                        if (filename.empty()) {
                            filename = "Data/population.csv";  // Default file
                        }

                        populationSector->LoadFromFile(filename);
                    }
                    else if (choice == 9) {  

                        int surveyChoice;
                        bool surveyRunning = true;

                        while (surveyRunning) {

                            populationSector->DisplaySurveyMenu();
                            cin >> surveyChoice;

                            if (surveyChoice == 7) surveyRunning = false;
                            else if (surveyChoice == 1) {
                                string ratio = populationSector->GetGenderRatioReport();
                                cout << "Gender Ratio (Male:Female:Unidentified): " << ratio << endl;
                            }
                            else if (surveyChoice == 2) {
                                string report = populationSector->GetOccupationReport();
                                cout << "=== OCCUPATION SUMMARY ===" << endl;
                                cout << report << endl;
                            }
                            else if (surveyChoice == 3) {
                                string report = populationSector->GetAgeDistributionReport();
                                cout << "=== AGE DISTRIBUTION ===" << endl;
                                cout << report << endl;
                            }
                            else if (surveyChoice == 4) {
                                string report = populationSector->GetPopulationDensityReport();
                                cout << report << endl;
                            }
                            else if (surveyChoice == 5) {
                                string report = populationSector->GetSectorPopulationReport();
                                cout << "=== SECTOR POPULATION ===" << endl;
                                cout << report << endl;
                            }
                            else if (surveyChoice == 6) {
                                populationSector->GenerateSurveyReport();
                            }
                            else cout << "Invalid choice. Try again." << endl;
                        }
                    }
                    else if (choice == 10) { 

                        int statsChoice;
                        bool statsRunning = true;

                        while (statsRunning)
                        {
                            populationSector->DisplayStatsMenu();
                            cin >> statsChoice;

                            if (statsChoice == 5) statsRunning = false;
                            else if (statsChoice == 1) {
                                cout << "Total Population: " << populationSector->GetTotalPopulation() << endl;
                            }
                            else if (statsChoice == 2) {
                                cout << "Total Houses: " << populationSector->GetTotalHouses() << endl;
                            }
                            else if (statsChoice == 3) {
                                cout << "Total Families: " << populationSector->GetTotalFamilies() << endl;
                            }
                            else if (statsChoice == 4) {
                                string sector;
                                cout << "Enter sector: "; cin >> sector;
                                cout << "Families in " << sector << ": " << populationSector->GetFamiliesInSector(sector) << endl;
                            }
                            else cout << "Invalid choice. Try again." << endl;
                        }
                    }
                    else {
                        cout << "Invalid Input" << endl;
                    }
                }
                }
            else if (choice == 8) {

                //Transport Sector:
                bool back = false;
                while (!back) {

                    transportMenu();
                    int choice; cin >> choice;
                    cin.ignore();

                    if (choice == 27) back = true;
                    else if (choice == 1) {

                        string id, name;
                        double x, y;
                        cout << "Enter stop ID: "; cin >> id;
                        cin.ignore();
                        cout << "Enter stop name: "; cin >> name;
                        cin.ignore();
                        
                        cout << "Enter x coordinate: "; cin >> x;
                                  
                        cout << "Enter y coordinate: "; cin >> y;
                        
                        transportSector->addStop(id, name, x, y);
                    }
                    else if (choice == 2) {

                        string one, two;
                        int distance;
                        cout << "Enter first stop ID: "; cin >> one;
                        cout << "Enter second stop ID: "; cin >> two;
                        cout << "Enter Distance between the two: "; cin >> distance;
                        transportSector->connectStops(one, two, distance, true);
                    }
                    else if (choice == 3) {

                        string num, comp, startingStop;
                        int count;
                        cout << "Enter Bus Number: "; cin >> num;
                        cout << "Enter Bus Company: "; cin >> comp;
                        cout << "Enter number of stops in route: "; cin >> count;
                       
                        cin.clear();
                        cin.ignore(1000, '\n');
                        if (count <= 0) {
                            cout << "Invalid Route Length" << endl;
                        }
                        else {
                            string* stops = new string[count];
                            bool allStopsValid = true;
                            cout << "Enter stop IDs in order: " << endl;
                            for (int i = 0; i < count; i++) {
                                cout << "Stop " << i + 1 << ": ";
                                cin >> stops[i];
                                if (!cityGraph->containsLocationId(stops[i])) {
									cout << "Stop ID " << stops[i] << " does not exist" << endl;
                                    continue;
                                }
                                cin.clear();
                                cin.ignore(1000, '\n');
                             
                            }
                            Route* route = transportSector->createRoute(stops, count);
                            startingStop = stops[0];
                            delete[]stops;
                            transportSector->addBus(num, comp, route, startingStop);
                        }
                    }
                    else if (choice == 4) {

                        string number, stop;
                        cout << "Enter Bus Number: "; cin >> number;
                        cout << "Enter new stop ID: "; cin >> stop;
                        transportSector->updateBusLocation(number, stop);
                    }
                    else if (choice == 5) {

                        string stop;
                        cout << "Enter stop ID: "; cin >> stop;
                        string nearest = transportSector->getNearestBus(stop);
                        if (nearest.empty()) cout << "No reachable bus for this stop (" << stop << ")" << endl;
                        else cout << "Nearest bus to stop (" << stop << ") is: " << nearest << endl;
                    }
                    else if (choice == 6) {

                        string from, to;
                        cout << "Enter source stop ID: "; cin >> from;
                        cout << "Enter destination stop ID: "; cin >> to;
                        transportSector->getShortestPath(from, to);
                    }
                    else if (choice == 7) {
                        transportSector->displayGraph();
                    }
                    else if (choice == 8) {
                        transportSector->displayBuses();
                    }
                    else if (choice == 9) {
                        transportSector->displayHashState();
                    }
                    else if (choice == 10) {

                        string stopId;
                        int cap;
                        cout << "Enter ID for queue: "; cin >> stopId;
                        cout << "Enter queue capacity: "; cin >> cap;
                        if (cap > 0) transportSector->initializeQueue(stopId, cap);
                        else transportSector->initializeQueue(stopId);
                    }
                    else if (choice == 11) {

                        string stopId, name, cnic, dest;
                        cout << "Enter Stop ID: "; cin >> stopId;
                        cout << "Enter Name: "; cin >> name;
                        cout << "Enter CNIC: "; cin >> cnic;
                        cout << "Enter Destination: "; cin >> dest;
                        transportSector->addPassenger(stopId, name, cnic, dest);
                    }
                    else if (choice == 12) {

                        string stopId;
                        int num;
                        cout << "Enter stop ID: "; cin >> stopId;
                        cout << "Enter number of passengers to board: "; cin >> num;
                        transportSector->boardPassengers(stopId, num);
                    }
                    else if (choice == 13) {

                        string stopId;
                        cout << "Enter stop ID: "; cin >> stopId;
                        transportSector->displayPassengersQueue(stopId);
                    }
                    else if (choice == 14) {
                        transportSector->displayTravelHistory();
                    }
                    else if (choice == 15) {
                        transportSector->removeLastTravel();
                    }
                    else if (choice == 16) {
                        transportSector->clearTravelHistory();
                    }
                    else if (choice == 17) {

                        string busNum, from, to;
                        cout << "Enter Bus Number: "; cin >> busNum;
                        cout << "Enter Source Stop ID: "; cin >> from;
                        cout << "Enter Destination Stop ID: "; cin >> to;
                        transportSector->journeySimulation(busNum, from, to);
                    }
                    else if (choice == 18) {
                        string busNum;
                        cout << "Enter Bus Number for real-time simulation: ";
						cin >> busNum;
                        transportSector->realTimeRouteSimulation(busNum);
                    }
                    else if (choice == 19) {
						string currentStop;
                        cout << endl;
                        cout << "Emergency Route Finder" << endl;
						cout << "Enter your current stop ID: ";
						cin >> currentStop;
                        cin.ignore();
                        transportSector->emergencyRouteToHospital(currentStop,medicalSector);
                    }
                    else if (choice == 20) {
                        transportSector->displaySchoolBuses();
                    }
                    else if (choice == 21) {
                        string schoolID;
                        cout << "Enter School ID: ";
						cin >> schoolID;
                        Bus* schoolBus = transportSector->findSchoolBusBySchool(schoolID);
                        if (schoolBus) {
                            schoolBus->displaySchoolBusInfo();
                        }
                        else {
                            cout << "No school bus found for school " << schoolID << endl;
                        }
                    }
                    else if (choice == 22) {
                        string sector;
                        cout << "Enter Sector (e.g., G-10, F-8): ";
                        cin >> sector;
                        transportSector->findSchoolBusesBySector(sector);
                    }
                    else if (choice == 23) {
                        string busNumber;
                        cout << "Enter School Bus Number to track: ";
					
                        cin >> busNumber;
                        transportSector->trackSchoolBusJourney(busNumber);
                        }
                    else if (choice == 24) 
                    {
                            transportSector->diplayAllStops();
                    }
                    else if (choice == 25) 
                    {
                                string busNumber, newStopID, newStopName;
                                double lat, lon;

                                cout << "Enter Bus Number: "; cin >> busNumber;
                                cout << "Enter new Stop ID: "; cin >> newStopID;
                                cin.ignore();
                                cout << "Enter new Stop Name: ";
                                getline(cin, newStopName);

                                cout << "Enter Latitude: ";
                                if (!SafeInput::getDouble(lat, -90, 90)) {
                                    cout << "Invalid latitude" << endl;
                                    continue;
                                }
                                cout << "Enter Longitude: ";
                                if (!SafeInput::getDouble(lon, -180, 180)) {
                                    cout << "Invalid longitude" << endl;
                                    continue;
                                }

                                transportSector->addNewStopAtEnd(busNumber, newStopID, newStopName, lat, lon);
                     }
                    else if (choice == 26)
                    {
                        string busNumber, stopID;

                        cout << "\n=== REMOVE STOP FROM BUS ROUTE ===" << endl;
                        cout << "Enter Bus Number: ";
                        cin >> busNumber;
                        cout << "Enter Stop ID to remove: ";
                        cin >> stopID;

                        Bus* bus = transportSector->getBusDetails(busNumber);
                        if (!bus) {
                            cout << "Bus not found!" << endl;
                            continue;
                        }

                        cout << "Current route: ";
                        if (bus->getCurrentRoute()) {
                            bus->getCurrentRoute()->display();
                        }
                        else {
                            cout << "No route assigned" << endl;
                            continue;
                        }

                       
                        }
                    else if (choice == 27)
                    {
                        back = true;
                        }
                }
            }
        }
    }
};