#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP
#include "../task3/CarStation.hpp"
#include "../task2/DineableRefuelable.hpp"
#include "../task1/Queue.hpp"
#include <memory>
#include <iostream>
using namespace std;

class Semaphore {
private:
    // Services from Task 2
    ElectricStation electricService;
    GasStation gasService;
    PeopleDinner peopleService;
    RobotDinner robotService;

    // Queues for each station
    unique_ptr<DequeQueue<Car>> q1;
    unique_ptr<DequeQueue<Car>> q2;
    unique_ptr<DequeQueue<Car>> q3;
    unique_ptr<DequeQueue<Car>> q4;

    // The 4 stations (using unique_ptr for automatic memory management)
    unique_ptr<CarStation> stationElectricPeople;
    unique_ptr<CarStation> stationElectricRobots;
    unique_ptr<CarStation> stationGasPeople;
    unique_ptr<CarStation> stationGasRobots;

public:
    Semaphore() {
        // Create queues
        q1 = make_unique<DequeQueue<Car>>();
        q2 = make_unique<DequeQueue<Car>>();
        q3 = make_unique<DequeQueue<Car>>();
        q4 = make_unique<DequeQueue<Car>>();
        
        // Initialize stations with correct dependencies
        stationElectricPeople = make_unique<CarStation>(&peopleService, &electricService, q1.get());
        stationElectricRobots = make_unique<CarStation>(&robotService, &electricService, q2.get());
        stationGasPeople = make_unique<CarStation>(&peopleService, &gasService, q3.get());
        stationGasRobots = make_unique<CarStation>(&robotService, &gasService, q4.get());
        
        cout << "Semaphore created with 4 specialized stations:" << endl;
        cout << "1. Electric + People" << endl;
        cout << "2. Electric + Robots" << endl;
        cout << "3. Gas + People" << endl;
        cout << "4. Gas + Robots" << endl;
    }

    // Guides a car to the correct station based on its type and passengers
    void guideCar(const Car& car) {
        string type = car.getTypeString();
        string passengers = car.getPassengersString();
        
        if (type == "ELECTRIC") {
            if (passengers == "PEOPLE") {
                stationElectricPeople->addCar(car);
            } else if (passengers == "ROBOTS") {
                stationElectricRobots->addCar(car);
            }
        } else if (type == "GAS") {
            if (passengers == "PEOPLE") {
                stationGasPeople->addCar(car);
            } else if (passengers == "ROBOTS") {
                stationGasRobots->addCar(car);
            }
        }
    }

    void serveAll() {
        cout << "\nSERVING ALL 4 STATIONS" << endl;
        cout << "\n1. Electric-People Station:" << endl;
        stationElectricPeople->serveCars();
        cout << "\n2. Electric-Robots Station:" << endl;
        stationElectricRobots->serveCars();
        cout << "\n3. Gas-People Station:" << endl;
        stationGasPeople->serveCars();
        cout << "\n4. Gas-Robots Station:" << endl;
        stationGasRobots->serveCars();
        cout << "ALL STATIONS COMPLETED" << endl;
    }

    // Get statistics using ServiceStats from Task 2
    int getPeople() const {
        return ServiceStats::getInstance()->getPeopleServed();
    }
    int getRobots() const {
        return ServiceStats::getInstance()->getRobotsServed();
    }
    int getGas() const {
        return ServiceStats::getInstance()->getGasCarsServed();
    }
    int getElectric() const {
        return ServiceStats::getInstance()->getElectricCarsServed();
    }
    
    void printSummary() const {
        cout << "\nSUMMARY" << endl;
        cout << "Total electric cars: " << getElectric() << endl;
        cout << "Total gas cars: " << getGas() << endl;
        cout << "People served dinner: " << getPeople() << endl;
        cout << "Robots served dinner: " << getRobots() << endl;
    }
};
#endif 