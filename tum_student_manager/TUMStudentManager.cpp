#include "TUMStudentManager.h"
#include "FileManager.h"
#include "Logger.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <limits>

using namespace std;

TUMStudentManager::TUMStudentManager() {
    faculties = FileManager::loadData();
    if (faculties.empty()) {
        initializeSampleData();
    }
}

void TUMStudentManager::initializeSampleData() {
    faculties.push_back(Faculty("Faculty of Computers, Informatics and Microelectronics", 
                               "FCIM", StudyField::SOFTWARE_ENGINEERING));
    faculties.push_back(Faculty("Faculty of Food Technology", 
                               "FFT", StudyField::FOOD_TECHNOLOGY));
    faculties.push_back(Faculty("Faculty of Mechanical Engineering", 
                               "FME", StudyField::MECHANICAL_ENGINEERING));
    
    Logger::logOperation("SYSTEM_START", "Initialized with sample data");
}

void TUMStudentManager::run() {
    cout << "=== Technical University of Moldova - Student Management System ===" << endl;
    
    while (true) {
        displayMainMenu();
        string choice;
        getline(cin, choice);
        
        if (choice == "1") {
            facultyOperations();
        } else if (choice == "2") {
            generalOperations();
        } else if (choice == "3") {
            batchOperations();
        } else if (choice == "0") {
            FileManager::saveData(faculties);
            cout << "Goodbye!" << endl;
            break;
        } else {
            cout << "Invalid option. Please try again." << endl;
        }
    }
}

void TUMStudentManager::displayMainMenu() {
    cout << "\n--- MAIN MENU ---" << endl;
    cout << "1. Faculty Operations" << endl;
    cout << "2. General Operations" << endl;
    cout << "3. Batch Operations" << endl;
    cout << "0. Exit" << endl;
    cout << "Choose an option: ";
}

void TUMStudentManager::facultyOperations() {
    Faculty* faculty = selectFaculty();
    if (faculty == nullptr) return;

    while (true) {
        cout << "\n--- FACULTY OPERATIONS: " << faculty->getName() << " ---" << endl;
        cout << "1. Create and assign a student" << endl;
        cout << "2. Graduate a student" << endl;
        cout << "3. Display enrolled students" << endl;
        cout << "4. Display graduates" << endl;
        cout << "5. Check if student belongs to faculty" << endl;
        cout << "0. Back to main menu" << endl;
        cout << "Choose an option: ";

        string choice;
        getline(cin, choice);
        
        if (choice == "1") {
            createAndAssignStudent(*faculty);
        } else if (choice == "2") {
            graduateStudent(*faculty);
        } else if (choice == "3") {
            displayEnrolledStudents(*faculty);
        } else if (choice == "4") {
            displayGraduates(*faculty);
        } else if (choice == "5") {
            checkStudentInFaculty(*faculty);
        } else if (choice == "0") {
            break;
        } else {
            cout << "Invalid option." << endl;
        }
    }
}

void TUMStudentManager::createAndAssignStudent(Faculty& faculty) {
    try {
        string firstName, lastName, email, enrollmentDateStr, dobStr;
        
        cout << "Enter first name: ";
        getline(cin, firstName);
        
        cout << "Enter last name: ";
        getline(cin, lastName);
        
        cout << "Enter email: ";
        getline(cin, email);
        
        cout << "Enter enrollment date (yyyy-mm-dd): ";
        getline(cin, enrollmentDateStr);
        
        cout << "Enter date of birth (yyyy-mm-dd): ";
        getline(cin, dobStr);

        tm enrollmentDate = parseDate(enrollmentDateStr);
        tm dateOfBirth = parseDate(dobStr);

        Student student(firstName, lastName, email, enrollmentDate, dateOfBirth);
        faculty.addStudent(student);
        
        Logger::logOperation("STUDENT_CREATED", 
            "Student " + firstName + " " + lastName + " (" + email + ") added to " + faculty.getName());
        cout << "Student created and assigned successfully!" << endl;
        
    } catch (const exception& e) {
        cout << "Error creating student: " << e.what() << endl;
    }
}

void TUMStudentManager::graduateStudent(Faculty& faculty) {
    string email;
    cout << "Enter student email to graduate: ";
    getline(cin, email);
    
    if (faculty.graduateStudent(email)) {
        Logger::logOperation("STUDENT_GRADUATED", 
            "Student " + email + " graduated from " + faculty.getName());
        cout << "Student graduated successfully!" << endl;
    } else {
        cout << "Cannot graduate student: " << email << " (student not found or already graduated)" << endl;
    }
}

