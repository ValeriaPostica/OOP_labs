#include <iostream>
#include <string>
#include <utility>
#include <stdexcept>
#include "include/Utils.hh"

using namespace std;

class Coffee {
protected:
    Intensity coffeeIntensity;
    const string name;
    
public:
    Coffee(Intensity cI, string n)
        : coffeeIntensity(cI), name(std::move(n)) {}

    virtual ~Coffee() = default;
};

class Americano : public Coffee {
protected:
    int mlOfWater;

public:
    Americano(Intensity intensity, int water, string name = "Americano")
        : Coffee(intensity, std::move(name)), mlOfWater(water) {
        if (mlOfWater < 0) {
            throw invalid_argument("Water amount must be positive");
        }
    }
};

class Cappuccino : public Coffee {
protected:
    int mlOfMilk;

public:
    Cappuccino(Intensity intensity, int milk, string name = "Cappuccino")
        : Coffee(intensity, std::move(name)), mlOfMilk(milk) {
        if (mlOfMilk < 0) {
            throw invalid_argument("Milk amount must be positive");
        }
    }
};

class SyrupCappuccino : public Cappuccino {
private:
    SyrupType syrup;

public:
    SyrupCappuccino(Intensity intensity, int milk, SyrupType sType, string name = "SyrupCappuccino")
        : Cappuccino(intensity, milk, std::move(name)), syrup(sType) {}
};

class PumpkinSpiceLatte : public Cappuccino {
private:
    int mgOfPumpkinSpice;

public: 
    PumpkinSpiceLatte(Intensity intensity, int milk, int spice, string name = "PumpkinSpiceLatte")
        : Cappuccino(intensity, milk, std::move(name)), mgOfPumpkinSpice(spice) {
        if (mgOfPumpkinSpice < 0) {
            throw invalid_argument("Pumpkin spice must be positive");
        }
    }
};

int main() {
    Americano americano(NORMAL, 150);
    Cappuccino cappuccino(STRONG, 100);
    SyrupCappuccino syrupCappuccino(LIGHT, 120, VANILLA);
    PumpkinSpiceLatte pumpkinSpiceLatte(NORMAL, 130, 50);

    return 0;
}