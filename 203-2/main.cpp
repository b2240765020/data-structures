#include "QuakeAssistController.h"
#include <iostream>
#include <fstream>
#include <string>

// This main file is provided for you.
// It reads commands from the given input file and forwards them
// to the QuakeAssistController. You do NOT need to modify main.cpp.

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    QuakeAssistController controller;
    std::string line;
    while (std::getline(std::cin, line)) {
        controller.parseAndExecute(line);
    }
    return 0;
}
