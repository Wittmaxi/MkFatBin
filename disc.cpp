#include "disc.hpp"

RawDisc::RawDisc (DiscSettings &set) : settings(set) {
    ensureSecExists ({0, 0, settings});
};

inline void padWithNulls (int amount, std::ofstream &file) {
    for (int i = 0; i < amount; i++)
        file << '\0';
}

int RawDisc::dumpExistingSectorBytes (SecOff sector, std::ofstream &file) {
    std::vector <uint8_t>& sec = getSectorAt (sector);

    int i = sector.offset;
    for (; i < sec.size(); i++)  {
        file << sec[i];
        std::cout << sec [i];
    }

    return settings.bytesPerSector - i;
}

void RawDisc::dumpSectorToFile (SecOff sector, std::ofstream &file) {
    int missingBytes = dumpExistingSectorBytes (sector, file);
    padWithNulls (missingBytes, file);
}

void RawDisc::dumpToFile (const std::string &fileName) {
    std::ofstream file (fileName);

    // assume that we never cached too many sectors - that check 
    // happens in addFile

    for (int i = 0; i < sectors.size(); i++)
        dumpSectorToFile ({i, 0, settings}, file);

    int missingBytes = (settings.totalSectors - sectors.size()) * settings.bytesPerSector;
    padWithNulls (missingBytes, file);

    file.flush();
}

void RawDisc::writeFileToSector (SecOff sector, std::ifstream& file) {
    Sector &outSec = getSectorAt (sector);

    file.read ((char*)outSec.data() + sector.offset, outSec.size() - sector.offset);

    for (auto i : getSectorAt (sector))
        std::cout << (int)i;
    std::cout << std::endl;
}

void RawDisc::writeFileToContiguousSectors (SecOff startSector, const std::filesystem::directory_entry &file) {
    int fSize = file.file_size();
    int fSizeSectors = fSize / settings.bytesPerSector + 1;
    ensureSecExists (startSector + fSizeSectors);
    std::ifstream fs (file.path().string());

    for (int i = 0; i < fSizeSectors; i++) 
        writeFileToSector (startSector + i, fs);
}

void RawDisc::enforceCorrectSecSize (SecOff sector) {
    Sector &sec = sectors [sector.sector];

    if (sec.size() != settings.bytesPerSector)
        sec.resize (settings.bytesPerSector);
}

void RawDisc::ensureSecExists (SecOff sector) {
    if (sectors.size() <= (sector.sector))
        sectors.resize (sector.sector + 1);
}

Sector& RawDisc::getSectorAt (SecOff sector) {
    ensureSecExists (sector);
    enforceCorrectSecSize (sector);
    return sectors[sector.sector];
}
