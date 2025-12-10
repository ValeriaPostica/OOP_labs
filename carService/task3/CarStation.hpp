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

public:
    CarStation(Dineable* dining, Refuelable* refueling, Queue<Car>* queue)
        : diningService(dining), refuelingService(refueling), carQueue(queue) {}
    
    void addCar(const Car& car) {
        carQueue->enqueue(car);
        cout << "Added Car " << car.getId() << " to queue" << endl;
    }
    
    void serveCars() {
        
        ServiceStats* globalStats = ServiceStats::getInstance();
        
        while (!carQueue->isEmpty()) {
            // 1. Dequeue next car (FIFO)
            Car car = carQueue->dequeue();
            
            cout << "Processing Car " << car.getId() << ":" << endl;
            car.print();
            
            string carId = "car" + to_string(car.getId());
            
            // 2. Serve dinner if car wants it
            diningService->serveDinner(carId);
            
            if (car.getPassengers() == PassengerType::PEOPLE) {
                globalStats->incrementPeople(); 
            } else {
                globalStats->incrementRobots();
            }
            
            // 3. Always refuel the car
            refuelingService->refuel(carId);
            
            cout << "  Service completed for Car " << car.getId() << endl;
        }
        
    }
    
    bool isQueueEmpty() const {
        return carQueue->isEmpty();
    }
    
    int getQueueSize() const {
        return carQueue->size();
    }
    
};
#endif 