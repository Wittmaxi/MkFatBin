#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <tuple>
#include "settings.hpp"
#include "disc.hpp"

class FATDriver {
    DiscSettings settings;
    RawDisc disc;
//////
    SecOff nextFreeSector;
    ClusOff firstDataCluster;
    SecOff rootSector;
    SecOff FATSector;
//////
    int rootSizeBytes = 0;
//////
    void dumpSectorToFile (int sectorNumber, std::ofstream &file);
    void readBootSector (const std::string& fileName);
    void writeSettingsToBootsector();
    void indexInFAT (ClusOff startSector, int numberSectors);
    void writeFATEntry (int cluster, uint16_t newValue);
    void writeFATData (SecOff position, uint16_t value);
    void writeFAT12Entry (int cluster, uint16_t newValue);
    void writeFAT16Entry (int cluster, uint16_t newValue);
    void writeRootEntry (const std::vector<uint8_t> &fDesc);
    void setupFAT ();
    SecOff calcNextEmptyEntryIndex ();
public:
    FATDriver (DiscSettings);
    void dumpToFile (const std::string &fileName);
    void loadBootSector (const std::string &fileName);
    void addFile (const std::filesystem::directory_entry &fileName);
};
