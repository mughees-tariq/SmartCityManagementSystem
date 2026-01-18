#pragma once
#include<iostream>
#include<string>
using namespace std;

class CitizenHashTable;

class IndividualNode {
    string CNIC;
    string Name;
    int Age;
    string Occupation;
    string Gender;
    IndividualNode* next;

public:
    IndividualNode(string cnic = "", string name = "", int age = 0, string occup = "", string gender = "")
    {
        CNIC = cnic;
        Name = name;
        Age = age;
        Occupation = occup;

        GenderInputHandling(gender);    //if gender isnot male or female then it is set as unidentified 

        next = nullptr;
    }

    string GetCNIC()
    {
        return CNIC;
    }
    string GetName()
    {
        return Name;
    }
    int GetAge()
    {
        return Age;
    }
    string GetOccupation()
    {
        return Occupation;
    }
    string GetGender()
    {
        return Gender;
    }
    IndividualNode* GetNext()
    {
        return next;
    }
    void SetNext(IndividualNode* ptr)
    {
        next = ptr;
    }

    void SetCNIC(string cnic)
    {
        CNIC = cnic;
    }
    void SetName(string name)
    {
        Name = name;
    }
    void SetAge(int age)
    {
        Age = age;
    }
    void SetOccupation(string occup)
    {
        Occupation = occup;
    }
    void SetGender(string gender)
    {
        GenderInputHandling(gender);
    }


    void GenderInputHandling(string gender)
    {
        //input handling for gender; if gender is not male or female then it is unidentified .

         //so for this ive converted the entered gender into lowercase first , so that it can be easily read for male or female.

        for (int i = 0; i < gender.length(); i++)
        {
            if (gender[i] >= 'A' && gender[i] <= 'Z')
            {
                gender[i] += 32;
            }
        }

        //now input handle for gender 
        if (gender != "male" && gender != "female")
            Gender = "unidentified";

        else
            Gender = gender;
    }
};

class FamilyNode {
    int NoOfMembers;
    FamilyNode* next;
    IndividualNode* child;

public:
    FamilyNode(int members = 0)
    {
        NoOfMembers = members;
        next = nullptr;
        child = nullptr;
    }

    void AddMember(string cnic = "", string name = "", int age = 0, string occup = "", string gender = "")
    {
        IndividualNode* newMember = new IndividualNode(cnic, name, age, occup, gender);

        if (child == nullptr)
        {
            child = newMember;
        }
        else
        {
            IndividualNode* temp = child;
            while (temp->GetNext() != nullptr)
            {
                temp = temp->GetNext();
            }
            temp->SetNext(newMember);
        }
        NoOfMembers++;
    }

    IndividualNode* GetChild()
    {
        return child;
    }
    FamilyNode* GetNext()
    {
        return next;
    }
    void SetNext(FamilyNode* ptr)
    {
        next = ptr;
    }
    int GetMemberCount()
    {
        return NoOfMembers;
    }

};

class HouseNode {
    string Street;
    int HouseNo;
    string Sector;
    double latitude;
    double longitude;
    HouseNode* next;
    FamilyNode* child;
    string houseID;

public:
    HouseNode(string street = "", int house = 0, string sector = "", double lat = 0, double longit = 0, string ID = "")
    {
        Street = street;
        HouseNo = house;
        Sector = sector;
        latitude = lat;
        longitude = longit;
        houseID = ID;
        next = nullptr;
        child = nullptr;
    }

    void AddFamily(int members = 0)
    {
        FamilyNode* newFamily = new FamilyNode(members);

        if (child == nullptr)
        {
            child = newFamily;
        }
        else
        {
            FamilyNode* temp = child;
            while (temp->GetNext() != nullptr)
            {
                temp = temp->GetNext();
            }
            temp->SetNext(newFamily);
        }
    }

    void AddMemberToFamily(string cnic = "", string name = "", int age = 0, string occup = "", string gender = "")
    {
        if (child == nullptr)
        {
            AddFamily(0);
        }

        FamilyNode* temp = child;
        while (temp->GetNext() != nullptr)
        {
            temp = temp->GetNext();
        }
        temp->AddMember(cnic, name, age, occup, gender);
    }

    HouseNode* GetNext()
    {
        return next;
    }
    void SetNext(HouseNode* ptr)
    {
        next = ptr;
    }
    double GetLatitude() 
    { 
        return latitude; 
    }
    double GetLongitude() 
    { 
        return longitude; 
    }
    string GetHouseID() 
    { 
        return houseID; 
    }
    void SetCoordinates(double lat, double lon) 
    {
        latitude = lat;
        longitude = lon;
    }
    void SetHouseID(string id) 
    { 
        houseID = id; 
    }
    FamilyNode* GetChild()
    {
        return child;
    }
    string GetStreet()
    {
        return Street;
    }
    int GetHouseNo()
    {
        return HouseNo;
    }
    string GetSector()
    {
        return Sector;
    }
};

class CitizenHashTable {


    struct CitizenTableNode
    {
        string CNIC;
        string Name;
        int Age;
        string Occupation;
        string Gender;
        string Sector;
        string Street;
        int HouseNo;
        bool isOccupied;
        int index;
        CitizenTableNode* right;

