#include <iostream>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include "include/Utils.hh"

using namespace std;

enum class CoffeeType { 
    AMERICANO, 
    CAPPUCCINO, 
    SYRUP_CAPPUCCINO, 
    PUMPKIN_SPICE_LATTE 
};

struct Order { // simple data passed from main
    CoffeeType type;
    Intensity intensity;
    int mlOfWater = 0;        // Americano
    int mlOfMilk = 0;         // Cappuccino variants
    SyrupType syrup = VANILLA;// Syrup Cappuccino
    int mgPumpkinSpice = 0;   // Pumpkin Spice Latte
};

class Barista; // forward declaration

class Coffee {
protected:
    Intensity coffeeIntensity;
    string name;

    Coffee(Intensity i, string n)
        : coffeeIntensity(i), name(std::move(n)) {}

    // each derived class defines its specific steps
    virtual void makeSpecific() const = 0;

    void printDetails() const {
        cout << "\nPreparing " << name << '\n'
             << "Setting intensity to " << intensityToString(coffeeIntensity) << endl;
    }
    
    // Main method to make coffee - calls printDetails and class-specific make method
    virtual void makeCoffee() const {
        printDetails();
        makeSpecific();
        cout << "Coffee ready!" << endl;
    }

    friend class Barista; // Barista can use protected members

public:
    virtual ~Coffee() = default;

};

class Americano : public Coffee {
protected:
    int mlOfWater;

    Americano(Intensity i, int w, string n = "Americano")
        : Coffee(i, std::move(n)), mlOfWater(w) {
        if (w < 0)
            throw invalid_argument("mlOfWater must be positive");
        }

    const Americano& makeAmericano() const {
        Coffee::makeCoffee();
        return *this;
    }

    void makeSpecific() const override {
        cout << "Pouring " << mlOfWater << " ml of water" << endl;
    }

    friend class Barista; // only Barista can create Americano
};

class Cappuccino : public Coffee {
protected:
    int mlOfMilk;

    Cappuccino(Intensity i, int m, string n = "Cappuccino")
        : Coffee(i, std::move(n)), mlOfMilk(m) {
            if (m < 0){
                throw invalid_argument("mlOfMilk must be positive");
            }
        }

    const Cappuccino& makeCappuccino() const {
        Coffee::makeCoffee();
        return *this;
    }

    void makeSpecific() const override {
        cout << "Adding " << mlOfMilk << " ml of milk" << endl;
    }

    friend class Barista;
};

class SyrupCappuccino : public Cappuccino {
protected:
    SyrupType syrup;

    SyrupCappuccino(Intensity i, int m, SyrupType s)
        : Cappuccino(i, m, "Syrup Cappuccino"), syrup(s) {}

    const SyrupCappuccino& makeSyrupCappuccino() const {
        Coffee::makeCoffee();
        return *this;
    }

    void makeSpecific() const override {
        Cappuccino::makeSpecific();
        cout << "Mixing in " << syrupTypeToString(syrup) << " syrup" << endl;
    }

    friend class Barista;
};

class PumpkinSpiceLatte : public Cappuccino {
protected:
    int mgPumpkinSpice;

    PumpkinSpiceLatte(Intensity i, int m, int mg)
        : Cappuccino(i, m, "Pumpkin Spice Latte"), mgPumpkinSpice(mg) {
            if (mg < 0){
                throw invalid_argument("mgOfPumpkinSpice must be positive");
            }
        }

    const PumpkinSpiceLatte& makePumpkinSpiceLatte() const {
        Coffee::makeCoffee();
        return *this;
    }

    void makeSpecific() const override {
        Cappuccino::makeSpecific();
        cout << "Sprinkling " << mgPumpkinSpice << " mg of pumpkin spice" << endl;
    }

    friend class Barista;
};

class Barista {
private:
    string name;
    vector<Order> orders;

    unique_ptr<Coffee> create(const Order &o) const {
        switch (o.type) {
            case CoffeeType::AMERICANO:
                // Barista is a friend, so it can call private ctor
                return unique_ptr<Coffee>(new Americano(o.intensity, o.mlOfWater));

            case CoffeeType::CAPPUCCINO:
                return unique_ptr<Coffee>(new Cappuccino(o.intensity, o.mlOfMilk));

            case CoffeeType::SYRUP_CAPPUCCINO:
                return unique_ptr<Coffee>(new SyrupCappuccino(o.intensity, o.mlOfMilk, o.syrup));

            case CoffeeType::PUMPKIN_SPICE_LATTE:
                return unique_ptr<Coffee>(new PumpkinSpiceLatte(o.intensity, o.mlOfMilk, o.mgPumpkinSpice));
        }
        throw runtime_error("Unknown coffee type");
    }

public:
    Barista(string name, vector<Order> orders)
        : name(std::move(name)), orders(std::move(orders)) {}

    void process() {
        cout << "Barista " << name << " is ready to make " << orders.size() << " coffee(s)!" << endl;
        for (const auto &o : orders) {
            auto coffee = create(o);
            switch (o.type) {
                case CoffeeType::AMERICANO:
                    static_cast<Americano*>(coffee.get())->makeAmericano();
                    break;
                case CoffeeType::CAPPUCCINO:
                    static_cast<Cappuccino*>(coffee.get())->makeCappuccino();
                    break;
                case CoffeeType::SYRUP_CAPPUCCINO:
                    static_cast<SyrupCappuccino*>(coffee.get())->makeSyrupCappuccino();
                    break;
                case CoffeeType::PUMPKIN_SPICE_LATTE:
                    static_cast<PumpkinSpiceLatte*>(coffee.get())->makePumpkinSpiceLatte();
                    break;
            }
        }
        cout << "\nAll orders completed!" << endl;
    }
};

int main() {

    vector<Order> orders = {
        {CoffeeType::AMERICANO,           NORMAL, 150},
        {CoffeeType::CAPPUCCINO,          STRONG, 0, 100},
        {CoffeeType::SYRUP_CAPPUCCINO,    LIGHT,  0, 120, VANILLA},
        {CoffeeType::PUMPKIN_SPICE_LATTE, NORMAL, 0, 130, VANILLA, 50}
    };
    Barista b("John", orders);
    b.process();
    return 0;
}
