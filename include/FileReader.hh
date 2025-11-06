//
// Slightly simplified reader that preserves newlines and prints a minimal error when missing.
//

#ifndef FILEREADER_HH
#define FILEREADER_HH

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class FileReader {
public:
    string readFileIntoString(const string& path) {
        ifstream in(path.c_str());
        if (!in.is_open()) {
            cerr << "Error opening: " << path << "\n";
            return "";
        }
        string out, line;
        while (getline(in, line)) {
            out += line;
            out.push_back('\n');
        }
        return out;
    }
};

#endif // FILEREADER_HH
