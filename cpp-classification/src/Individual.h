#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <string>
#include <vector>
#include "../include/nlohmann/json.hpp"

using namespace std;

class Individual {
private:
    int id;
    bool isHumanoid;
    bool humanoidKnown;
    string originPlanet;
    int age;
    vector<string> physicalTraits;

public:
    Individual(const nlohmann::json& entry);
    
    // Getters
    int getId() const;
    bool getIsHumanoid() const;
    string getOriginPlanet() const;
    int getAge() const;
    vector<string> getPhysicalTraits() const;
    
    // Helper methods
    bool hasTrait(const string& trait) const;
    bool hasPlanet() const;
    bool hasAge() const;
    bool hasHumanoidInfo() const;
    
    string classify() const;
    
    void printInfo() const;
};

#endif