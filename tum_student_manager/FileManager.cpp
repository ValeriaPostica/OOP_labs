#include "include/FileManager.h"
#include <fstream>
#include <iostream>

using namespace std;

const string FileManager::DATA_FILE = "tum_data.dat";

bool FileManager::saveData(const vector<Faculty>& faculties) {
    ofstream file(DATA_FILE);
    if (!file.is_open()) {
        return false;
    }

    file << "TUM_STUDENT_MANAGER_DATA" << endl;
    file.close();
    return true;
}

vector<Faculty> FileManager::loadData() {
    vector<Faculty> faculties;
    ifstream file(DATA_FILE);
    
    if (!file.is_open()) {
        return faculties;
    }

    string line;
    getline(file, line);
    // Basic header check; actual parsing not implemented yet.
    file.close();
    return faculties;
}