void TUMStudentManager::displayEnrolledStudents(const Faculty& faculty) {
    vector<Student> enrolled = faculty.getEnrolledStudents();
    cout << "\n--- Enrolled Students in " << faculty.getName() << " ---" << endl;
    if (enrolled.empty()) {
        cout << "No enrolled students." << endl;
    } else {
        for (const auto& student : enrolled) {
            cout << student.toString() << endl;
        }
    }
}

void TUMStudentManager::displayGraduates(const Faculty& faculty) {
    vector<Student> graduates = faculty.getGraduates();
    cout << "\n--- Graduates from " << faculty.getName() << " ---" << endl;
    if (graduates.empty()) {
        cout << "No graduates." << endl;
    } else {
        for (const auto& student : graduates) {
            cout << student.toString() << endl;
        }
    }
}

void TUMStudentManager::checkStudentInFaculty(const Faculty& faculty) {
    string email;
    cout << "Enter student email: ";
    getline(cin, email);
    
    bool hasStudent = faculty.hasStudent(email);
    cout << "Student " << email << " belongs to " << faculty.getName() << ": " 
              << (hasStudent ? "Yes" : "No") << endl;
}

void TUMStudentManager::generalOperations() {
    while (true) {
        cout << "\n--- GENERAL OPERATIONS ---" << endl;
        cout << "1. Create new faculty" << endl;
        cout << "2. Search student faculty" << endl;
        cout << "3. Display all faculties" << endl;
        cout << "4. Display faculties by field" << endl;
        cout << "0. Back to main menu" << endl;
        cout << "Choose an option: ";

        string choice;
        getline(cin, choice);
        
        if (choice == "1") {
            createNewFaculty();
        } else if (choice == "2") {
            searchStudentFaculty();
        } else if (choice == "3") {
            displayAllFaculties();
        } else if (choice == "4") {
            displayFacultiesByField();
        } else if (choice == "0") {
            break;
        } else {
            cout << "Invalid option." << endl;
        }
    }
}

void TUMStudentManager::createNewFaculty() {
    string name, abbreviation;
    
    cout << "Enter faculty name: ";
    getline(cin, name);
    
    cout << "Enter faculty abbreviation: ";
    getline(cin, abbreviation);
    
    cout << "Available study fields:" << endl;
    cout << "1. Mechanical Engineering" << endl;
    cout << "2. Software Engineering" << endl;
    cout << "3. Food Technology" << endl;
    cout << "4. Urbanism Architecture" << endl;
    cout << "5. Veterinary Medicine" << endl;
    
    cout << "Choose study field (1-5): ";
    string fieldChoice;
    getline(cin, fieldChoice);
    
    StudyField studyField;
    if (fieldChoice == "1") studyField = StudyField::MECHANICAL_ENGINEERING;
    else if (fieldChoice == "2") studyField = StudyField::SOFTWARE_ENGINEERING;
    else if (fieldChoice == "3") studyField = StudyField::FOOD_TECHNOLOGY;
    else if (fieldChoice == "4") studyField = StudyField::URBANISM_ARCHITECTURE;
    else if (fieldChoice == "5") studyField = StudyField::VETERINARY_MEDICINE;
    else {
        cout << "Invalid field choice." << endl;
        return;
    }
    
    Faculty faculty(name, abbreviation, studyField);
    faculties.push_back(faculty);
    
    Logger::logOperation("FACULTY_CREATED", 
        "Faculty " + name + " (" + abbreviation + ") created");
    cout << "Faculty created successfully!" << endl;
}

void TUMStudentManager::searchStudentFaculty() {
    string email;
    cout << "Enter student email: ";
    getline(cin, email);
    
    for (auto& faculty : faculties) {
        if (faculty.hasStudent(email)) {
            cout << "Student " << email << " belongs to: " << faculty.getName() << endl;
            return;
        }
    }
    cout << "Student " << email << " not found in any faculty." << endl;
}

void TUMStudentManager::displayAllFaculties() {
    cout << "\n--- All Faculties ---" << endl;
    if (faculties.empty()) {
        cout << "No faculties available." << endl;
    } else {
        for (const auto& faculty : faculties) {
            cout << faculty.toString() << endl;
        }
    }
}

