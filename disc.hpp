#pragma once
#include <vector>
#include <string>
#include <fstream>

class DiscSettings {
public:
    char OEM[8] = { 'M', 'A', 'X', 'W', 'I', 'T', 'T', 0 };
    int bytesPerSector = 0x200;
    int sectorsPerCluster = 0x1;
    int reservedSectors = 0x0;
    int numberFAT = 0x2; // by default, write two FATs
    int rootEntries = 0xE0;
    int totalSectors = 0xB40; // 1.44MB 
    int mediaDescriptor = 0xF0;
    int sectorsPerFAT = 0x9;
    int sectorsPerTrack = 0x12;
    int headCount = 0x2;
    int hiddenSectors = 0x0;
};

class Disc {
    std::vector<std::vector<char>> rawSectors;
    DiscSettings settings;
    
public:
    Disc (DiscSettings);
    void dumpToFile (const std::string &fileName);
    void dumpSectorToFile (int sectorNumber, std::ofstream &file);
    void loadBootSector (const std::string &fileName);
    void addFile (std::string &);
};
