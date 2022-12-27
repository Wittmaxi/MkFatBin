#include "disc.hpp"
#include <iostream>
#include <cstring>

Disc::Disc (DiscSettings set) {
    settings = set;
    rawSectors.push_back (std::vector <char> (0)); // always have at least a bootsector
    writeSettingsToBootsector();
    FATsector = set.reservedSectors + set.hiddenSectors;
    rootSector = FATsector + set.numberFAT * set.sectorsPerFAT;
    rootSizeBytes = 32 * set.rootEntries;
    nextFreeSector = rootSector + rootSizeBytes / set.bytesPerSector;

    for (int i = 0; i < nextFreeSector; i++)
        rawSectors.push_back (std::vector<char> (set.bytesPerSector)); // make sure that atleast the sys-segments are filled. TODO remove because inefficient
}

inline void padWithNulls (int amount, std::ofstream &file) {
    for (int i = 0; i < amount; i++)
        file << '\0';
}

inline int writeExistingSector (std::vector<char> &sec, int bytesPerSector, std::ofstream &file) {
    bool isSectorTooBig = sec.size() > bytesPerSector;
    if (isSectorTooBig)
        sec.resize(bytesPerSector);

    for (int i = 0; i < sec.size(); i++)
        file << sec[i];

    return bytesPerSector - sec.size();
}

inline void Disc::dumpSectorToFile (int sectorNumber, std::ofstream &file) {
    int missingBytes = writeExistingSector (rawSectors[sectorNumber], settings.bytesPerSector, file);
    padWithNulls (missingBytes, file);
}

void Disc::dumpToFile (const std::string &fileName) {
    std::ofstream file (fileName);

    // assume that we never cached too many sectors - that check 
    // happens in addFile
    for (int i = 0; i < rawSectors.size(); i++)
        dumpSectorToFile (i, file);

    int missingBytes = (settings.totalSectors - rawSectors.size()) * settings.bytesPerSector;
    padWithNulls (missingBytes, file);

    file.flush();
}

inline void Disc::readBootSector (const std::string &fileName) {
    std::vector<char> &bs = rawSectors [0];

    bs.resize(settings.bytesPerSector); 
    std::ifstream file (fileName);
    file.read (bs.data(), settings.bytesPerSector); // dirty trick so I don't need to buffer stuff in between
}

inline void Disc::writeSettingsToBootsector () {
    constexpr int bpbSize = sizeof (DiscSettings);
    if (rawSectors[0].size() < bpbSize + 3)
        rawSectors[0].resize(bpbSize + 3); // make sure that we have enough space

    std::memcpy (rawSectors [0].data() + 3, reinterpret_cast <char*>(&settings), bpbSize);
}

void Disc::loadBootSector (const std::string &fileName) {
    //boot sector is ALWAYS sector 0 (sector 1 in CHL addressing)
    readBootSector (fileName);
    writeSettingsToBootsector();
}
