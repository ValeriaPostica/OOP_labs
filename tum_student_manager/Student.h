#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <ctime>

using namespace std;

class Student {
private:
    string firstName;
    string lastName;
    string email;
    tm enrollmentDate;
    tm dateOfBirth;
    bool graduated;
    string studentId;

    string generateStudentId();

public:
    Student(const string& firstName, const string& lastName, 
            const string& email, const tm& enrollmentDate, 
            const tm& dateOfBirth);
    
    // Getters
    string getFirstName() const;
    string getLastName() const;
    string getEmail() const;
    tm getEnrollmentDate() const;
    tm getDateOfBirth() const;
    bool isGraduated() const;
    string getStudentId() const;
    
    // Setters
    void setGraduated(bool graduated);
    
    string toString() const;
};

#endif