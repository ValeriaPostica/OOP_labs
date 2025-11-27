#include <iostream>
#include <string>
#include "include/Display.hh"
#include "include/Assistant.hh"

using namespace std;

int main() {
    Display dell(1920, 1080, 93.0f, "Dell FHD");
    Display hp(2560, 1440, 110.0f, "HP QHD");
    Display samsung(3840, 2160, 160.0f, "Samsung 4K");

    Assistant mia("Mia");
    mia.assignDisplay(dell);
    mia.assignDisplay(hp);
    mia.assignDisplay(samsung);

    mia.assist();

    Display to_buy(3840, 2160, 160.0f, "Samsung 4K");
    mia.buyDisplay(to_buy);

    mia.assist();
    return 0;
}
