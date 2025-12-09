#ifndef CAR_STATION_HPP
#define CAR_STATION_HPP
#include "Car.hpp"
#include "../task1/Queue.hpp"
#include "../task2/DineableRefuelable.hpp"
#include <string>
#include <iostream>
using namespace std;

class CarStation {
private:
    Dineable* diningService;      
    Refuelable* refuelingService;
    Queue<Car>* carQueue;         
    
    int totalCarsServed;
    int electricCarsServed;
    int gasCarsServed;
    int peopleServed;
    int robotsServed;
    int totalConsumption;

public:
    CarStation(Dineable* dining, Refuelable* refueling, Queue<Car>* queue)
        : diningService(dining), refuelingService(refueling), carQueue(queue),
          totalCarsServed(0), electricCarsServed(0), gasCarsServed(0),
          peopleServed(0), robotsServed(0), totalConsumption(0) {}
    
    void addCar(const Car& car) {
        carQueue->enqueue(car);
        cout << "Added Car " << car.getId() << " to queue" << endl;
    }
    
    void serveCars() {
        cout << "STARTING CAR SERVICE" << endl;
        
        ServiceStats* globalStats = ServiceStats::getInstance();
        
        while (!carQueue->isEmpty()) {
            // 1. Dequeue next car (FIFO)
            Car car = carQueue->dequeue();
            
            cout << "\nProcessing Car " << car.getId() << ":" << endl;
            car.print();
            
            string carId = "car" + to_string(car.getId());
            
            // 2. Serve dinner if car wants it
            if (car.getIsDining()) {
                diningService->serveDinner(carId);
                
                if (car.getPassengers() == PassengerType::PEOPLE) {
                    globalStats->incrementPeople();
                    peopleServed++;  
                } else {
                    globalStats->incrementRobots();
                    robotsServed++;  
                }
            } else {
                cout << "  No dining required" << endl;
            }
            
            // 3. Always refuel the car
            refuelingService->refuel(carId);
            
            // 4. Update car type statistics
            if (car.getType() == CarType::ELECTRIC) {
                globalStats->incrementElectric();
                electricCarsServed++;  
            } else {
                globalStats->incrementGas();
                gasCarsServed++;  
            }
            
            // 5. Update consumption
            totalConsumption += car.getConsumption();
            totalCarsServed++;
            
            cout << "  Service completed for Car " << car.getId() << endl;
        }
        
        cout << "ALL CARS SERVED" << endl;
    }
    
    bool isQueueEmpty() const {
        return carQueue->isEmpty();
    }
    
    int getQueueSize() const {
        return carQueue->size();
    }
    
    int getTotalCarsServed() const { return totalCarsServed; }
    int getElectricCarsServed() const { return electricCarsServed; }
    int getGasCarsServed() const { return gasCarsServed; }
    int getPeopleServed() const { return peopleServed; }
    int getRobotsServed() const { return robotsServed; }
    int getTotalConsumption() const { return totalConsumption; }
};
#endif 