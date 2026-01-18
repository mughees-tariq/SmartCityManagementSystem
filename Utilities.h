#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <ctime>
using namespace std;

class InputValidator {
public:
    // Validate non-empty string
    static bool isValidString(const string& str) {
        if (str.empty()) {
            return false;
        }
        for (int i = 0; i < str.length(); i++) {
            if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && str[i] != '\r') {
                return true;
            }
        }
        return false;
    }

    // Valid CNIC format: xxxxx-xxxxxxx-x
    static bool isValidCNIC(const string& cnic) {
        if (cnic.length() != 15) {
            return false;
        }
        if (cnic[5] != '-' || cnic[13] != '-') {
            return false;
        }
        for (int i = 0; i < cnic.length(); i++) {
            if (i == 5 || i == 13) {
                continue;
            }
            if (cnic[i] < '0' || cnic[i] > '9') return false;
        }
        return true;
    }

    // age
    static bool isValidAge(int age) {
        return age >= 0 && age <= 150;
    }

    // rating
    static bool isValidRating(double rating) {
        return rating >= 0.0 && rating <= 5.0;
    }

    // coordinates
    static bool isValidLatitude(double lat) {
        return lat >= -90.0 && lat <= 90.0;
    }

    static bool isValidLongitude(double lon) {
        return lon >= -180.0 && lon <= 180.0;
    }

    // positive number
    static bool isPositive(int num) {
        return num > 0;
    }

    static bool isPositive(double num) {
        return num > 0.0;
    }

    // Validate menu choice
    static bool isValidMenuChoice(int choice, int min, int max) {
        return choice >= min && choice <= max;
    }
};

class StringUtils {
public:
    // Trim spaces from start and end
    static string trim(const string& str) {
        int start = 0;
        int end = str.length() - 1;

        while (start <= end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n' || str[start] == '\r')) {
            start++;
        }

        while (end >= start && (str[end] == ' ' || str[end] == '\t' ||
            str[end] == '\n' || str[end] == '\r')) {
            end--;
        }

        if (start > end) return "";
        return str.substr(start, end - start + 1);
    }

