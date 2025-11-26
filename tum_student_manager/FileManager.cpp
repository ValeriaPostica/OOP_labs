#include "include/FileManager.h"
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <direct.h>

using namespace std;

static string fieldToString(StudyField f) {
    switch (f) {
        case StudyField::MECHANICAL_ENGINEERING: return "MECHANICAL_ENGINEERING";
        case StudyField::SOFTWARE_ENGINEERING: return "SOFTWARE_ENGINEERING";
        case StudyField::FOOD_TECHNOLOGY: return "FOOD_TECHNOLOGY";
        case StudyField::URBANISM_ARCHITECTURE: return "URBANISM_ARCHITECTURE";
        case StudyField::VETERINARY_MEDICINE: return "VETERINARY_MEDICINE";
    }
    return "UNKNOWN";
}

static StudyField stringToField(const string& s) {
    if (s == "MECHANICAL_ENGINEERING") return StudyField::MECHANICAL_ENGINEERING;
    if (s == "SOFTWARE_ENGINEERING") return StudyField::SOFTWARE_ENGINEERING;
    if (s == "FOOD_TECHNOLOGY") return StudyField::FOOD_TECHNOLOGY;
    if (s == "URBANISM_ARCHITECTURE") return StudyField::URBANISM_ARCHITECTURE;
    return StudyField::VETERINARY_MEDICINE;
}

static vector<string> split(const string& s, char delim) {
    vector<string> out;
    string cur;
    for (char c : s) {
        if (c == delim) { out.push_back(cur); cur.clear(); }
        else cur.push_back(c);
    }
    out.push_back(cur);
    return out;
}

static string studentToLine(const Student& st) {
    // Format: first|last|email|eY|eM|eD|bY|bM|bD
    tm e = st.getEnrollmentDate();
    tm b = st.getDateOfBirth();
    int eY = e.tm_year + 1900;
    int eM = e.tm_mon + 1;
    int eD = e.tm_mday;
    int bY = b.tm_year + 1900;
    int bM = b.tm_mon + 1;
    int bD = b.tm_mday;
    string line = st.getFirstName() + "|" + st.getLastName() + "|" + st.getEmail() + "|" +
                  to_string(eY) + "|" + to_string(eM) + "|" + to_string(eD) + "|" +
                  to_string(bY) + "|" + to_string(bM) + "|" + to_string(bD);
    return line;
}

static Student studentFromParts(const vector<string>& parts) {
    // parts: first,last,email,eY,eM,eD,bY,bM,bD (size==9 expected)
    string first = parts[0];
    string last = parts[1];
    string email = parts[2];
    int eY = stoi(parts[3]);
    int eM = stoi(parts[4]);
    int eD = stoi(parts[5]);
    int bY = stoi(parts[6]);
    int bM = stoi(parts[7]);
    int bD = stoi(parts[8]);
    tm e = {};
    e.tm_year = eY - 1900;
    e.tm_mon = eM - 1;
    e.tm_mday = eD;
    tm b = {};
    b.tm_year = bY - 1900;
    b.tm_mon = bM - 1;
    b.tm_mday = bD;
    return Student(first, last, email, e, b);
}

bool FileManager::save(const vector<Faculty>& faculties, const string& dir) {
    // create directory
    if (!dir.empty()) {
        _mkdir(dir.c_str());
    }

    // 1) faculties.txt
    string facPath = dir + "/faculties.txt";
    ofstream facOut(facPath.c_str());
    if (!facOut.is_open()) return false;
    for (const auto& f : faculties) {
        facOut << f.getName() << '|' << f.getAbbreviation() << '|' << fieldToString(f.getStudyField()) << '\n';
    }
    facOut.close();

    // 2) members.txt
    string memPath = dir + "/members.txt";
    ofstream memOut(memPath.c_str());
    if (!memOut.is_open()) return false;
    for (const auto& f : faculties) {
        for (const auto& s : f.getStudents()) {
            char kind = s.isGraduated() ? 'G' : 'E';
            memOut << f.getAbbreviation() << '|' << kind << '|' << studentToLine(s) << '\n';
        }
    }
    memOut.close();
    return true;
}

vector<Faculty> FileManager::load(const string& dir) {
    vector<Faculty> result;

    // read faculties metadata
    string facPath = dir + "/faculties.txt";
    ifstream facIn(facPath.c_str());
    struct FacMeta { string name; string abbr; StudyField field; };
    vector<FacMeta> metas;
    if (facIn.is_open()) {
        string line;
        while (getline(facIn, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            if (p.size() != 3) continue;
            metas.push_back(FacMeta{p[0], p[1], stringToField(p[2])});
        }
        facIn.close();
    }

    // read members
    string memPath = dir + "/members.txt";
    unordered_map<string, vector<Student>> enrolled, grads;
    ifstream memIn(memPath.c_str());
    if (memIn.is_open()) {
        string line;
        while (getline(memIn, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            // expect: abbr|E/G|first|last|email|eY|eM|eD|bY|bM|bD  => size 11
            if (p.size() != 11) continue;
            string abbr = p[0];
            char kind = p[1].empty() ? 'E' : p[1][0];
            vector<string> sp(p.begin()+2, p.end()); // 9 parts
            Student s = studentFromParts(sp);
            if (kind == 'G') {
                s.setGraduated(true);
                grads[abbr].push_back(s);
            } else {
                enrolled[abbr].push_back(s);
            }
        }
        memIn.close();
    }

    // build faculties
    for (const auto& m : metas) {
        Faculty f(m.name, m.abbr, m.field);
        if (enrolled.count(m.abbr)) {
            for (const auto& s : enrolled[m.abbr]) f.addStudent(s);
        }
        if (grads.count(m.abbr)) {
            for (auto s : grads[m.abbr]) {
                // ensure graduate flag set
                s.setGraduated(true);
                f.addStudent(s);
            }
        }
        result.push_back(std::move(f));
    }

    return result;
}