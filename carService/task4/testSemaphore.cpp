#include <iostream>
#include <cassert> // For assert (testing)
#include <vector>
#include <string>
#include "Semaphore.hpp"
using namespace std;

Car parseJsonToCar(const string& jsonStr) {
    // Lambda function to extract value from JSON
    auto extractValue = [&](const string& key) -> string {
        string search = "\"" + key + "\":";  // Search for key
        size_t pos = jsonStr.find(search);   // Find position
        if (pos == string::npos) return "";  // Return empty if not found
        
        pos += search.length();  // Move past search string
        
        // Skip spaces and quotes
        while (pos < jsonStr.size() && (jsonStr[pos] == ' ' || jsonStr[pos] == '\"')) 
            pos++;
        
        // Extract value
        string value;
        while (pos < jsonStr.size() && jsonStr[pos] != ',' && jsonStr[pos] != '}') {
            if (jsonStr[pos] != '\"') // Skip quotes
                value += jsonStr[pos];
            pos++;
        }
        return value;
    };
    
    try {
        string idStr = extractValue("id");
        string type = extractValue("type");
        string passengers = extractValue("passengers");
        string isDiningStr = extractValue("isDining");
        string consumptionStr = extractValue("consumption");
        
        int id = stoi(idStr);
        bool isDining = (isDiningStr == "true");
        int consumption = stoi(consumptionStr);
        
        return Car::createFromStrings(id, type, passengers, isDining, consumption);
        
    } catch (...) {
        return Car(); // Return default car on error
    }
}

int main() {
    cout << "Task 4: Semaphore Tests" << endl;
    ServiceStats::getInstance()->reset();
    Semaphore semaphore;
    
    vector<string> jsonInputs = {
        "{\"id\":1,\"type\":\"ELECTRIC\",\"passengers\":\"PEOPLE\",\"isDining\":false,\"consumption\":32}",
        "{\"id\":2,\"type\":\"ELECTRIC\",\"passengers\":\"PEOPLE\",\"isDining\":false,\"consumption\":21}",
        "{\"id\":3,\"type\":\"GAS\",\"passengers\":\"ROBOTS\",\"isDining\":true,\"consumption\":35}",
        "{\"id\":4,\"type\":\"GAS\",\"passengers\":\"PEOPLE\",\"isDining\":true,\"consumption\":19}"  
    };
    cout << "Guiding cars..." << endl;
    
    for (const auto& jsonStr : jsonInputs) {
        Car car = parseJsonToCar(jsonStr);
        if (car.getId() != 0) { // Valid car
            cout << "Parsed car " << car.getId() << endl;
            semaphore.guideCar(car);
        } else {
            cout << "Failed to parse: " << jsonStr << endl;
        }
    }
    
    cout << "Serving all cars..." << endl;
    semaphore.serveAll();
    semaphore.printSummary();
    cout << "Verifying statistics..." << endl;
    
    // Test 1: Electric cars count
    assert(semaphore.getElectric() == 2);
    cout << "Electric Cars: " << semaphore.getElectric() << " (Expected: 2)" << endl;
    
    // Test 2: Gas cars count  
    assert(semaphore.getGas() == 2);
    cout << "Gas Cars: " << semaphore.getGas() << " (Expected: 2)" << endl;
    
    // Test 3: People served dinner
    assert(semaphore.getPeople() == 3);
    cout << "People: " << semaphore.getPeople() << " (Expected: 3)" << endl;
    
    // Test 4: Robots served dinner
    assert(semaphore.getRobots() == 1);
    cout << "Robots: " << semaphore.getRobots() << " (Expected: 1)" << endl;
    
    cout << "\nTask 4 requirements met!" << endl;
    return 0;
}