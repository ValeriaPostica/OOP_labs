#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <cassert>
#include "Scheduler.hpp"
#include "../task4/Semaphore.hpp"
#include "../task2/DineableRefuelable.hpp"
#include "nlohmann/json.hpp"

using namespace std;
namespace fs = std::filesystem;

void runGenerator(const fs::path& binDir) {
    fs::path genPath = binDir / "generator.exe";

    if (fs::exists(genPath)) {
        cout << "[System] Launching Generator..." << endl;
        string cmd = "\"" + genPath.string() + "\"";
        cout << "[System] Command: " << cmd << endl;
        std::system(cmd.c_str()); //Launches generator.exe
    } else {
        cerr << "[System] Error: Could not find generator executable at " << genPath << endl;
    }
}

int main(int argc, char** argv) {
    // Determine where we are running from to find the generator
    fs::path selfPath = fs::absolute(argv[0]);
    fs::path binDir = selfPath.parent_path();

    // Launch Generator in a background thread
    thread generatorThread([binDir]() {
        runGenerator(binDir);
    });
    generatorThread.detach(); // Allow it to run independently

    cout << "Car Service Application (Async)" << endl;
    cout << "Waiting for generator to produce cars in 'queue' folder..." << endl;

    // 1. Initialize the Semaphore (The Controller)
    Semaphore semaphore;

    // 2. Initialize the Scheduler
    Scheduler scheduler(semaphore, (binDir / "queue").string());

    // 3. Start the background tasks
    // Scan every 300ms, Serve every 2 second
    scheduler.start(300, 2000);

    // 4. Main loop: Monitor for completion
    while (true) {
        if (scheduler.isFinished()) {
            cout << "\nFinish signal detected!" << endl;
            
            // Give a moment for final processing
            this_thread::sleep_for(chrono::seconds(2));
            
            // One final serve to ensure queue is empty
            semaphore.serveAll();
            
            break;
        }
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    // 5. Stop scheduler
    scheduler.stop();

    // 6. Print Final Statistics
    cout << "\nFinal Statistics" << endl;
    cout << "Electric Cars Served: " << semaphore.getElectric() << endl;
    cout << "Gas Cars Served:      " << semaphore.getGas() << endl;
    cout << "People Served:        " << semaphore.getPeople() << endl;
    cout << "Robots Served:        " << semaphore.getRobots() << endl;

    // Verify against finish.json
    try {
        fs::path finishPath = binDir / "queue" / "finish.json";
        std::ifstream in(finishPath);
        if (!in) {
            std::cerr << "[Verify] Could not open finish.json at " << finishPath << std::endl;
        } else {
            nlohmann::json j; in >> j;
            int expectElectric = j.value("ELECTRIC", 0);
            int expectGas      = j.value("GAS", 0);
            int expectPeople   = j.value("PEOPLE", 0);
            int expectRobots   = j.value("ROBOTS", 0);

            assert(semaphore.getElectric() == expectElectric);
            cout << "[Verify] Expected ELECTRIC: " << expectElectric << ", Got: " << semaphore.getElectric() << std::endl;

            assert(semaphore.getGas() == expectGas);
            cout << "[Verify] Expected GAS: " << expectGas << ", Got: " << semaphore.getGas() << std::endl;

            assert(semaphore.getPeople() == expectPeople);
            cout << "[Verify] Expected PEOPLE: " << expectPeople << ", Got: " << semaphore.getPeople() << std::endl;
            
            assert(semaphore.getRobots() == expectRobots);
            cout << "[Verify] Expected ROBOTS: " << expectRobots << ", Got: " << semaphore.getRobots() << std::endl;

        }
    } catch (const std::exception& e) {
        std::cerr << "[Verify] Error reading/verifying finish.json: " << e.what() << std::endl;
    }

    return 0;
}


/* cmake --build cmake-build-debug --target all_tasks
.\cmake-build-debug\task1.exe
.\cmake-build-debug\task2.exe
.\cmake-build-debug\task3.exe
.\cmake-build-debug\task4.exe
.\cmake-build-debug\task5.exe
*/