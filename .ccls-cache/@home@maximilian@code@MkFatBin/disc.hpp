/* Copyright 2023 Maximilian Wittmer */
#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "settings.hpp"
#include "addressing.hpp"

typedef std::vector<uint8_t> Sector;

class RawDisc {
    std::vector<Sector> sectors; // Don't save contiguous memory to save RAM
    DiscSettings settings;

    inline void dumpSectorToFile (SecOff sector, std::ofstream &file);
    inline int dumpExistingSectorBytes (SecOff sector, std::ofstream &file);
public:
    RawDisc (DiscSettings& set);
    void writeFileToContiguousSectors (SecOff startSector, const std::filesystem::directory_entry &file);
    void writeArrayToContiguousSectors (SecOff startSector, const std::vector<uint8_t> &data);
    void writeFileToSector (SecOff sector, std::ifstream &file);
    void dumpToFile (const std::string &fileName);
    void enforceCorrectSecSize (SecOff sector);
    void ensureSecExists (SecOff sector);
    Sector& getSectorAt (SecOff sector);
};
