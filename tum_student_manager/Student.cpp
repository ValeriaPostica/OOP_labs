#include "include/Student.h"
#include <sstream>
#include <iomanip>

using namespace std;

string Student::generateStudentId() {
    return "TUM" + to_string(static_cast<long>(time(nullptr)) % 10000);
}

Student::Student(const string& firstName, const string& lastName, 
                 const string& email, const tm& enrollmentDate, 
                 const tm& dateOfBirth)
    : firstName(firstName), lastName(lastName), email(email), 
      enrollmentDate(enrollmentDate), dateOfBirth(dateOfBirth), graduated(false) {
    studentId = generateStudentId();
}

string Student::getFirstName() const { return firstName; }
string Student::getLastName() const { return lastName; }
string Student::getEmail() const { return email; }
tm Student::getEnrollmentDate() const { return enrollmentDate; }
tm Student::getDateOfBirth() const { return dateOfBirth; }
bool Student::isGraduated() const { return graduated; }
string Student::getStudentId() const { return studentId; }

void Student::setGraduated(bool graduated) { this->graduated = graduated; }

string Student::toString() const {
    stringstream ss;
    ss << firstName << " " << lastName << " (" << email << ") - " 
       << (graduated ? "Graduated" : "Enrolled");
    return ss.str();
}