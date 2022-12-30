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
    int nextFreeSector;
//////
    int FATsector = 0;
    int rootSector = 0;
    int rootSizeBytes = 0;
//////
    void dumpSectorToFile (int sectorNumber, std::ofstream &file);
    void readBootSector (const std::string& fileName);
    void writeSettingsToBootsector();
    void indexInFAT (int startSector, int numberSectors);
    void writeRootEntry (const std::vector<uint8_t> &fDesc);
    std::tuple<int, int> calcNextEmptyEntryIndex ();
public:
    FATDriver (DiscSettings);
    void dumpToFile (const std::string &fileName);
    void loadBootSector (const std::string &fileName);
    void addFile (const std::filesystem::directory_entry &fileName);
};
