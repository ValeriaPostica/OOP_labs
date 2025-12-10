#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>
#include <set>
#include <regex>
#include <atomic>
#include <fstream>
#include "nlohmann/json.hpp"
#include "../task4/Semaphore.hpp"
#include "../task3/Car.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

class Scheduler {
private:
    Semaphore& semaphore;
    string queueDir;
    atomic<bool> running;
    set<int> processedIds;
    
    // Threads
    thread scannerThread;
    thread serverThread;

public:
    Scheduler(Semaphore& sem, const string& dir = "queue") 
        : semaphore(sem), queueDir(dir), running(false) {}

    ~Scheduler() {
        stop();
    }

    void start(int scanIntervalMs, int serveIntervalMs) {
        if (running) return;
        running = true;

        // 1. Scanner Task (Producer)
        scannerThread = thread([this, scanIntervalMs]() {
            regex carRegex(R"(^Car(\d+)\.json$)");
            
            while (running) {
                if (fs::exists(queueDir)) {
                    for (const auto& entry : fs::directory_iterator(queueDir)) {
                        if (!entry.is_regular_file()) continue;
                        
                        string name = entry.path().filename().string();
                        smatch m;
                        if (regex_match(name, m, carRegex)) {
                            int id = stoi(m[1].str());
                            
                            // Skip if already processed
                            if (processedIds.count(id)) continue;

                            // Try to read and parse
                            try {
                                ifstream f(entry.path());
                                json j;
                                f >> j;
                                
                                int carId = j.at("id").get<int>();
                                string type = j.at("type").get<string>();
                                string passengers = j.at("passengers").get<string>();
                                bool isDining = j.at("isDining").get<bool>();
                                int consumption = j.at("consumption").get<int>();

                                Car c = Car::createFromStrings(carId, type, passengers, isDining, consumption);
                                
                                // Guide the car to the correct station
                                semaphore.guideCar(c);
                                processedIds.insert(id);
                                // cout << "[Scanner] Added Car " << id << endl;
                            } catch (...) {
                                // File might be locked or incomplete, skip for now
                            }
                        }
                    }
                }
                this_thread::sleep_for(chrono::milliseconds(scanIntervalMs));
            }
        });

        // 2. Server Task (Consumer)
        serverThread = thread([this, serveIntervalMs]() {
            while (running) {
                // cout << "[Server] Serving cars..." << endl;
                semaphore.serveAll();
                this_thread::sleep_for(chrono::milliseconds(serveIntervalMs));
            }
        });
    }

    void stop() {
        running = false;
        if (scannerThread.joinable()) scannerThread.join();
        if (serverThread.joinable()) serverThread.join();
    }

    bool isFinished() {
        return fs::exists(fs::path(queueDir) / "finish.json");
    }
};

#endif // SCHEDULER_HPP
