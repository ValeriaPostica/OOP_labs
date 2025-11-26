#include "FileReader.h"
#include <fstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;

FileReader::FileReader(const string& filename) : filename(filename) {}

json FileReader::readFile() {
    ifstream file(filename);
    if (!file) {
        throw runtime_error("Could not open file: " + filename);
    }
    
    json jsonData;
    file >> jsonData;
    return jsonData;
}

void FileReader::printJsonToConsole(const json& data) {
    cout << "RAW JSON CONTENT" << endl;
    cout << data.dump(4) << endl;  //convert JSON to string with 4 space indentation
}

void FileReader::printEachObjectSeparately(const json& data) {
    cout << "\nPRINTING EACH OBJECT SEPARATELY" << endl;
    
    if (data.contains("data") && data["data"].is_array()) {
        for (const auto& entry : data["data"]) {
            int objectId = entry.value("id", -1);
            cout << "\nObject " << (objectId + 1) << "" << endl;
            cout << entry.dump(4) << endl;
        }
    }
}