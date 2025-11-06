#ifndef ASSISTANT_HH
#define ASSISTANT_HH

#include <iostream>
#include <string>
#include <vector>
#include "Display.hpp"

using namespace std;

class Assistant {
private:
    string assistantName;
    vector<Display> assignedDisplays;

    static bool sameModel(const Display& a, const Display& b) {
        return a.getModel() == b.getModel();
    }

public:
    Assistant(string name) : assistantName(name) {}

    string getName() const { return assistantName; }
    void setName(string name) { assistantName = name; }

    void assignDisplay(const Display& d) {
        assignedDisplays.push_back(d);
    }

    // Compare each display with the next one in the list (sequential)
    void assist() const {
        if (assignedDisplays.empty()) {
            cout << "Assistant " << assistantName << ": no displays assigned.\n\n";
            return;
        }
        cout << "Assistant " << assistantName << " is analyzing " << assignedDisplays.size() << " displays...\n\n";
        for (size_t i = 0; i + 1 < assignedDisplays.size(); ++i) {
            assignedDisplays[i].compareWithMonitor(assignedDisplays[i + 1]);
        }
    }

    // "Buy" (remove) by model equality; return removed or a default "Unknown"
    Display buyDisplay(const Display& d) {
        for (size_t i = 0; i < assignedDisplays.size(); ++i) {
            if (sameModel(assignedDisplays[i], d)) {
                Display chosen = assignedDisplays[i];
                assignedDisplays.erase(assignedDisplays.begin() + (long)i);
                cout << "You bought: " << chosen.getModel() << "\n\n";
                return chosen;
            }
        }
        cout << "Display not found: " << d.getModel() << "\n\n";
        return Display();
    }
};

#endif // ASSISTANT_HH
