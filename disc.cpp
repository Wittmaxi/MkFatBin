#include "disc.hpp"

#include <iostream>

Disc::Disc (DiscSettings set) {
    settings = set;
}

void padWithNulls (int amount, std::ofstream &file) {
    for (int i = 0; i < amount; i++) {
        file << 0;
    }
}

int writeExistingSector (std::vector<char> &sec, int bytesPerSector, std::ofstream &file) {
    bool isSectorTooBig = sec.size() > bytesPerSector;
    if (isSectorTooBig)
        sec.resize(bytesPerSector);

    for (int i = 0; i < sec.size(); i++)
        file << sec[i];

    return bytesPerSector - sec.size();
}

void Disc::dumpSectorToFile (int sectorNumber, std::ofstream &file) {
    int missingBytes = settings.bytesPerSector;
    bool doesSectorExist = rawSectors.size() > sectorNumber;

    if (doesSectorExist)
        missingBytes = writeExistingSector (rawSectors[sectorNumber], settings.bytesPerSector, file);

    padWithNulls (missingBytes, file);
}

void Disc::dumpToFile (const std::string &fileName) {
    std::ofstream file (fileName);

    for (int i = 0; i < settings.totalSectors; i++)
        dumpSectorToFile (i, file);
    file.flush();
}

void Disc::loadBootSector (const std::string &fileName) {
    //boot sector is ALWAYS sector 0 (sector 1 in CHL addressing)
    
}
