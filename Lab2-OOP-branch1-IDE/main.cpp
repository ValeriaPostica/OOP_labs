#include <iostream>
#include <string>
#include "terminal_ide.hpp"

int main() {
    try {
        TerminalIDE ide;
        ide.loop();
    } catch (const std::exception& e) {
        std::cerr << "Fatal: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
