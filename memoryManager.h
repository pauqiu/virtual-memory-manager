#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <string>
#include <vector>

// ---- MACROS ----
#define PAGE_SIZE 256
#define PHYSICAL_MEMORY_SIZE 65536

struct data {
    char values[PAGE_SIZE] = {'\0'};
};

struct pageTableEntry {
    int frameNumber;
    bool valid;
};

class memoryManager {

    public:
        memoryManager();
        ~memoryManager() = default;
        void simulateData();
        void readAddress(std::string filePath);
        void getValues(std::string address);
        void obtainFrame(int offset, int pageNumber);
        int pageFault(int pageNumber);
        void outPut(int offset, int pageNumber);

    private:
        std::vector<pageTableEntry> pageTable;
        char physicalMemory[PHYSICAL_MEMORY_SIZE] = {'\0'};
        char backingStore[PHYSICAL_MEMORY_SIZE] = {'\0'};
};

#endif // MEMORYMANAGER_H