        CitizenTableNode(string cnic = "", string name = "", int age = 0, string occup = "", string gender = "", string sector = "", string street = "", int house = 0, int idx = 0)
        {
            CNIC = cnic;
            Name = name;
            Age = age;
            Occupation = occup;
            Gender = gender;
            Sector = sector;
            Street = street;
            HouseNo = house;
            index = idx;
            isOccupied = false;
            right = nullptr;
        }

        string GetCNIC()
        {
            return CNIC;
        }
        string GetName()
        {
            return Name;
        }
        int GetAge()
        {
            return Age;
        }
        string GetOccupation()
        {
            return Occupation;
        }
        string GetGender()
        {
            return Gender;
        }
        string GetSector()
        {
            return Sector;
        }
        string GetStreet()
        {
            return Street;
        }
        int GetHouseNo()
        {
            return HouseNo;
        }
        bool IfOccupied()
        {
            return isOccupied;
        }
        int GetIndex()
        {
            return index;
        }
        CitizenTableNode* GetRight()
        {
            return right;
        }

        void SetOccupied(bool val)
        {
            isOccupied = val;
        }
        void SetRight(CitizenTableNode* ptr)
        {
            right = ptr;
        }
    };

    CitizenTableNode* CitizenTable;
    int TableSize;

public:
    CitizenHashTable(int size = 10) //keeping default size 10. resize not required because if collisions occur then it is saved as linked list .
    {
        if (size <= 0)
            size = 10;
        TableSize = size;
        CitizenTable = new CitizenTableNode[TableSize];
    }

    int HashFunction(string key)
    {
        long long hash = 0;
        long long p = 31;
        long long pow = 1;
        int mod = TableSize;
        if (mod <= 1) mod = 1;

        for (int i = 0; key[i] != '\0'; i++)
        {
            hash = (hash + ((long long)(unsigned char)key[i] * pow)) % mod;
            pow = (pow * p) % mod;
        }
        return (int)hash;
    }

    void AddCitizen(string cnic = "", string name = "", int age = 0, string occup = "", string gender = "", string sector = "", string street = "", int house = 0)
    {
        if (cnic == "")
            return;

        int Index = HashFunction(cnic);

        if (CitizenTable[Index].IfOccupied() == false)
        {
            CitizenTable[Index].CNIC = cnic;
            CitizenTable[Index].Name = name;
            CitizenTable[Index].Age = age;
            CitizenTable[Index].Occupation = occup;
            CitizenTable[Index].Gender = gender;
            CitizenTable[Index].Sector = sector;
            CitizenTable[Index].Street = street;
            CitizenTable[Index].HouseNo = house;
            CitizenTable[Index].index = Index;
            CitizenTable[Index].SetOccupied(true);
            CitizenTable[Index].SetRight(nullptr);
        }
        else
        {
            CitizenTableNode* temp = &CitizenTable[Index];
            while (temp != nullptr)
            {
                if (temp->GetCNIC() == cnic)
                    return;
                if (temp->GetRight() == nullptr)
                    break;
                temp = temp->GetRight();
            }
            temp->SetRight(new CitizenTableNode(cnic, name, age, occup, gender, sector, street, house, Index));
        }
    }

    bool SearchByCNIC(string cnic, string& name, int& age, string& occup, string& gender, string& sector, string& street, int& house)
    {
        if (cnic == "")
            return false;

        int idx = HashFunction(cnic);
        CitizenTableNode* head = &CitizenTable[idx];

        while (head != nullptr)
        {
            if (head->GetCNIC() == cnic && head->IfOccupied())
            {
                name = head->GetName();
                age = head->GetAge();
                occup = head->GetOccupation();
                gender = head->GetGender();
                sector = head->GetSector();
                street = head->GetStreet();
                house = head->GetHouseNo();
                return true;
            }
            head = head->GetRight();
        }
        return false;
    }

    CitizenTableNode** SearchByName(string name, int& count)    //what if multiple people with the same name? so returning the stored names
    {
        if (name == "")
        {
            count = 0;
            return nullptr;
        }

        count = 0;
        for (int i = 0; i < TableSize; i++)
        {
            CitizenTableNode* current = &CitizenTable[i];
            while (current != nullptr)
            {
                if (current->IfOccupied() && current->GetName() == name)
                    count++;
                current = current->GetRight();
            }
        }

        if (count == 0)
            return nullptr;

        CitizenTableNode** results = new CitizenTableNode * [count];
        int idx = 0;

        for (int i = 0; i < TableSize; i++)
        {
            CitizenTableNode* current = &CitizenTable[i];
            while (current != nullptr)
            {
                if (current->IfOccupied() && current->GetName() == name)
                {
                    results[idx] = current;
                    idx++;
                }
                current = current->GetRight();
            }
        }

        return results;
    }

