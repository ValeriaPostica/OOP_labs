#include <iostream>
#include <cassert>
#include "DineableRefuelable.hpp"
using namespace std;

int main() {
    PeopleDinner peopleService;
    RobotDinner robotService;
    ElectricStation electricService;
    GasStation gasService;

    ServiceStats* stats = ServiceStats::getInstance();
    
    peopleService.serveDinner("car1");
    robotService.serveDinner("car2");
    electricService.refuel("car3");
    gasService.refuel("car4");

    assert(stats->getElectricCarsServed() == 1);
    assert(stats->getGasCarsServed() == 1);
    cout << "All tests passed.\n";
    
    return 0;
}