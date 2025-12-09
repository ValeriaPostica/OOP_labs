#include <iostream>
#include <cassert>
#include "CarStation.hpp"
#include "../task1/Queue.hpp"
#include "../task2/DineableRefuelable.hpp"
using namespace std;

int main() {
    cout << "CarStation Tests" << endl;
    
    // Reset global stats before testing
    ServiceStats::getInstance()->reset();
    
    cout << "\nTest 1: Empty Queue" << endl;
    {
        PeopleDinner dinner;
        ElectricStation fuel;
        DequeQueue<Car> queue;
        CarStation station(&dinner, &fuel, &queue);
        
        assert(station.isQueueEmpty());
        station.serveCars();  
        assert(station.getTotalCarsServed() == 0);
        cout << "Passed" << endl;
    }
    
    cout << "\nTest 2: Single Car Service" << endl;
    {
        PeopleDinner dinner;
        ElectricStation fuel;
        DequeQueue<Car> queue;
        CarStation station(&dinner, &fuel, &queue);
        
        Car car(1, CarType::ELECTRIC, PassengerType::PEOPLE, true, 42);
        station.addCar(car);
        station.serveCars();
        
        assert(station.getTotalCarsServed() == 1);
        assert(station.getElectricCarsServed() == 1);
        assert(station.getPeopleServed() == 1);
        cout << "Passed" << endl;
    }
    
    cout << "\nTest 3: Multiple Cars FIFO" << endl;
    {
        RobotDinner dinner;
        GasStation fuel;
        VectorQueue<Car> queue;
        CarStation station(&dinner, &fuel, &queue);
        
        station.addCar(Car(10, CarType::GAS, PassengerType::ROBOTS, true, 30));
        station.addCar(Car(11, CarType::GAS, PassengerType::ROBOTS, false, 35));
        station.addCar(Car(12, CarType::GAS, PassengerType::ROBOTS, true, 40));
        
        station.serveCars();
        
        assert(station.getTotalCarsServed() == 3);
        assert(station.getGasCarsServed() == 3);
        assert(station.getRobotsServed() == 2);  // Only 2 wanted dining
        cout << "Passed" << endl;
    }
    
    cout << "\nTest 4: Car Without Dining" << endl;
    {
        PeopleDinner dinner;
        ElectricStation fuel;
        ListQueue<Car> queue;
        CarStation station(&dinner, &fuel, &queue);
        
        Car car(20, CarType::ELECTRIC, PassengerType::PEOPLE, false, 25);
        station.addCar(car);
        station.serveCars();
        
        assert(station.getTotalCarsServed() == 1);
        assert(station.getPeopleServed() == 0);  // No dining
        cout << "Passed" << endl;
    }
    
    cout << "\nTest 5: Dependency Injection" << endl;
    {
        RobotDinner dinner;
        GasStation fuel;
        DequeQueue<Car> queue;
        
        CarStation station(&dinner, &fuel, &queue);
        
        Car car(30, CarType::GAS, PassengerType::ROBOTS, true, 50);
        station.addCar(car);
        station.serveCars();
        
        assert(station.getGasCarsServed() == 1);
        assert(station.getRobotsServed() == 1);
        cout << "Passed" << endl;
    }
    
    cout << "\nFinal Statistics:" << endl;
    ServiceStats::getInstance()->printStats();
    
    cout << "\nAll Task 3 tests passed!" << endl;
    return 0;
}