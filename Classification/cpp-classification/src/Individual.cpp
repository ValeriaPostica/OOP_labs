#include "Individual.h"
#include <iostream>
#include <algorithm>
#include <cctype>

using json = nlohmann::json;

string toUpper(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

// JSON Constructor
Individual::Individual(const json& entry) {
    // Safely parse id: if the key exists and is an integer, use it.
    if (entry.contains("id") && entry["id"].is_number_integer()) {
        id = entry["id"].get<int>();
    } else {
        id = -1;
    }

    // Safely parse isHumanoid: if the key exists and is a boolean, use it.
    // If it's missing or null (or not a boolean), mark as unknown and default to false.
    if (entry.contains("isHumanoid") && entry["isHumanoid"].is_boolean()) {
        isHumanoid = entry["isHumanoid"].get<bool>();
        humanoidKnown = true;
    } else {
        isHumanoid = false;
        humanoidKnown = false;
    }
    
    if (entry.contains("planet") && entry["planet"].is_string()) {
        originPlanet = toUpper(entry["planet"].get<string>());
    } else if (entry.contains("originPlanet") && entry["originPlanet"].is_string()) {
        originPlanet = toUpper(entry["originPlanet"].get<string>());
    } else {
        originPlanet = "";
    }
    
    // Safely parse age: avoid converting null to number
    if (entry.contains("age") && entry["age"].is_number()) {
        age = entry["age"].get<int>();
    } else {
        age = -1;
    }
    
    if (entry.contains("traits") && entry["traits"].is_array()) {
        for (const auto& trait : entry["traits"]) {
            if (trait.is_string()) {
                physicalTraits.push_back(toUpper(trait.get<string>()));
            }
        }
    } else if (entry.contains("physicalTraits") && entry["physicalTraits"].is_array()) {
        for (const auto& trait : entry["physicalTraits"]) {
            if (trait.is_string()) {
                physicalTraits.push_back(toUpper(trait.get<string>()));
            }
        }
    }
}

// Getters
int Individual::getId() const {
    return id;
}

bool Individual::getIsHumanoid() const {
    return isHumanoid;
}

string Individual::getOriginPlanet() const {
    return originPlanet;
}

int Individual::getAge() const {
    return age;
}

vector<string> Individual::getPhysicalTraits() const {
    return physicalTraits;
}

// Helper methods
bool Individual::hasTrait(const string& trait) const {
    string upperTrait = toUpper(trait);
    return find(physicalTraits.begin(), physicalTraits.end(), upperTrait) != physicalTraits.end();
}

bool Individual::hasPlanet() const {
    return !originPlanet.empty();
}

bool Individual::hasAge() const {
    return age != -1;
}

bool Individual::hasHumanoidInfo() const {
    return humanoidKnown;
}

//Rule-Based Classification System
string Individual::classify() const {
        // use existing members: `originPlanet`, `physicalTraits`
        string planet = originPlanet;
        vector<string> traits = physicalTraits;
        auto hasTrait = [&](const string& trait) {
            return find(traits.begin(), traits.end(), trait) != traits.end();
        };

        //Check unambiguous criterias
        // Star Wars
        if (planet == "ENDOR" || planet == "KASHYYYK" || hasTrait("HAIRY")){
            return "star-wars";
        }

        // Marvel
        if (planet == "ASGARD"){
            return "marvel";
        }

        // Hitchhiker's
        if (planet == "BETELGEUSE" || hasTrait("EXTRA_ARMS") || hasTrait("EXTRA_HEAD") || planet == "VOGSPHERE" || hasTrait("GREEN")){
            return "hitch-hiker";
        }

        // Lord of the Rings
        if (getAge() > 5000 || planet == "EARTH" || hasTrait("POINTY_EARS")){
            return "rings";
        }

        // ambiguous - use scoring
        int star_wookie = 0;
        int star_ewok = 0;
        int marvel_asgardian = 0;
        int hitchhiker_betelgeusian = 0;
        int hitchhiker_vogon = 0;
        int rings_elf = 0;
        int rings_dwarf = 0;

        if (humanoidKnown) {
            if (!isHumanoid){
                star_ewok++;
                star_wookie++;
                hitchhiker_vogon++;
            } else {
                marvel_asgardian++;
                hitchhiker_betelgeusian++;
                rings_elf++;
                rings_dwarf++;
            }
        }

        if (hasTrait("TALL")){
            star_wookie++;
            marvel_asgardian++;
        }

        if (hasTrait("SHORT")){
            star_ewok++;
            rings_dwarf++;
        }

        if (hasTrait("BLONDE")){
            marvel_asgardian++;
            rings_elf++;
        }

        if (hasTrait("BULKY")){
            hitchhiker_vogon++;
            rings_dwarf++;
        }

        if (60 < getAge())
            star_ewok--;
        
        if (100 < getAge())
            hitchhiker_betelgeusian--;

        if (200 < getAge()){
            hitchhiker_vogon--;
            rings_dwarf--;
        }

        if (400 < getAge())
            star_wookie--;
        
        if (5000 < getAge())
            marvel_asgardian--;

        int max_count = max({star_wookie, star_ewok, marvel_asgardian, hitchhiker_betelgeusian, hitchhiker_vogon, rings_elf, rings_dwarf});
        vector<int> counts = {star_wookie, star_ewok, marvel_asgardian, hitchhiker_betelgeusian, hitchhiker_vogon, rings_elf, rings_dwarf};
        int ties = count(counts.begin(), counts.end(), max_count);
        if (ties > 1) return "undefined";

        if (max_count == star_wookie || max_count == star_ewok) return "star-wars";
        if (max_count == marvel_asgardian) return "marvel";
        if (max_count == hitchhiker_betelgeusian || max_count == hitchhiker_vogon) return "hitch-hiker";
        if (max_count == rings_elf || max_count == rings_dwarf) return "rings";
        return "undefined";
}