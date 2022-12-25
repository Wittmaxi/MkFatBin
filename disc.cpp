#include "disc.hpp"
#include <iostream>
#include <cstring>

Disc::Disc (DiscSettings set) {
    settings = set;
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

    std::cout << "HI" << std::endl;
    int missingBytes = (settings.totalSectors - rawSectors.size()) * settings.bytesPerSector;
    std::cout << rawSectors.size() << std::endl;
    std::cout << settings.totalSectors;
    std::cout << missingBytes << std::endl;
    padWithNulls (missingBytes, file);

    std::cout << "HI---" << std::endl;
    file.flush();
    std::cout << "HI---" << std::endl;
}

inline void Disc::readBootSector (const std::string &fileName) {
    if (rawSectors.size() == 0)
        rawSectors.push_back (std::vector <char> (0));
    std::vector<char> &bs = rawSectors [0];

    bs.resize(settings.bytesPerSector); 
    std::ifstream file (fileName);
    file.read (bs.data(), settings.bytesPerSector); // dirty trick so I don't need to buffer stuff in between
}

inline void Disc::writeSettingsToBootsector () {
    std::memcpy (rawSectors [0].data() + 3, &settings.OEM, 8);
    std::memcpy (rawSectors [0].data() + 11, reinterpret_cast <char*>(&settings.bytesPerSector), 2);
    std::memcpy (rawSectors [0].data() + 13, reinterpret_cast <char*>(&settings.sectorsPerCluster), 1);
    std::memcpy (rawSectors [0].data() + 14, reinterpret_cast <char*>(&settings.reservedSectors), 2);
    std::memcpy (rawSectors [0].data() + 16, reinterpret_cast <char*>(&settings.numberFAT), 1);
    std::memcpy (rawSectors [0].data() + 17, reinterpret_cast <char*>(&settings.rootEntries), 2);
    std::memcpy (rawSectors [0].data() + 19, reinterpret_cast <char*>(&settings.totalSectors), 2);
    std::memcpy (rawSectors [0].data() + 21, reinterpret_cast <char*>(&settings.mediaDescriptor), 1);
    std::memcpy (rawSectors [0].data() + 22, reinterpret_cast <char*>(&settings.sectorsPerFAT), 2);
    std::memcpy (rawSectors [0].data() + 24, reinterpret_cast <char*>(&settings.sectorsPerTrack), 2);
    std::memcpy (rawSectors [0].data() + 26, reinterpret_cast <char*>(&settings.headCount), 2);
    std::memcpy (rawSectors [0].data() + 28, reinterpret_cast <char*>(&settings.hiddenSectors), 4);
}

void Disc::loadBootSector (const std::string &fileName) {
    //boot sector is ALWAYS sector 0 (sector 1 in CHL addressing)
    readBootSector (fileName);
    writeSettingsToBootsector();
}
