#include "include/FileManager.h"
#include <fstream>
#include <iostream>

using namespace std;

const string FileManager::DATA_FILE = "tum_data.dat";

void FileManager::saveData(const vector<Faculty>& faculties) {
    ofstream file(DATA_FILE);
    if (!file.is_open()) {
        cout << "Error: Could not open file for saving data." << endl;
        return;
    }
    
    file << "TUM_STUDENT_MANAGER_DATA" << endl;
    file.close();
    cout << "Data saved successfully!" << endl;
}

vector<Faculty> FileManager::loadData() {
    vector<Faculty> faculties;
    ifstream file(DATA_FILE);
    
    if (!file.is_open()) {
        cout << "No previous data found. Starting fresh." << endl;
        return faculties;
    }
    
    string line;
    getline(file, line);
    
    if (line == "TUM_STUDENT_MANAGER_DATA") {
        cout << "Previous data loaded successfully!" << endl;
    } else {
        cout << "Data file corrupted. Starting fresh." << endl;
    }
    
    file.close();
    return faculties;
}