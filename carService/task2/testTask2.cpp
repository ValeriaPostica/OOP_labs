#include <iostream>
#include "DineableRefuelable.hpp"
using namespace std;

int main() {
    PeopleDinner peopleService;
    RobotDinner robotService;
    ElectricStation electricService;
    GasStation gasService;
    
    peopleService.serveDinner("car1");
    robotService.serveDinner("car2");
    electricService.refuel("car3");
    gasService.refuel("car4");
    
    ServiceStats::getInstance()->printStats();
    
    return 0;
}