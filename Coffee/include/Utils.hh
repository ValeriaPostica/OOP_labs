#ifndef UTILS_HH
#define UTILS_HH

#include <string>
#include <stdexcept>
using namespace std;

enum Intensity {
    LIGHT,
    NORMAL,
    STRONG
};

enum SyrupType {
    MACADAMIA, 
    VANILLA,
    COCONUT,
    CARAMEL,
    CHOCOLATE,
    POPCORN
};

inline string intensityToString(Intensity intensity) {
    switch (intensity) {
        case LIGHT:
            return "Light";
        case NORMAL:
            return "Normal";
        case STRONG:
            return "Strong";
        default:
            return "Unknown";
    }
}

inline Intensity stringToIntensity(const string& str) {
    if (str == "Light") {
        return LIGHT;
    } else if (str == "Normal") {
        return NORMAL;
    } else if (str == "Strong") {
        return STRONG;
    } else {
        throw invalid_argument("Invalid intensity string");
    }
}

inline string syrupTypeToString(SyrupType syrup) {
    switch (syrup) {
        case MACADAMIA:
            return "Macadamia";
        case VANILLA:
            return "Vanilla";
        case COCONUT:
            return "Coconut";
        case CARAMEL:
            return "Caramel";
        case CHOCOLATE:
            return "Chocolate";
        case POPCORN:
            return "Popcorn";
        default:
            return "Unknown";
    }
}

inline SyrupType stringToSyrupType(const string& str) {
    if (str == "Macadamia") {
        return MACADAMIA;
    } else if (str == "Vanilla") {
        return VANILLA;
    } else if (str == "Coconut") {
        return COCONUT;
    } else if (str == "Caramel") {
        return CARAMEL;
    } else if (str == "Chocolate") {
        return CHOCOLATE;
    } else if (str == "Popcorn") {
        return POPCORN;
    } else {
        throw invalid_argument("Invalid syrup type string");
    }
}

#endif