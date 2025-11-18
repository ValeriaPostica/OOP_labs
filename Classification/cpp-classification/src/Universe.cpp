#include "Universe.h"

Universe::Universe(const string& name) : name(name) {}

void Universe::addIndividual(const nlohmann::json& individual) {
    individuals.push_back(individual);
}

vector<nlohmann::json> Universe::getIndividuals() const {
    return individuals;
}

string Universe::getName() const {
    return name;
}

int Universe::getCount() const {
    return individuals.size();
}