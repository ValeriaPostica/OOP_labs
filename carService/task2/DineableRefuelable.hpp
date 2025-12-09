#ifndef DINEABLE_REFUELABLE_HPP
#define DINEABLE_REFUELABLE_HPP
#include <string>
#include <iostream>
using namespace std;

//INTERFACE 1: Dineable
class Dineable {
public:
    virtual ~Dineable() = default;
    virtual void serveDinner(const string& carId) = 0;
};

//INTERFACE 2: Refuelable
class Refuelable {
public:
    virtual ~Refuelable() = default;
    virtual void refuel(const string& carId) = 0;
};

class ServiceStats {
private:
    static ServiceStats* instance;
    
    int peopleServed = 0;
    int robotsServed = 0;
    int electricCarsServed = 0;
    int gasCarsServed = 0;
    
    ServiceStats() = default;
    
public:
    static ServiceStats* getInstance() {
        if (!instance) {
            instance = new ServiceStats();
        }
        return instance;
    }
    
    int getPeopleServed() const { return peopleServed; }
    int getRobotsServed() const { return robotsServed; }
    int getElectricCarsServed() const { return electricCarsServed; }
    int getGasCarsServed() const { return gasCarsServed; }
    
    void incrementPeople() { peopleServed++; }
    void incrementRobots() { robotsServed++; }
    void incrementElectric() { electricCarsServed++; }
    void incrementGas() { gasCarsServed++; }
    
    // Reset for testing
    void reset() {
        peopleServed = 0;
        robotsServed = 0;
        electricCarsServed = 0;
        gasCarsServed = 0;
    }
    
    void printStats() const {
        cout << "\nSERVICE STATISTICS\n";
        cout << "People served: " << peopleServed << "\n";
        cout << "Robots served: " << robotsServed << "\n";
        cout << "Electric cars refueled: " << electricCarsServed << "\n";
        cout << "Gas cars refueled: " << gasCarsServed << "\n";
    }
};

// Initialize static member
ServiceStats* ServiceStats::instance = nullptr;

//dinner implementation
class PeopleDinner : public Dineable {
public:
    void serveDinner(const string& carId) override {
        cout << "Serving dinner to people in " << carId << endl;
        ServiceStats::getInstance()->incrementPeople();
    }
};

class RobotDinner : public Dineable {
public:
    void serveDinner(const string& carId) override {
        cout << "Serving dinner to robots in " << carId << endl;
        ServiceStats::getInstance()->incrementRobots();
    }
};

//refueling implementation
class ElectricStation : public Refuelable {
public:
    void refuel(const string& carId) override {
        cout << "Refueling electric " << carId << endl;
        ServiceStats::getInstance()->incrementElectric();
    }
};

class GasStation : public Refuelable {
public:
    void refuel(const string& carId) override {
        cout << "Refueling gas " << carId << endl;
        ServiceStats::getInstance()->incrementGas();
    }
};
#endif