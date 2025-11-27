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

    unique_ptr<Coffee> create(const Order &o) const {
        switch (o.type) {
            case CoffeeType::AMERICANO:
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
    explicit Barista(string n) : name(std::move(n)) {}

    const string & getName() const { return name; }

    void make(const Order &o) const {
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
};

void validateOrder(const Order &o) {
    string msg;
    auto add = [&](const string &part){ if(!msg.empty()) msg += "; "; msg += part; };
    switch(o.type) {
        case CoffeeType::AMERICANO:
            if(o.mlOfMilk>0) add("milk for Americano");
            if(o.mgPumpkinSpice>0) add("pumpkin spice in Americano");
            if(o.syrup!=VANILLA) add("syrup in Americano");
            break;
        case CoffeeType::CAPPUCCINO:
            if(o.mlOfWater>0) add("extra water for Cappuccino");
            if(o.mgPumpkinSpice>0) add("pumpkin spice in plain Cappuccino");
            if(o.syrup!=VANILLA) add("syrup without Syrup Cappuccino type");
            break;
        case CoffeeType::SYRUP_CAPPUCCINO:
            if(o.mlOfWater>0) add("water in Syrup Cappuccino");
            if(o.mgPumpkinSpice>0) add("pumpkin spice in Syrup Cappuccino");
            break;
        case CoffeeType::PUMPKIN_SPICE_LATTE:
            if(o.mlOfWater>0) add("water in Pumpkin Spice Latte");
            if(o.mgPumpkinSpice<=0) add("missing pumpkin spice");
            break;
    }
    if(!msg.empty()) {
        cout << "[Warning] Barista is confused of this order but will do this because the client is always right and will add " << msg << endl;
    }

    if(o.mlOfWater > 250) {
        cout << "[Bonus] Barista whispers: This much water? Are we brewing a koi pond?" << endl;
    }
    if(o.mgPumpkinSpice > 150) {
        cout << "[Bonus] Barista whispers: That's enough pumpkin spice to summon autumn itself!" << endl;
    }
}

class BaristaPool {
private:
    vector<Barista> baristas;
    int currentIndex = 0;
    int madeByCurrent = 0;
    static constexpr int MaxPerBarista = 3;

    void rotate() {
        int lastIndex = currentIndex;
        currentIndex = (currentIndex + 1) % baristas.size();
        madeByCurrent = 0;
        cout << "\nBarista " << baristas[lastIndex].getName() << " is tired of this, switching to barista: " << baristas[currentIndex].getName() << endl;
    }
public:
    explicit BaristaPool(vector<string> names) {
        for(auto &n: names) baristas.emplace_back(n);
        if(baristas.empty()) throw runtime_error("No baristas available");
        cout << "Starting with barista: " << baristas[0].getName() << endl;
    }

    void process(const vector<Order>& orders) {
        cout << "Processing " << orders.size() << " orders with " << baristas.size() << " barista(s)." << endl;
        for(const auto &o : orders) {
            if(madeByCurrent == MaxPerBarista) {
                rotate();
            }
            cout << "\nBarista " << baristas[currentIndex].getName() << " handling order #" << (madeByCurrent+1) << " of 3" << endl;
            validateOrder(o);
            baristas[currentIndex].make(o);
            ++madeByCurrent;
        }
        cout << "\nAll orders completed!" << endl;
    }
};

int main() {
    vector<Order> orders = {
        {CoffeeType::AMERICANO,           NORMAL, 150},
        {CoffeeType::CAPPUCCINO,          STRONG, 20, 100},            // water will trigger warning
        {CoffeeType::SYRUP_CAPPUCCINO,    LIGHT,  0, 120, CARAMEL},    // caramel syrup OK
        {CoffeeType::PUMPKIN_SPICE_LATTE, NORMAL, 0, 130, VANILLA, 50},
        {CoffeeType::AMERICANO,           STRONG, 260, 10},            // milk triggers warning and water bonus
        {CoffeeType::CAPPUCCINO,          LIGHT,  0, 90, VANILLA, 5},  // spice + syrup warning
        {CoffeeType::PUMPKIN_SPICE_LATTE, NORMAL, 0, 140, VANILLA, 155}  // spice bonus
    };

    vector<string> baristaNames = {"Alice", "Bob", "Charlie"};
    BaristaPool pool(baristaNames);
    pool.process(orders);
    return 0;
}
