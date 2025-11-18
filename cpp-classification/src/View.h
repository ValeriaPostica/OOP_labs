#ifndef VIEW_H
#define VIEW_H

#include <string>
#include "Universe.h"
#include "../include/nlohmann/json.hpp"

using json = nlohmann::json;

class View {
public:
    // Convert a Universe to the expected output JSON structure
    static json universeToJson(const Universe& u);

    // Write the Universe JSON into a file under `outDir` (default: "output/")
    static void writeUniverseToFile(const Universe& u, const std::string& outDir = "output/");
};

#endif
