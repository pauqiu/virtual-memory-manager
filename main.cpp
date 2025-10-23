#include <iostream>

#include "memoryManager.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <argument>" << std::endl;
        return 1;
    }

    std::string adresses = argv[1];
    
    memoryManager manager;
    manager.readAddress(adresses);

    return 0;
}