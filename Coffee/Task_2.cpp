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

    Intensity getCoffeeIntensity() const {
        return coffeeIntensity;
    }

    const string& getName() const {
        return name;
    }

    virtual void printDetails() const {
        cout << "\n " << name << " Details" << endl;
        cout << "Coffee intensity: " << intensityToString(coffeeIntensity) << endl;
    }
};

class Americano : public Coffee {
protected:
    int mlOfWater;

public:
    Americano(Intensity intensity, int water, const string& name = "Americano")
        : Coffee(intensity, name), mlOfWater(water) {
        if (mlOfWater < 0) {
            throw invalid_argument("Water amount must be positive");
        }
    }

    void printDetails() const override {
        Coffee::printDetails();
        cout << "Water: " << mlOfWater << " ml" << endl;
    }
};

class Cappuccino : public Coffee {
protected:
    int mlOfMilk;

public:
    Cappuccino(Intensity intensity, int milk, const string& name = "Cappuccino")
        : Coffee(intensity, name), mlOfMilk(milk) {
        if (mlOfMilk < 0) {
            throw invalid_argument("Milk amount must be positive");
        }
    }

    void printDetails() const override {
        Coffee::printDetails();
        cout << "Milk: " << mlOfMilk << " ml" << endl;
    }
};

class SyrupCappuccino : public Cappuccino {
protected:
    SyrupType syrup;

public:
    SyrupCappuccino(Intensity intensity, int milk, SyrupType sType, string name = "Syrup Cappuccino")
        : Cappuccino(intensity, milk, name), syrup(sType) {}

    void printDetails() const override {
        Cappuccino::printDetails();
        cout << "Syrup type: " << syrupTypeToString(syrup) << endl;
    }
};

class PumpkinSpiceLatte : public Cappuccino {
    int mgOfPumpkinSpice;

public: 
    PumpkinSpiceLatte(Intensity intensity, int milk, int spice, string name = "Pumpkin Spice Latte")
        : Cappuccino(intensity, milk, name), mgOfPumpkinSpice(spice) {
        if (mgOfPumpkinSpice < 0) {
            throw invalid_argument("Pumpkin spice must be positive");
        }
    }

    void printDetails() const override {
        Cappuccino::printDetails();
        cout << "Pumpkin spice: " << mgOfPumpkinSpice << " mg" << endl;
    }
};

int main() {
    Americano americano(NORMAL, 150);
    americano.printDetails();

    Cappuccino cappuccino(STRONG, 100);
    cappuccino.printDetails();

    SyrupCappuccino syrupCappuccino(LIGHT, 120, VANILLA);
    syrupCappuccino.printDetails();

    PumpkinSpiceLatte pumpkinSpiceLatte(NORMAL, 130, 50);
    pumpkinSpiceLatte.printDetails();

    return 0;
}