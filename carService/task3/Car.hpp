#ifndef CAR_HPP
#define CAR_HPP
#include <string>
#include <iostream>
using namespace std;

enum class CarType { ELECTRIC, GAS };
enum class PassengerType { PEOPLE, ROBOTS };

class Car {
private:
    int id;
    CarType type;
    PassengerType passengers;
    bool isDining;
    int consumption;

public:
    Car() : id(0), type(CarType::ELECTRIC), passengers(PassengerType::PEOPLE), 
            isDining(false), consumption(0) {}
    
    Car(int id, CarType type, PassengerType passengers, bool isDining, int consumption)
        : id(id), type(type), passengers(passengers), isDining(isDining), 
          consumption(consumption) {}
    
    int getId() const { return id; }
    CarType getType() const { return type; }
    PassengerType getPassengers() const { return passengers; }
    bool getIsDining() const { return isDining; }
    int getConsumption() const { return consumption; }
    
    string getTypeString() const {
        return (type == CarType::ELECTRIC) ? "ELECTRIC" : "GAS";
    }
    
    string getPassengersString() const {
        return (passengers == PassengerType::PEOPLE) ? "PEOPLE" : "ROBOTS";
    }
    
    string getDiningString() const {
        return isDining ? "DINING" : "NOT_DINING";
    }
    
    void print() const {
        cout << "Car " << id << ": Type=" << getTypeString() 
             << ", Passengers=" << getPassengersString()
             << ", Dining=" << getDiningString()
             << ", Consumption=" << consumption << endl;
    }
    
    static Car createFromStrings(int id, const string& typeStr, 
                                 const string& passengersStr, 
                                 bool isDining, int consumption) {
        CarType type = (typeStr == "ELECTRIC") ? CarType::ELECTRIC : CarType::GAS;
        PassengerType passengers = (passengersStr == "PEOPLE") ? 
                                   PassengerType::PEOPLE : PassengerType::ROBOTS;
        return Car(id, type, passengers, isDining, consumption);
    }
};
#endif 