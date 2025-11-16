#ifndef LOGGER_H
#define LOGGER_H

#include <string>

using namespace std;

class Logger {
private:
    static const string LOG_FILE;
    
public:
    static void logOperation(const string& operation, const string& details);
};

#endif