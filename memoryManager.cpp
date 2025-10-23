#include "memoryManager.h"

#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>

memoryManager::memoryManager() 
{
    // initialize page table
    pageTableEntry init = {-1, false};
    pageTable = std::vector<pageTableEntry>(256, init);

    simulateData();
}

void memoryManager::simulateData()
{
    std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(32, 126);

    // adding random values to the secondary storage
    for(int i = 0; i < PHYSICAL_MEMORY_SIZE; i++) {
        backingStore[i] = static_cast<char>(dist(rng));
    }
}

void memoryManager::readAddress(std::string filePath)
{
    std::ifstream addressFile(filePath);

    if (!addressFile.is_open()) {
        std::cerr << "Could not open addresses file!" << std::endl;
        return;
    }

    std::string address;
    while (std::getline(addressFile, address)) {
        getValues(address);
    }

    addressFile.close();
}

/**
 * Offset: 0-7
 * Page number: 8-15
 * Bits, not bytes!
 */

void memoryManager::getValues(std::string address) 
{
    int addressNum = 0;
    try {
        addressNum = std::stoi(address);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument" << e.what() << std::endl;
    }

    // isolate the lower 16 bits
    int maskedAddress = addressNum & 0xFFFF;

    // upper 8 bits
    int pageNumber = (maskedAddress >> 8) & 0xFF;

    // lower 8 bits
    int offset = maskedAddress & 0xFF;

    std::cout << "----Logical address----" << std::endl;
    std::cout << "Page number: " << pageNumber << " Offset: " 
            << offset << std::endl;

    obtainFrame(offset, pageNumber);
}

void memoryManager::obtainFrame(int offset, int pageNumber)
{
    int frame = -1;
    if (pageTable[pageNumber].valid) {
        frame = pageTable[pageNumber].frameNumber;
    } else {
        std::cout << "Page fault!" << std::endl;
        frame = pageFault(pageNumber);
        if (frame == -1) {
            std::cout << "There aren't any free frames!" << std::endl;
            // TODO: Implement frame eviction
        } 
    }

    std::cout << "----Physical address----" << std::endl;
    std::cout << "Frame number: " << frame << " Offset: " 
            << offset << std::endl;
    std::cout << "Data: " << physicalMemory[frame] << std::endl;
}

int memoryManager::pageFault(int pageNumber)
{
    for (int frame = 0; frame < PHYSICAL_MEMORY_SIZE; frame++) {
        if (physicalMemory[frame] == '\0') {
            std::cout << "Free frame found: " << frame << std::endl;
            physicalMemory[frame] = backingStore[pageNumber];

            pageTableEntry entry = { frame, true };
            pageTable[pageNumber] = entry;
            return frame;
        }
    }

    return -1;
}