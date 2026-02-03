#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

int startYear = 1000;
int endYear = 9333;


// absolute path to "data" folder
fs::path base = "absolute path to data folder";


// vector of months
std::vector<std::string> months = {    
    "January", 
    "February", 
    "March", 
    "April", 
    "May", 
    "June",
    "July", 
    "August", 
    "September", 
    "October", 
    "November", 
    "December"
};


// Function for creating path dynamically in runtime
fs::path makePath(int year, const std::string& month) {

    return base / std::to_string(year) / (month + ".txt");

}


// Function for extracting the number from txt file
int extractNumber(const fs::path& filePath) {

    if (!fs::exists(filePath)) {

        std::cerr << "Error: File does not exist -> " << filePath << std::endl;
        std::exit(EXIT_FAILURE);

    }

    std::ifstream file(filePath);

    if (!file.is_open()) {

        std::cerr << "Error: Could not open file -> " << filePath << std::endl;
        std::exit(EXIT_FAILURE);

    }

    std::string line;

    while (std::getline(file, line)) {

        std::string key = "[[Number of people:";
        auto pos = line.find(key);

        if (pos != std::string::npos) {

            std::string number;
            for (char c : line) {
                if (isdigit(c))
                    number += c;
            }
            return std::stoi(number);
        }
    }

    std::cerr << "Error: Pattern not found in " << filePath << std::endl;
    std::exit(EXIT_FAILURE);
}


// Function for getting range within a year
std::pair<int, int> getYearRange(int year) {

    int minVal = -1, maxVal = -1;

    // Find first existing month
    for (int i=0; i<12; i++) {

        fs::path p = makePath(year, months[i]);

        if (fs::exists(p)) {

            minVal = extractNumber(p);
            break;
        }

    }

    // Find last existing month
    for (int i = 11; i >= 0; --i) {

        fs::path p = makePath(year, months[i]);

        if (fs::exists(p)) {

            maxVal = extractNumber(p);
            break;
        }
    }

    if (minVal == -1 || maxVal == -1) {

        std::cerr << "Error: Year " << year << " exists but contains no valid month files." << std::endl;
        std::exit(EXIT_FAILURE);

    }

    return {minVal, maxVal};
}


// function for input and validation of target value, also cheking if target lies on boundary
int getTarget() {

    int input = 0;
    std::cout << "Enter the target value to search : ";
    std::cin >> input;

    // Get dynamic boundaries for start and end years
    std::pair<int, int> startRange = getYearRange(startYear);
    std::pair<int, int> endRange = getYearRange(endYear);

    int globalMin = startRange.first;
    int globalMax = endRange.second;

    if (input == globalMin) {

        std::cout << "Found in " << startYear << std::endl;
        std::exit(EXIT_SUCCESS);

    }
    if (input == globalMax) {

        std::cout << "Found in " << endYear << std::endl;
        std::exit(EXIT_SUCCESS);

    }

    if (input > globalMin && input < globalMax) {

        return input;

    } else {

        std::cout << "Given value is out of Range\n";
        std::exit(0);
    }
}


// function for binary search inside a year
void searchInsideYear(int year, int target) {

    // Collecting only months that actually exist
    std::vector<std::string> validMonths;

    for (int i=0; i<12; i++) {

        if (fs::exists(makePath(year, months[i]))) {

            validMonths.push_back(months[i]);
        }

    }

    // Binary Search on the valid months
    int low = 0;
    int high = validMonths.size() - 1;

    while (low <= high) {

        int mid = (low + high) / 2;
        int val = extractNumber(makePath(year, validMonths[mid]));

        if (val == target) {

            std::cout << "Found in " << year << " " << validMonths[mid] << "\n";
            return;

        } else if (target < val) {

            high = mid - 1;

        } else {

            low = mid + 1;
        }
    }

    std::cout << "Value does not exist in data" << std::endl;
}


// function for binary search across years
void searchAcrossYears(int start, int last, int target) {

    int low = start;
    int high = last;

    while (low <= high) {

        int mid = (low + high) / 2;

        // Dynamically finding the min and max for this specific year
        std::pair<int, int> range = getYearRange(mid);

        int currentYearMin = range.first;
        int currentYearMax = range.second;

        // Checking boundaries
        if (target == currentYearMin || target == currentYearMax) {

            // to print the exact month name correctly
            searchInsideYear(mid, target); 
            return;

        }

        if (target < currentYearMin) {

            high = mid - 1;

        } else if (target > currentYearMax) {

            low = mid + 1;

        } else {

            // Target is inside this year
            searchInsideYear(mid, target);
            return;
        }
    }
    
    std::cout << "Value not found after checking year ranges." << std::endl;
}


int main() {


    int target = getTarget();

    searchAcrossYears(startYear, endYear, target);


    return 0;
}
