#include "View.h"
#include <fstream>
#include <algorithm>
#include <cctype>

using json = nlohmann::json;

using namespace std;

// helper: convert UPPERCASE/ALLCAPS word to Title case (Asgard, Endor)
static string toTitleCase(const string& s) {
    if (s.empty()) return s;
    string lower;
    lower.reserve(s.size());
    for (char c : s) lower.push_back(tolower(static_cast<unsigned char>(c)));
    lower[0] = toupper(static_cast<unsigned char>(lower[0]));
    return lower;
}

json View::universeToJson(const Universe& u) {
    json out;
    out["name"] = u.getName();
    json arr = json::array();

    for (const auto& entry : u.getIndividuals()) {
        json el;

        if (entry.contains("id") && !entry["id"].is_null()) el["id"] = entry["id"];

        if (entry.contains("isHumanoid") && !entry["isHumanoid"].is_null()) el["isHumanoid"] = entry["isHumanoid"];

        // age
        if (entry.contains("age") && !entry["age"].is_null()) el["age"] = entry["age"];

        // planet: support "planet" or "originPlanet"
        if (entry.contains("planet") && !entry["planet"].is_null()) {
            // convert to Title case for nicer output
            el["planet"] = toTitleCase(entry["planet"].get<string>());
        } else if (entry.contains("originPlanet") && !entry["originPlanet"].is_null()) {
            el["planet"] = toTitleCase(entry["originPlanet"].get<string>());
        }

        // traits: support "traits" or "physicalTraits"
        json traitsArray = json::array();
        if (entry.contains("traits") && entry["traits"].is_array()) {
            for (const auto& t : entry["traits"]) if (!t.is_null()) traitsArray.push_back(t);
        } else if (entry.contains("physicalTraits") && entry["physicalTraits"].is_array()) {
            for (const auto& t : entry["physicalTraits"]) if (!t.is_null()) traitsArray.push_back(t);
        }

        if (!traitsArray.empty()) el["traits"] = traitsArray;

        arr.push_back(el);
    }

    out["individuals"] = arr;
    return out;
}

void View::writeUniverseToFile(const Universe& u, const string& outDir) {
    json out = universeToJson(u);

    // build filename: outDir + name + ".json"
    string filename = outDir;
    if (!filename.empty() && filename.back() != '/' && filename.back() != '\\') filename += '/';
    filename += u.getName();
    filename += ".json";

    ofstream ofs(filename);
    if (!ofs) {
        throw runtime_error("Could not open output file: " + filename);
    }
    ofs << out.dump(4) << endl;
}
