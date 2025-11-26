#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <iostream>
#include <string>

using namespace std;

class Display {
private:
    int width;
    int height;
    float ppi;
    string model;

public:
    Display()
        : width(0), height(0), ppi(0.0f), model("Unknown") {}

    Display(int w, int h, float p, string m)
        : width(w), height(h), ppi(p), model(m) {}

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    float getPPI() const { return ppi; }
    string getModel() const { return model; }

    // Compare only by total pixel area
    void compareSize(const Display& other) const {
        long long areaA = width * height;
        long long areaB = other.width * other.height;

        cout << "[Size] " << model << " (" << width << "x" << height << ") vs "
             << other.model << " (" << other.width << "x" << other.height << "): ";

        if (areaA == areaB) {
            cout << "same surface area.\n";
        } else if (areaA > areaB) {
            cout << model << " is larger.\n";
        } else {
            cout << other.model << " is larger.\n";
        }
    }

    // Compare by PPI
    void compareSharpness(const Display& other) const {
        cout << "[Sharpness] " << model << " (" << ppi << " ppi) vs "
             << other.model << " (" << other.ppi << " ppi): ";

        if (ppi == other.ppi) {
            cout << "same sharpness.\n";
        } else if (ppi > other.ppi) {
            cout << model << " is sharper.\n";
        } else {
            cout << other.model << " is sharper.\n";
        }
    }

    void compareWithMonitor(const Display& other) const {
        cout << "Comparing \"" << model << "\" with \"" << other.model << "\" \n";
        compareSize(other);
        compareSharpness(other);
        cout << endl;
    }
};

#endif // DISPLAY_HPP
