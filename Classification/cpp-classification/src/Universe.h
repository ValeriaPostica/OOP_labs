#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <string>
#include <vector>
#include "../include/nlohmann/json.hpp"

using namespace std;

class Universe {
private:
    string name;
    vector<nlohmann::json> individuals;

public:
    Universe(const string& name);
    void addIndividual(const nlohmann::json& individual);
    vector<nlohmann::json> getIndividuals() const;
    string getName() const;
    int getCount() const;
};

#endif