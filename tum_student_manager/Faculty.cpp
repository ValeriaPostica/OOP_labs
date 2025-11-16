#include "Faculty.h"
#include <sstream>

using namespace std;

Faculty::Faculty(const string& name, const string& abbreviation, StudyField studyField)
    : name(name), abbreviation(abbreviation), studyField(studyField) {}

void Faculty::addStudent(const Student& student) {
    students.push_back(student);
}

bool Faculty::graduateStudent(const string& email) {
    for (auto& student : students) {
        if (student.getEmail() == email && !student.isGraduated()) {
            student.setGraduated(true);
            return true;
        }
    }
    return false;
}

vector<Student> Faculty::getEnrolledStudents() const {
    vector<Student> enrolled;
    for (const auto& student : students) {
        if (!student.isGraduated()) {
            enrolled.push_back(student);
        }
    }
    return enrolled;
}

vector<Student> Faculty::getGraduates() const {
    vector<Student> graduates;
    for (const auto& student : students) {
        if (student.isGraduated()) {
            graduates.push_back(student);
        }
    }
    return graduates;
}

bool Faculty::hasStudent(const string& email) const {
    for (const auto& student : students) {
        if (student.getEmail() == email) {
            return true;
        }
    }
    return false;
}

string Faculty::getName() const { return name; }
string Faculty::getAbbreviation() const { return abbreviation; }
StudyField Faculty::getStudyField() const { return studyField; }
vector<Student> Faculty::getStudents() const { return students; }

string Faculty::toString() const {
    stringstream ss;
    ss << name << " (" << abbreviation << ") - ";
    switch (studyField) {
        case StudyField::MECHANICAL_ENGINEERING: ss << "Mechanical Engineering"; break;
        case StudyField::SOFTWARE_ENGINEERING: ss << "Software Engineering"; break;
        case StudyField::FOOD_TECHNOLOGY: ss << "Food Technology"; break;
        case StudyField::URBANISM_ARCHITECTURE: ss << "Urbanism Architecture"; break;
        case StudyField::VETERINARY_MEDICINE: ss << "Veterinary Medicine"; break;
    }
    return ss.str();
}