    string GenerateGenderRatio()
    {
        int male = 0, female = 0, unidentified = 0;

        for (int i = 0; i < TableSize; i++)
        {
            CitizenTableNode* current = &CitizenTable[i];
            while (current != nullptr)
            {
                if (current->IfOccupied())
                {
                    string gender = current->GetGender();
                    if (gender == "male")
                        male++;
                    else if (gender == "female")
                        female++;
                    else unidentified++;
                }
                current = current->GetRight();
            }
        }

        return to_string(male) + ":" + to_string(female) + ":" + to_string(unidentified);
    }

    ~CitizenHashTable()
    {
        if (CitizenTable)
        {
            for (int i = 0; i < TableSize; i++)
            {
                CitizenTableNode* head = CitizenTable[i].GetRight();
                while (head != nullptr)
                {
                    CitizenTableNode* nxt = head->GetRight();
                    delete head;
                    head = nxt;
                }
            }
            delete[] CitizenTable;
            CitizenTable = nullptr;
        }
    }

    string GenerateSectorPopulationSummary()
    {
        string* sectors = nullptr;
        int* counts = nullptr;
        int sectorCount = 0;
        int capacity = 0;

        for (int i = 0; i < TableSize; i++)
        {
            CitizenTableNode* current = &CitizenTable[i];
            while (current != nullptr)
            {
                if (current->IfOccupied())
                {
                    string sector = current->GetSector();
                    bool found = false;

                    for (int j = 0; j < sectorCount; j++)
                    {
                        if (sectors[j] == sector)
                        {
                            counts[j]++;
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        if (sectorCount == capacity)
                        {
                            int newCapacity;
                            if (capacity == 0)
                            {
                                newCapacity = 1;
                            }
                            else
                            {
                                newCapacity = capacity * 2;
                            }

                            string* newSectors = new string[newCapacity];
                            int* newCounts = new int[newCapacity];

                            for (int k = 0; k < sectorCount; k++)
                            {
                                newSectors[k] = sectors[k];
                                newCounts[k] = counts[k];
                            }

                            delete[] sectors;
                            delete[] counts;
                            sectors = newSectors;
                            counts = newCounts;
                            capacity = newCapacity;
                        }

                        sectors[sectorCount] = sector;
                        counts[sectorCount] = 1;
                        sectorCount++;
                    }
                }
                current = current->GetRight();
            }
        }

        string result = "";
        for (int i = 0; i < sectorCount; i++)
        {
            result += sectors[i] + ": " + to_string(counts[i]) + " people" + "\n";
        }

        delete[] sectors;
        delete[] counts;

        return result;
    }

    string GenerateOccupationSummary()
    {
        struct OccEntry {
            string occupation;
            int count;
            OccEntry* next;
        };

        OccEntry* occHead = nullptr;
        int uniqueOccCount = 0;
        int totalPeople = 0;

        for (int i = 0; i < TableSize; i++)
        {
            CitizenTableNode* current = &CitizenTable[i];
            while (current != nullptr)
            {
                if (current->IfOccupied())
                {
                    totalPeople++;
                    string occ = current->GetOccupation();

                    if (occ == "")  // Handle empty occupation
                        occ = "Unemployed/Not Specified";

                    OccEntry* temp = occHead;
                    OccEntry* prev = nullptr;

                    bool found = false;
                    while (temp != nullptr)
                    {
                        if (temp->occupation == occ)
                        {
                            temp->count++;
                            found = true;
                            break;
                        }
                        prev = temp;
                        temp = temp->next;
                    }

                    if (!found)
                    {
                        OccEntry* newEntry = new OccEntry();
                        newEntry->occupation = occ;
                        newEntry->count = 1;
                        newEntry->next = nullptr;

                        if (occHead == nullptr)
                            occHead = newEntry;
                        else
                            prev->next = newEntry;

                        uniqueOccCount++;
                    }
                }
                current = current->GetRight();
            }
        }

        string result = "Total People: " + to_string(totalPeople) + "\n";
        result += "Unique Occupations: " + to_string(uniqueOccCount) + "\n";

        OccEntry* current = occHead;
        while (current != nullptr)
        {
            double percentage = (current->count * 100.0) / totalPeople;
            result += current->occupation + ": " + to_string(current->count) + " (" + to_string(percentage).substr(0, 4) + "%)\n";
            OccEntry* next = current->next;
            delete current;
            current = next;
        }

        return result;
    }

    string GenerateAgeDistribution()
    {
        string* sectors = nullptr;
        int* totalAges = nullptr;
        int* personCounts = nullptr;
        int sectorCount = 0;
        int capacity = 0;

        for (int i = 0; i < TableSize; i++)
        {
            CitizenTableNode* current = &CitizenTable[i];
            while (current != nullptr)
            {
                if (current->IfOccupied())
                {
                    string sector = current->GetSector();
                    int age = current->GetAge();
                    bool found = false;

                    for (int j = 0; j < sectorCount; j++)
                    {
                        if (sectors[j] == sector)
                        {
                            totalAges[j] += age;
                            personCounts[j]++;
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        if (sectorCount == capacity)
                        {
                            int newCapacity;
                            if (capacity == 0)
                            {
                                newCapacity = 1;
                            }
                            else
                            {
                                newCapacity = capacity * 2;
                            }

                            string* newSectors = new string[newCapacity];
                            int* newTotalAges = new int[newCapacity];
                            int* newPersonCounts = new int[newCapacity];

                            for (int k = 0; k < sectorCount; k++)
                            {
                                newSectors[k] = sectors[k];
                                newTotalAges[k] = totalAges[k];
                                newPersonCounts[k] = personCounts[k];
                            }

                            delete[] sectors;
                            delete[] totalAges;
                            delete[] personCounts;
                            sectors = newSectors;
                            totalAges = newTotalAges;
                            personCounts = newPersonCounts;
                            capacity = newCapacity;
                        }

                        sectors[sectorCount] = sector;
                        totalAges[sectorCount] = age;
                        personCounts[sectorCount] = 1;
                        sectorCount++;
                    }
                }
                current = current->GetRight();
            }
        }

        string result = "";
        for (int i = 0; i < sectorCount; i++)
        {
            double averageAge = (double)totalAges[i] / personCounts[i];
            result += sectors[i] + ": " + to_string(averageAge) + " years average\n";
        }

        delete[] sectors;
        delete[] totalAges;
        delete[] personCounts;

        return result;
    }
    string CalculatePopulationDensity()
    {
        string* sectors = nullptr;
        int* personCounts = nullptr;
        int sectorCount = 0;
        int capacity = 0;

        for (int i = 0; i < TableSize; i++)
        {
            CitizenTableNode* current = &CitizenTable[i];
            while (current != nullptr)
            {
                if (current->IfOccupied())
                {
                    string sector = current->GetSector();
                    bool found = false;

                    for (int j = 0; j < sectorCount; j++)
                    {
                        if (sectors[j] == sector)
                        {
                            personCounts[j]++;
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        if (sectorCount == capacity)
                        {
                            int newCapacity;
                            if (capacity == 0)
                            {
                                newCapacity = 1;
                            }
                            else
                            {
                                newCapacity = capacity * 2;
                            }

                            string* newSectors = new string[newCapacity];
                            int* newPersonCounts = new int[newCapacity];

                            for (int k = 0; k < sectorCount; k++)
                            {
                                newSectors[k] = sectors[k];
                                newPersonCounts[k] = personCounts[k];
                            }

                            delete[] sectors;
                            delete[] personCounts;
                            sectors = newSectors;
                            personCounts = newPersonCounts;
                            capacity = newCapacity;
                        }

                        sectors[sectorCount] = sector;
                        personCounts[sectorCount] = 1;
                        sectorCount++;
                    }
                }
                current = current->GetRight();
            }
        }

        // Calculate total population
        int totalPopulation = 0;
        for (int i = 0; i < sectorCount; i++)
        {
            totalPopulation += personCounts[i];
        }

        string result = "";
        result += "=== POPULATION DENSITY HEATMAP ===\n";
        result += "Color Guide: \033[32mLow\033[0m | \033[33mMedium\033[0m | \033[31mHigh\033[0m\n";
        result += "Total Population: " + to_string(totalPopulation) + "\n";
        result += "==================================\n";

        for (int i = 0; i < sectorCount; i++)
        {
            // Calculate percentage of total population
            double percentOfTotal = (personCounts[i] * 100.0) / totalPopulation;

            // Bar length based on percentage (0-10 scale)
            int barLength = (int)((percentOfTotal / 100.0) * 10.0);
            if (barLength < 1)
                barLength = 1;

            string colorCode;
            if (percentOfTotal <= 33.0)         // Less than 15% of total
                colorCode = "\033[32m";        // Green (Low density)
            else if (percentOfTotal <= 66.0)    // 15-30% of total
                colorCode = "\033[33m";        // Yellow (Medium density)
            else                                // More than 30% of total
                colorCode = "\033[31m";        // Red (High density)

            string bar = "";
            for (int j = 0; j < barLength; j++)
                bar += "#";

            result += sectors[i] + ": " + colorCode + bar + "\033[0m ("
                + to_string(personCounts[i]) + " people, "
                + to_string((int)percentOfTotal) + "%)\n";
        }

        delete[] sectors;
        delete[] personCounts;

        return result;
    }

};

class HousingSystem {
    HouseNode* head;    //only 1 housing system for all of islamabad so it has no sibling pointer . it handles all houses 
    CitizenHashTable* citizenTable;
    Graph* cityGraph;

    double min_lat = 33.60;
    double max_lat = 33.80;
    double min_lon = 72.90;
    double max_lon = 73.20;

public:

    bool ValidateCoordinates(double lat, double lon) 
    {
        if (lat < min_lat || lat > max_lat) {
            cout << "Error: Latitude must be between " << min_lat << " and " << max_lat << endl;
            return false;
        }
        if (lon < min_lon || lon > max_lon) {
            cout << "Error: Longitude must be between " << min_lon << " and " << max_lon << endl;
            return false;
        }
        return true;
    }

    HousingSystem(Graph* graph, const string& filename = "Data/population.csv")
    {
        cityGraph = graph;
        head = nullptr;
        citizenTable = new CitizenHashTable();

    }

    void GenerateRandomCoordinates(double& lat, double& lon) 
    {
        lat = min_lat + (double(rand()) / RAND_MAX) * (max_lat - min_lat);
        lon = min_lon + (double(rand()) / RAND_MAX) * (max_lon - min_lon);
    }

    void AddHouse(string street = "", int house = 0, string sector = "",
        double lat = 0.0, double lon = 0.0, string customID = "")
    {
        if (!cityGraph) {
            cout << "Error: City graph is not initialized yet" << endl;
            return;
        }

        HouseNode* existing = FindHouse(street, house, sector);
        if (existing != nullptr)
        {
            cout << "House already exists at " << sector << ", " << street << " " << house << endl;
            return;
        }

        if (lat == 0.0 && lon == 0.0) {
            GenerateRandomCoordinates(lat, lon);
            cout << "Generated coordinates: (" << lat << ", " << lon << ")" << endl;
        }
        else {
            if (!ValidateCoordinates(lat, lon)) {
                cout << "Error: Coordinates are outside Islamabad boundaries." << endl;
                cout << "Would you like to generate random coordinates instead? (y/n): ";
                char choice;
                cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    GenerateRandomCoordinates(lat, lon);
                    cout << "Generated coordinates: (" << lat << ", " << lon << ")" << endl;
                }
                else {
                    cout << "House not added." << endl;
                    return;
                }
            }
        }

        string houseID;
        if (customID != "") {
            houseID = customID;
        }
        else {
            houseID = "HOU_" + sector + "_" + street + "_" + to_string(house);
        }

        HouseNode* newHouse = new HouseNode(street, house, sector, lat, lon, houseID);

        if (head == nullptr)
        {
            head = newHouse;
        }
        else
        {
            HouseNode* temp = head;
            while (temp->GetNext() != nullptr)
            {
                temp = temp->GetNext();
            }
            temp->SetNext(newHouse);
        }

        if (!cityGraph->containsLocationId(houseID)) {
            string nodeName = "House " + to_string(house) + " - " + street + ", " + sector;
            if (cityGraph->addLocation(houseID, nodeName, lat, lon)) {
                cout << "House added to city graph with ID: " << houseID << endl;
            }
            else {
                cout << "Warning: House could not be added to city graph." << endl;
            }
        }

        cout << "House added successfully." << endl;
    }

    void AddAllHousesToGraph() {
        if (!cityGraph) return;

        HouseNode* current = head;
        int addedCount = 0;
        int skippedCount = 0;

        while (current != nullptr) 
        {
            string houseID = current->GetHouseID();
            if (houseID == "") 
            {
                houseID = "HOU_" + current->GetSector() + "_" +
                    current->GetStreet() + "_" + to_string(current->GetHouseNo());
                current->SetHouseID(houseID);
            }

            if (ValidateCoordinates(current->GetLatitude(), current->GetLongitude())) 
            {
                if (!cityGraph->containsLocationId(houseID)) 
                {
                    string nodeName = "House " + to_string(current->GetHouseNo()) + " - " + current->GetStreet() + ", " + current->GetSector();
                    cityGraph->addLocation(houseID, nodeName,
                        current->GetLatitude(), current->GetLongitude());
                    addedCount++;
                }
            }
            else 
            {
                double lat, lon;
                GenerateRandomCoordinates(lat, lon);
                current->SetCoordinates(lat, lon);

                if (!cityGraph->containsLocationId(houseID)) 
                {
                    string nodeName = "House " + to_string(current->GetHouseNo()) + " - " + current->GetStreet() + ", " + current->GetSector();
                    cityGraph->addLocation(houseID, nodeName, lat, lon);
                    addedCount++;
                    cout << "Fixed invalid coordinates for house ID: " << houseID << endl;
                }
                skippedCount++;
            }

            current = current->GetNext();
        }

        cout << "Added " << addedCount << " valid house nodes to city graph." << endl;
        if (skippedCount > 0) {
            cout << "Fixed " << skippedCount << " houses with invalid coordinates." << endl;
        }
    }

    string FindNearestHouse(double lat, double lon) 
    {
        if (!cityGraph || !head) 
            return "";

        double minDistance = 999999.0;
        string nearestID = "";

        HouseNode* current = head;
        while (current != nullptr) 
        {
            double distance = sqrt(pow(current->GetLatitude() - lat, 2) +
                pow(current->GetLongitude() - lon, 2));
            if (distance < minDistance) {
                minDistance = distance;
                nearestID = current->GetHouseID();
            }
            current = current->GetNext();
        }

        return nearestID;
    }

    void AddMemberToHouse(string street = "", int house = 0, string sector = "", string cnic = "", string name = "", int age = 0, string occup = "", string gender = "", int familyNumber = -1)
    {
        HouseNode* temp = head;
        while (temp != nullptr)
        {
            if (temp->GetStreet() == street && temp->GetHouseNo() == house && temp->GetSector() == sector)
            {
                if (temp->GetChild() == nullptr)
                {
                    // No families exist
                    cout << "Error: No families in this house. Add family first." << endl;
                    return;
                }

                FamilyNode* targetFamily = temp->GetChild();

                // If familyNumber specified, find that specific family
                if (familyNumber > 0)
                {
                    int currentFamily = 1;
                    while (targetFamily != nullptr && currentFamily < familyNumber)
                    {
                        targetFamily = targetFamily->GetNext();
                        currentFamily++;
                    }

                    if (targetFamily == nullptr)
                    {
                        cout << "Error: Family " << familyNumber << " not found." << endl;
                        return; 
                    }
                }
                else
                {
                    while (targetFamily->GetNext() != nullptr)
                    {
                        targetFamily = targetFamily->GetNext();
                    }
                }

                // Add member to the selected family
                targetFamily->AddMember(cnic, name, age, occup, gender);
                citizenTable->AddCitizen(cnic, name, age, occup, gender, sector, street, house);
                return;
            }
            temp = temp->GetNext();
        }

        cout << "Error: House not found in " << sector << ", " << street << " " << house << endl;
    }

    bool ValidateCNIC(string cnic)
    {
        if (cnic.length() != 15)
            return false;

        if (cnic[5] != '-' || cnic[13] != '-')
            return false;

        for (int i = 0; i < cnic.length(); i++)
        {
            if (i == 5 || i == 13)
                continue;

            if (cnic[i] < '0' || cnic[i] > '9')
                return false;
        }

        return true;
    }

    HouseNode* FindHouse(string street = "", int house = 0, string sector = "")
    {
        HouseNode* temp = head;
        while (temp != nullptr)
        {
            if (temp->GetStreet() == street && temp->GetHouseNo() == house && temp->GetSector() == sector)
            {
                return temp;
            }
            temp = temp->GetNext();
        }
        return nullptr;
    }

    void SearchCitizenByName(string name)
    {
        if (name == "")
        {
            cout << "Error: Empty name provided" << endl;
            return;
        }

        bool found = false;
        HouseNode* currentHouse = head;

        while (currentHouse != nullptr)
        {
            FamilyNode* currentFamily = currentHouse->GetChild();

            while (currentFamily != nullptr)
            {
                IndividualNode* currentPerson = currentFamily->GetChild();

                while (currentPerson != nullptr)
                {
                    if (currentPerson->GetName() == name)
                    {
                        if (!found)
                        {
                            cout << "Citizens found with name '" << name << "':" << endl;
                            cout << "====================================" << endl;
                            found = true;
                        }

                        cout << "Name: " << currentPerson->GetName() << endl;
                        cout << "CNIC: " << currentPerson->GetCNIC() << endl;
                        cout << "Age: " << currentPerson->GetAge() << endl;
                        cout << "Occupation: " << currentPerson->GetOccupation() << endl;
                        cout << "Gender: " << currentPerson->GetGender() << endl;
                        cout << "Address: " << currentHouse->GetSector() << ", "
                            << currentHouse->GetStreet() << " "
                            << currentHouse->GetHouseNo() << endl;
                        cout << "------------------------------------" << endl;
                    }
                    currentPerson = currentPerson->GetNext();
                }
                currentFamily = currentFamily->GetNext();
            }
            currentHouse = currentHouse->GetNext();
        }

        if (!found)
        {
            cout << "No citizen found with name: " << name << endl;
        }
    }

    void DisplayHousesInSector(string sector)
    {
        if (sector == "")
        {
            cout << "Error: Empty sector provided" << endl;
            return;
        }

        int houseCount = 0;
        int residentCount = 0;

        cout << "=== HOUSES IN SECTOR: " << sector << " ===" << endl;

        HouseNode* current = head;
        while (current != nullptr)
        {
            if (current->GetSector() == sector)
            {
                houseCount++;
                int peopleInHouse = 0;

                // Count people in this house
                FamilyNode* family = current->GetChild();
                while (family != nullptr)
                {
                    peopleInHouse += family->GetMemberCount();
                    family = family->GetNext();
                }

                residentCount += peopleInHouse;

                cout << houseCount << ". House No: " << current->GetHouseNo()
                    << ", Street: " << current->GetStreet()
                    << ", Residents: " << peopleInHouse << endl;
            }
            current = current->GetNext();
        }

        if (houseCount == 0)
        {
            cout << "No houses found in sector: " << sector << endl;
        }
        else
        {
            cout << "SUMMARY:" << endl;
            cout << "Total Houses: " << houseCount << endl;
            cout << "Total Residents: " << residentCount << endl;
            cout << "Average per house: " << (residentCount * 1.0 / houseCount) << " people" << endl;
        }
    }

    void DisplayHousingHierarchy()
    {
        if (head == nullptr)
        {
            cout << "No housing data available." << endl;
            return;
        }

        cout << "=== HOUSING HIERARCHY ===" << endl;
        cout << "Islamabad Housing System" << endl;
        cout << "========================" << endl;

        int totalHouses = 0;
        int totalFamilies = 0;
        int totalIndividuals = 0;

        HouseNode* currentHouse = head;
        while (currentHouse != nullptr)
        {
            totalHouses++;

            cout << "HOUSE: " << currentHouse->GetHouseNo()
                << ", Sector: " << currentHouse->GetSector()
                << ", Street: " << currentHouse->GetStreet() << endl;

            FamilyNode* currentFamily = currentHouse->GetChild();
            int familyNum = 0;

            while (currentFamily != nullptr)
            {
                totalFamilies++;
                familyNum++;

                cout << "  Family " << familyNum
                    << " (Members: " << currentFamily->GetMemberCount() << ")" << endl;

                IndividualNode* currentPerson = currentFamily->GetChild();
                int personNum = 0;

                while (currentPerson != nullptr)
                {
                    totalIndividuals++;
                    personNum++;

                    cout << "    - Person " << personNum << ": "
                        << currentPerson->GetName()
                        << ", CNIC: " << currentPerson->GetCNIC()
                        << ", Age: " << currentPerson->GetAge()
                        << ", " << currentPerson->GetOccupation() << endl;

                    currentPerson = currentPerson->GetNext();
                }

                currentFamily = currentFamily->GetNext();
            }

            cout << endl;
            currentHouse = currentHouse->GetNext();
        }

        cout << "========================" << endl;
        cout << "SUMMARY:" << endl;
        cout << "Total Houses: " << totalHouses << endl;
        cout << "Total Families: " << totalFamilies << endl;
        cout << "Total Individuals: " << totalIndividuals << endl;
        cout << "========================" << endl;
    }

    void LoadFromFile(const string& filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Cannot open file: " << filename << endl;
            return;
        }

        char line[1000];
        file.getline(line, 1000);

        int recordsProcessed = 0;
        int recordsFailed = 0;
        int housesCreated = 0;
        int familiesCreated = 0;

        while (file.getline(line, 1000))
        {
            if (line[0] == '\0') continue;

            string cnic = "";
            string name = "";
            string ageStr = "";
            string sector = "";
            string street = "";
            string houseStr = "";
            string occupation = "";

            int fieldCount = 0;
            bool inQuotes = false;
            string currentField = "";

            int i = 0;
            while (line[i] != '\0')
            {
                char c = line[i];
                if (c == '"')
                {
                    inQuotes = !inQuotes;
                    i++;
                    continue;
                }
                if (c == ',' && !inQuotes)
                {
                    switch (fieldCount)
                    {
                    case 0: cnic = currentField; break;
                    case 1: name = currentField; break;
                    case 2: ageStr = currentField; break;
                    case 3: sector = currentField; break;
                    case 4: street = currentField; break;
                    case 5: houseStr = currentField; break;
                    case 6: occupation = currentField; break;
                    }
                    currentField = "";
                    fieldCount++;
                }
                else
                {
                    currentField += c;
                }
                i++;
            }

            if (fieldCount == 6)
            {
                occupation = currentField;
            }

            if (cnic == "" || name == "" || ageStr == "" || sector == "" || street == "" || houseStr == "")
            {
                recordsFailed++;
                continue;
            }

            int age = 0;
            for (int j = 0; j < ageStr.length(); j++)
            {
                if (ageStr[j] >= '0' && ageStr[j] <= '9')
                {
                    age = age * 10 + (ageStr[j] - '0');
                }
            }

            int houseNo = 0;
            for (int j = 0; j < houseStr.length(); j++)
            {
                if (houseStr[j] >= '0' && houseStr[j] <= '9')
                {
                    houseNo = houseNo * 10 + (houseStr[j] - '0');
                }
            }

            if (!ValidateCNIC(cnic))
            {
                cout << "Invalid CNIC format for " << name << ": " << cnic << endl;
                recordsFailed++;
                continue;
            }

            HouseNode* existingHouse = FindHouse(street, houseNo, sector);
            if (existingHouse == nullptr)
            {
                double lat, lon;
                GenerateRandomCoordinates(lat, lon);

                AddHouse(street, houseNo, sector, lat, lon);
                existingHouse = FindHouse(street, houseNo, sector);

                if (existingHouse != nullptr)
                {
                    existingHouse->AddFamily();
                    housesCreated++;
                    familiesCreated++;
                }
            }

            if (existingHouse != nullptr)
            {
                string gender = "unidentified";
                if (existingHouse->GetChild() != nullptr)
                {
                    existingHouse->GetChild()->AddMember(cnic, name, age, occupation, gender);
                    citizenTable->AddCitizen(cnic, name, age, occupation, gender, sector, street, houseNo);
                    recordsProcessed++;
                }
            }
            else
            {
                recordsFailed++;
            }
        }

        file.close();

        cout << "=== DATA LOADING SUMMARY ===" << endl;
        cout << "Records processed: " << recordsProcessed << endl;
        cout << "Records failed: " << recordsFailed << endl;
        cout << "Houses created: " << housesCreated << endl;
        cout << "Families created: " << familiesCreated << endl;
        cout << "============================" << endl;
    }

    int GetTotalHouses()
    {
        int count = 0;
        HouseNode* current = head;
        while (current != nullptr)
        {
            count++;
            current = current->GetNext();
        }
        return count;
    }

    int GetTotalFamilies()
    {
        int count = 0;
        HouseNode* house = head;
        while (house != nullptr)
        {
            FamilyNode* family = house->GetChild();
            while (family != nullptr)
            {
                count++;
                family = family->GetNext();
            }
            house = house->GetNext();
        }
        return count;
    }

    int GetFamiliesInSector(string sector)
    {
        int count = 0;
        HouseNode* house = head;
        while (house != nullptr)
        {
            if (house->GetSector() == sector)
            {
                FamilyNode* family = house->GetChild();
                while (family != nullptr)
                {
                    count++;
                    family = family->GetNext();
                }
            }
            house = house->GetNext();
        }
        return count;
    }

    HouseNode* GetHead() 
    {
        return head; 
    }

    string GetGenderRatioReport()
    {
        return citizenTable->GenerateGenderRatio();
    }

    string GetSectorPopulationReport()
    {
        return citizenTable->GenerateSectorPopulationSummary();
    }

    string GetOccupationReport()
    {
        return citizenTable->GenerateOccupationSummary();
    }

    string GetAgeDistributionReport()
    {
        return citizenTable->GenerateAgeDistribution();
    }

    string GetPopulationDensityReport()
    {
        return citizenTable->CalculatePopulationDensity();
    }

    void SearchCitizenByCNIC(string cnic)
    {
        string name, occup, gender, sector, street;
        int age, house;

        bool found = citizenTable->SearchByCNIC(cnic, name, age, occup, gender, sector, street, house);

        if (!found)
        {
            cout << "No citizen found with CNIC: " << cnic << endl;
        }
        else
        {
            cout << "=== CITIZEN FOUND ===" << endl;
            cout << "CNIC: " << cnic << endl;
            cout << "Name: " << name << endl;
            cout << "Age: " << age << endl;
            cout << "Occupation: " << occup << endl;
            cout << "Gender: " << gender << endl;
            cout << "Address: " << sector << ", " << street << " " << house << endl;
        }
    }

    int GetTotalPopulation()
    {
        int count = 0;
        HouseNode* current = head;
        while (current != nullptr)
        {
            FamilyNode* family = current->GetChild();
            while (family != nullptr)
            {
                count += family->GetMemberCount();
                family = family->GetNext();
            }
            current = current->GetNext();
        }
        return count;
    }

    void GenerateSurveyReport()
    {
        cout << "=== SMART CITY POPULATION SURVEY ===" << endl;
        cout << "=====================================" << endl;

        cout << "1. GENDER RATIO (Male:Female:Unidentified):" << endl;
        cout << GetGenderRatioReport() << endl;

        cout << "2. SECTOR POPULATION DISTRIBUTION:" << endl;
        cout << GetSectorPopulationReport();

        cout << "3. OCCUPATION SUMMARY:" << endl;
        cout << GetOccupationReport();

        cout << "4. AGE DISTRIBUTION (Average Age per Sector):" << endl;
        cout << GetAgeDistributionReport();

        cout << "5. POPULATION DENSITY (People per Sector):" << endl;
        cout << GetPopulationDensityReport();

        cout << "6. TOTAL POPULATION COUNT:" << endl;
        cout << "Total Residents: " << GetTotalPopulation() << endl;

        cout << "=====================================" << endl;
        cout << "=== SURVEY COMPLETE ===" << endl;
    }

    ~HousingSystem()
    {
        HouseNode* current = head;
        while (current != nullptr)
        {
            HouseNode* nextHouse = current->GetNext();

            FamilyNode* family = current->GetChild();
            while (family != nullptr)
            {
                FamilyNode* nextFamily = family->GetNext();

                IndividualNode* person = family->GetChild();
                while (person != nullptr)
                {
                    IndividualNode* nextPerson = person->GetNext();
                    delete person;
                    person = nextPerson;
                }

                delete family;
                family = nextFamily;
            }

            delete current;
            current = nextHouse;
        }
        delete citizenTable;
    }

    void DisplayMainMenu()
    {
        cout << "=== SMART CITY HOUSING MANAGEMENT SYSTEM ===" << endl;
        cout << "1. Add New House" << endl;
        cout << "2. Add Family to House" << endl;
        cout << "3. Add Member to Family" << endl;
        cout << "4. Search Citizen by CNIC" << endl;
        cout << "5. Search Citizen by Name" << endl;
        cout << "6. View Houses in Sector" << endl;
        cout << "7. Display Housing Hierarchy" << endl;
        cout << "8. Generate Survey Reports" << endl;
        cout << "9. Display Statistics" << endl;
        cout << "10. Exit" << endl;
        cout << "Enter your choice (1-10): ";
    }

    void DisplaySurveyMenu()
    {
        cout << "=== SURVEY REPORTS ===" << endl;
        cout << "1. Gender Ratio Report" << endl;
        cout << "2. Occupation Summary Report" << endl;
        cout << "3. Age Distribution Report" << endl;
        cout << "4. Population Density Report" << endl;
        cout << "5. Sector Population Report" << endl;
        cout << "6. All Reports (Complete Survey)" << endl;
        cout << "7. Back to Main Menu" << endl;
        cout << "Enter your choice (1-7): ";
    }

    void DisplayStatsMenu()
    {
        cout << "=== SYSTEM STATISTICS ===" << endl;
        cout << "1. Total Population" << endl;
        cout << "2. Total Houses" << endl;
        cout << "3. Total Families" << endl;
        cout << "4. Families in Specific Sector" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "Enter your choice (1-5): ";
    }
};
