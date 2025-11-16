#include "Logger.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

const string Logger::LOG_FILE = "tum_operations.log";

void Logger::logOperation(const string& operation, const string& details) {
    time_t now = time(nullptr);
    tm* timeinfo = localtime(&now);
    
    stringstream timestamp;
    timestamp << put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    
    string logEntry = "[" + timestamp.str() + "] " + operation + ": " + details + "\n";
    
    ofstream file(LOG_FILE, ios::app);
    if (file.is_open()) {
        file << logEntry;
        file.close();
    }
    
    cout << logEntry;
}