#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include "../include/nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class FileReader {
private:
    string filename;

public:
    FileReader(const string& filename);
    json readFile();
    void printJsonToConsole(const json& data);
    void printEachObjectSeparately(const json& data);
};

#endif