#include <iostream>
#include <vector>
#include "FileReader.h"
#include "Individual.h"
#include "Universe.h"
#include "View.h"

using namespace std;

int main() {
 /* 
    //day2
    cout << "FILE READING AND JSON PARSING" << endl;
    
    try {
        cout << "\nTEST 1: Reading file and printing raw JSON"<<endl;
        FileReader reader("cpp-classification/resources/test-input.json");
        auto jsonData = reader.readFile();
        reader.printJsonToConsole(jsonData);

        cout << "\nTEST 2: Printing each JSON object separately" << endl;
        reader.printEachObjectSeparately(jsonData);

        cout << "\nTEST 3: Exploring JSON structure" << endl;
        if (jsonData.contains("data") && jsonData["data"].is_array()) {
            cout << "Found 'data' array with " << jsonData["data"].size() << " elements" << endl;
            
            //access first element's properties
            if (!jsonData["data"].empty()) {
                auto firstEntry = jsonData["data"][0];
                cout << "First entry ID: " << firstEntry.value("id", -1) << endl;
                cout << "First entry planet: " << firstEntry.value("planet", "unknown") << endl;
            }
        }
        
    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
    
    cout << "\nTESTING COMPLETED SUCCESSFULLY" << endl;

*/



    //day3
 /*   cout << "JSON TO CLASSES MAPPING" << endl;
    
    try {
        FileReader reader("cpp-classification/resources/input.json");
        auto jsonData = reader.readFile();
        auto dataArray = jsonData["data"];
        
        //container for Individual objects
        vector<Individual> individuals;
        
        cout << "\nMAPPING JSON TO INDIVIDUAL OBJECTS" << endl;
        for (const auto& entry : dataArray) {
            individuals.emplace_back(entry);
        }
        
        cout << "Created " << individuals.size() << " Individual objects" << endl;
        
        cout << "\nTEST 1: PRINTING ALL INDIVIDUALS" << endl;
        for (const auto& individual : individuals) {
            individual.printInfo();
        }
        
        cout << "\nTEST 2: PRINTING ONLY IDS" << endl;
        for (const auto& individual : individuals) {
            cout << "ID: " << individual.getId() << endl;
        }
        
        cout << "\nTEST 3: INDIVIDUALS WITH EVEN IDs" << endl;
        for (const auto& individual : individuals) {
            if (individual.getId() % 2 == 0) {
                individual.printInfo();
            }
        }
        
        cout << "\nTEST 4: INDIVIDUALS WITH ODD IDs" << endl;
        for (const auto& individual : individuals) {
            if (individual.getId() % 2 == 1) {
                individual.printInfo();
            }
        }
        
        cout << "\nTEST 5: HUMANOID STATISTICS" << endl;
        int humanoidCount = 0;
        int nonHumanoidCount = 0;
        
        for (const auto& individual : individuals) {
            if (individual.getIsHumanoid()) {
                humanoidCount++;
            } else {
                nonHumanoidCount++;
            }
        }

        cout << "Humanoids: " << humanoidCount << endl;
        cout << "Non-humanoids: " << nonHumanoidCount << endl;
        
        cout << "\nTEST 6: INDIVIDUALS WITH UNKNOWN AGE" << endl;
        for (const auto& individual : individuals) {
            if (individual.getAge() == -1) {
                individual.printInfo();
            }
        }
        
    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }
    
    cout << "\nMAPPING AND EXPERIMENTATION COMPLETE" << endl; */





   //day4 and 5
    // Create universes (declare outside try so they remain in scope for output)
    Universe starWars("star-wars");
    Universe hitchhikers("hitch-hiker");
    Universe marvel("marvel");
    Universe rings("rings");
    Universe undefined("undefined");

    try {
        FileReader reader("cpp-classification/resources/input_full.json");
        auto jsonData = reader.readFile();
        
        json dataArray;
        if (jsonData.is_array()) {
            dataArray = jsonData;  
        } else if (jsonData.contains("data") && jsonData["data"].is_array()) {
            dataArray = jsonData["data"];  
        } else {
            throw std::runtime_error("Invalid JSON structure: expected array or object with 'data' array");
        }
        
        
        vector<Individual> individuals;
        
        cout << "\nCLASSIFICATION RESULTS" << endl;
        
        for (const auto& entry : dataArray) {
            Individual individual(entry);
            individuals.push_back(individual);
            
            string classification = individual.classify();
            
            // Add to universe
            if (classification == "star-wars") {
                starWars.addIndividual(entry);
            } else if (classification == "hitch-hiker") {
                hitchhikers.addIndividual(entry);
            } else if (classification == "marvel") {
                marvel.addIndividual(entry);
            } else if (classification == "rings") {
                rings.addIndividual(entry);
            } else {
                undefined.addIndividual(entry);
            }
        }
        
        cout << "Star Wars:       " << starWars.getCount() << " individuals" << endl;
        cout << "Hitchhiker's:    " << hitchhikers.getCount() << " individuals" << endl;
        cout << "Marvel:          " << marvel.getCount() << " individuals" << endl;
        cout << "Lord of the Rings: " << rings.getCount() << " individuals" << endl;
        cout << "Undefined:       " << undefined.getCount() << " individuals" << endl;
        
    } catch (const exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    }

        // Write universes to output files
        try {
            View::writeUniverseToFile(starWars);
            View::writeUniverseToFile(hitchhikers);
            View::writeUniverseToFile(marvel);
            View::writeUniverseToFile(rings);
            View::writeUniverseToFile(undefined);
            cout << "\nOutput files written to 'output/'" << endl;
        } catch (const exception& e) {
            cerr << "ERROR writing output files: " << e.what() << endl;
            return 1;
        }

    return 0;
}