    // Split string by a char - returns array and size
    static string* split(const string& str, char delimiter, int& count) {
        if (str.empty()) {
            count = 0;
            return nullptr;
        }

        count = 1;
        // Count delimiters
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == delimiter) count++;
        }

        string* result = new string[count];
        int idx = 0;
        string current = "";

        for (int i = 0; i < str.length(); i++) {
            if (str[i] == delimiter) {
                result[idx++] = trim(current);
                current = "";
            }
            else {
                current += str[i];
            }
        }
        result[idx] = trim(current);

        return result;
    }

    // string to double
    static double toDouble(const string& str) {
        if (str.empty()) return 0.0;
        return atof(str.c_str());
    }

    // string to int
    static int toInt(const string& str) {
        if (str.empty()) return 0;

        int result = 0;
        bool negative = false;
        int start = 0;

        if (str[0] == '-') {
            negative = true;
            start = 1;
        }

        for (int i = start; i < str.length(); i++) {
            if (str[i] >= '0' && str[i] <= '9') {
                result = result * 10 + (str[i] - '0');
            }
            else {
                break; // Stop at first non-digit
            }
        }

        return negative ? -result : result;
    }

    //  int to string
    static string toString(int num) {
        if (num == 0) return "0";

        bool negative = num < 0;
        if (negative) num = -num;

        string result = "";
        while (num > 0) {
            result = char('0' + (num % 10)) + result;
            num /= 10;
        }

        if (negative) result = "-" + result;
        return result;
    }

    //  double to string with rounding
    static string toString(double num, int precision = 2) {
        bool negative = num < 0;
        if (negative) num = -num;

        // Add epsilon for rounding
        double epsilon = 0.5;
        for (int i = 0; i < precision; i++) {
            epsilon /= 10.0;
        }
        num += epsilon;

        int intPart = (int)num;
        double fracPart = num - intPart;

        string result = toString(intPart) + ".";

        for (int i = 0; i < precision; i++) {
            fracPart *= 10.0;
            int digit = (int)fracPart;
            result += char('0' + digit);
            fracPart -= digit;
        }

        if (negative) result = "-" + result;
        return result;
    }

    // lowercase
    static string toLower(const string& str) {
        string result = "";
        for (int i = 0; i < str.length(); i++) {
            if (str[i] >= 'A' && str[i] <= 'Z') {
                result += (str[i] + 32);
            }
            else {
                result += str[i];
            }
        }
        return result;
    }

    // Check if string contains substring 
    static bool contains(const string& str, const string& substr) {
        string strLower = toLower(str);
        string substrLower = toLower(substr);

        if (substrLower.length() > strLower.length()) return false;

        for (int i = 0; i <= strLower.length() - substrLower.length(); i++) {
            bool match = true;
            for (int j = 0; j < substrLower.length(); j++) {
                if (strLower[i + j] != substrLower[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return true;
        }
        return false;
    }

    // Remove quotes from string
    static string removeQuotes(const string& str) {
        if (str.length() >= 2 && str[0] == '"' && str[str.length() - 1] == '"') {
            return str.substr(1, str.length() - 2);
        }
        return str;
    }
};

class MathUtils {
public:
    static const double PI;
    static const double EARTH_RADIUS_KM;

    //  degrees to radians
    static double toRadians(double degrees) {
        return degrees * PI / 180.0;
    }

    // we used the Haversine formula for distance
    static double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
        // Validate coordinates
        if (!InputValidator::isValidLatitude(lat1) || !InputValidator::isValidLatitude(lat2) ||
            !InputValidator::isValidLongitude(lon1) || !InputValidator::isValidLongitude(lon2)) {
            return -1.0; // Invalid coordinates
        }

        double dLat = toRadians(lat2 - lat1);
        double dLon = toRadians(lon2 - lon1);

        lat1 = toRadians(lat1);
        lat2 = toRadians(lat2);

        double a = sin(dLat / 2) * sin(dLat / 2) +
            cos(lat1) * cos(lat2) *
            sin(dLon / 2) * sin(dLon / 2);

        double c = 2 * atan2(sqrt(a), sqrt(1 - a));

        return EARTH_RADIUS_KM * c;
    }

    static double abs(double x) {
        return x < 0 ? -x : x;
    }

    static int abs(int x) {
        return x < 0 ? -x : x;
    }

    static double max(double a, double b) {
        return fmax(a,b);
    }

    static double min(double a, double b) {
        return fmin(a,b);
    }
};

// constants
const double MathUtils::PI = 3.14159265358979323846;
const double MathUtils::EARTH_RADIUS_KM = 6371.0;

//Random double for coords
static double randomDoubleInRange(double minVal, double maxVal) {
    if (maxVal <= minVal) return minVal;
    double r = static_cast<double>(rand()) / static_cast<double>(RAND_MAX); // in [0,1]
    return minVal + r * (maxVal - minVal);
}

class SafeInput {
public:
    // Safe integer input with validation
    static bool getInt(int& value, int min = -2147483647, int max = 2147483647) {
        if (!(cin >> value)) {
            cin.clear();
            cin.ignore(10000, '\n');
            return false;
        }

        if (value < min || value > max) {
            cin.ignore(10000, '\n');
            return false;
        }

        return true;
    }

    // Safe double input
    static bool getDouble(double& value, double min = -1e308, double max = 1e308) {
        if (!(cin >> value)) {
            cin.clear();
            cin.ignore(10000, '\n');
            return false;
        }

        if (value < min || value > max) {
            cin.ignore(10000, '\n');
            return false;
        }

        return true;
    }

    // Safe string input
    static bool getString(string& value) {
        if (!getline(cin, value)) {
            return false;
        }
        value = StringUtils::trim(value);
        return !value.empty();
    }

    // Get confirmation (y/n)
    static bool getConfirmation(const string& message) {
        cout << message << " (y/n): ";
        char choice;
        cin >> choice;
        cin.ignore(10000, '\n');
        return (choice == 'y' || choice == 'Y');
    }
};