void TUMStudentManager::displayFacultiesByField() {
    cout << "Available study fields:" << endl;
    cout << "1. Mechanical Engineering" << endl;
    cout << "2. Software Engineering" << endl;
    cout << "3. Food Technology" << endl;
    cout << "4. Urbanism Architecture" << endl;
    cout << "5. Veterinary Medicine" << endl;
    
    cout << "Choose study field (1-5): ";
    string fieldChoice;
    getline(cin, fieldChoice);
    
    StudyField selectedField;
    if (fieldChoice == "1") selectedField = StudyField::MECHANICAL_ENGINEERING;
    else if (fieldChoice == "2") selectedField = StudyField::SOFTWARE_ENGINEERING;
    else if (fieldChoice == "3") selectedField = StudyField::FOOD_TECHNOLOGY;
    else if (fieldChoice == "4") selectedField = StudyField::URBANISM_ARCHITECTURE;
    else if (fieldChoice == "5") selectedField = StudyField::VETERINARY_MEDICINE;
    else {
        cout << "Invalid field choice." << endl;
        return;
    }
    
    cout << "\n--- Faculties in ";
    switch (selectedField) {
        case StudyField::MECHANICAL_ENGINEERING: cout << "Mechanical Engineering"; break;
        case StudyField::SOFTWARE_ENGINEERING: cout << "Software Engineering"; break;
        case StudyField::FOOD_TECHNOLOGY: cout << "Food Technology"; break;
        case StudyField::URBANISM_ARCHITECTURE: cout << "Urbanism Architecture"; break;
        case StudyField::VETERINARY_MEDICINE: cout << "Veterinary Medicine"; break;
    }
    cout << " ---" << endl;
    
    bool found = false;
    for (const auto& faculty : faculties) {
        if (faculty.getStudyField() == selectedField) {
            cout << faculty.toString() << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "No faculties found for this field." << endl;
    }
}

void TUMStudentManager::batchOperations() {
    cout << "\n--- BATCH OPERATIONS ---" << endl;
    cout << "1. Batch enroll students from file" << endl;
    cout << "2. Batch graduate students from file" << endl;
    cout << "0. Back to main menu" << endl;
    cout << "Choose an option: ";

    string choice;
    getline(cin, choice);
    
    if (choice == "1") {
        batchEnrollStudents();
    } else if (choice == "2") {
        batchGraduateStudents();
    } else if (choice == "0") {
        return;
    } else {
        cout << "Invalid option." << endl;
    }
}

void TUMStudentManager::batchEnrollStudents() {
    string filename;
    cout << "Enter filename for batch enrollment: ";
    getline(cin, filename);
    cout << "Batch enrollment from " << filename << " would be implemented here." << endl;
}

void TUMStudentManager::batchGraduateStudents() {
    string filename;
    cout << "Enter filename for batch graduation: ";
    getline(cin, filename);
    cout << "Batch graduation from " << filename << " would be implemented here." << endl;
}

Faculty* TUMStudentManager::selectFaculty() {
    if (faculties.empty()) {
        cout << "No faculties available. Please create a faculty first." << endl;
        return nullptr;
    }

    cout << "\n--- Select Faculty ---" << endl;
    for (size_t i = 0; i < faculties.size(); i++) {
        cout << (i + 1) << ". " << faculties[i].toString() << endl;
    }
    
    cout << "Choose faculty (1-" << faculties.size() << "): ";
    string choice;
    getline(cin, choice);
    
    try {
        int index = stoi(choice) - 1;
        if (index >= 0 && index < static_cast<int>(faculties.size())) {
            return &faculties[index];
        } else {
            cout << "Invalid faculty choice." << endl;
        }
    } catch (const exception& e) {
        cout << "Please enter a valid number." << endl;
    }
    return nullptr;
}

tm TUMStudentManager::parseDate(const string& dateStr) {
    tm date = {};
    istringstream ss(dateStr);
    ss >> get_time(&date, "%Y-%m-%d");
    if (ss.fail()) {
        throw runtime_error("Invalid date format. Please use yyyy-mm-dd.");
    }
    return date;
}

string TUMStudentManager::dateToString(const tm& date) {
    stringstream ss;
    ss << put_time(&date, "%Y-%m-%d");
    return ss.str();
}