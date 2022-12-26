#pragma once
#include <vector>
#include <string>
#include <fstream>

class __attribute__((__packed__)) DiscSettings {
public:
    char OEM[8] = { 'M', 'A', 'X', 'I', 'F', 'A', 'T', 0 };
    uint16_t bytesPerSector = 0x200;
    uint8_t sectorsPerCluster = 0x1;
    uint16_t reservedSectors = 0x0;
    uint8_t numberFAT = 0x2; // by default, write two FATs
    uint16_t rootEntries = 0xE0;
    uint16_t totalSectors = 0xB40; // 1.44MB 
    uint8_t mediaDescriptor = 0xF0;
    uint16_t sectorsPerFAT = 0x9;
    uint16_t sectorsPerTrack = 0x12;
    uint16_t headCount = 0x2;
    uint32_t hiddenSectors = 0x0;
    uint32_t largeSectorCount = 0x0;
    // extended BPB
    uint8_t driveNumber = 0x0; // ignore
    uint8_t winNTFlags = 0x0; // ignore
    uint8_t signature = 0x28;
    uint32_t volumeID = 0x0; // ignore
    char volumeLabel[11] = "VOLUMELABL";
    char systemIdentifierString[8] = "FAT12";
};


class Disc {
    std::vector<std::vector<char>> rawSectors;
    DiscSettings settings;
//////
    void dumpSectorToFile (int sectorNumber, std::ofstream &file);
    void readBootSector (const std::string& fileName);
    void writeSettingsToBootsector();
public:
    Disc (DiscSettings);
    void dumpToFile (const std::string &fileName);
    void loadBootSector (const std::string &fileName);
    void addFile (std::string &fileName);
};
