#include <iostream>
#include <cassert>
#include "CarStation.hpp"
#include "../task1/Queue.hpp"
#include "../task2/DineableRefuelable.hpp"
using namespace std;

int main() {
    cout << "CarStation Tests" << endl;

    ServiceStats* stats = ServiceStats::getInstance();
    
    // Reset global stats before testing
    stats->reset();
    
    cout << "\nTest 1: Empty Queue" << endl;
    {
        PeopleDinner dinner;
        ElectricStation fuel;
        DequeQueue<Car> queue;
        CarStation station(&dinner, &fuel, &queue);
        
        assert(station.isQueueEmpty());
        station.serveCars();  
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
        
        assert(stats->getPeopleServed() == 1);
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

         
        assert(stats->getGasCarsServed() == 3);
        assert(stats->getRobotsServed() == 3);  // Only 2 wanted dining
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
        
        assert(stats->getPeopleServed() == 2);
        cout << "Passed" << endl;
    }
    
    cout << "\nFinal Statistics:" << endl;
    stats->printStats();
    
    cout << "\nAll Task 3 tests passed!" << endl;
    return 0;
}