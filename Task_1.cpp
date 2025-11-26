#include <iostream>
#include <string>
#include "include/Display.hh"

using namespace std;

int main() {
    Display a(1920, 1080, 92.0f, "ViewSonic VX");
    Display b(2560, 1440, 109.0f, "HP QHD");
    Display c(3840, 2160, 160.0f, "Samsung 4K");

    cout << "Pairwise checks\n";
    a.compareSize(b);
    a.compareSharpness(b);
    b.compareSize(c);
    b.compareSharpness(c);

    cout << "\nMixed comparison\n";
    a.compareWithMonitor(c);
    b.compareWithMonitor(c);
    a.compareWithMonitor(b);

    return 0;
}
