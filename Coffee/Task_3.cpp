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

    void printIntensityStep() const {
        cout << "Setting intensity to " << intensityToString(coffeeIntensity) << endl;
    }
    
public:
    Coffee(Intensity cI, string name)
        : coffeeIntensity(cI), name(std::move(name)) {}

    virtual ~Coffee() = default;

    Intensity getCoffeeIntensity() const {
        return coffeeIntensity;
    }

    const string& getName() const {
        return name;
    }

    virtual void makeCoffee() const final {
        cout << "\nPreparing " << name << endl;
        printIntensityStep();
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

    Americano makeAmericano() const {
        Coffee::makeCoffee();
        cout << "Pouring " << mlOfWater << " ml of water" << endl;
        return Americano(getCoffeeIntensity(), mlOfWater);
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

    Cappuccino makeCappuccino() const {
        Coffee::makeCoffee();
        cout << "Adding " << mlOfMilk << " ml of milk" << endl;
        return Cappuccino(getCoffeeIntensity(), mlOfMilk);
    }
};

class SyrupCappuccino : public Cappuccino {
protected:
    SyrupType syrup;

public:
    SyrupCappuccino(Intensity intensity, int milk, SyrupType sType, string name = "Syrup Cappuccino")
        : Cappuccino(intensity, milk, name), syrup(sType) {}

    SyrupCappuccino makeSyrupCappuccino() const {
        Cappuccino::makeCappuccino();
        cout << "Mixing in " << syrupTypeToString(syrup) << " syrup" << endl;
        return SyrupCappuccino(getCoffeeIntensity(), mlOfMilk, syrup);
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

    PumpkinSpiceLatte makePumpkinSpiceLatte() const {
        Cappuccino::makeCappuccino();
        cout << "Sprinkling " << mgOfPumpkinSpice << " mg of pumpkin spice" << endl;
        return PumpkinSpiceLatte(getCoffeeIntensity(), mlOfMilk, mgOfPumpkinSpice);
    }
};

int main() {
    Americano americano(NORMAL, 150);
    auto americanoMade = americano.makeAmericano();

    Cappuccino cappuccino(STRONG, 100);
    auto cappuccinoMade = cappuccino.makeCappuccino();

    SyrupCappuccino syrupCappuccino(LIGHT, 120, VANILLA);
    auto syrupCappuccinoMade = syrupCappuccino.makeSyrupCappuccino();

    PumpkinSpiceLatte pumpkinSpiceLatte(NORMAL, 130, 50);
    auto pumpkinSpiceLatteMade = pumpkinSpiceLatte.makePumpkinSpiceLatte();

    return 0;
}