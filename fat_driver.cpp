#include "fat_driver.hpp"
#include "file_descriptor.hpp"

FATDriver::FATDriver (DiscSettings set) : settings (set), disc(set) {
    writeSettingsToBootsector();
    FATsector = set.reservedSectors + set.hiddenSectors;
    rootSector = FATsector + set.numberFAT * set.sectorsPerFAT;
    rootSizeBytes = 32 * set.rootEntries;
    nextFreeSector = rootSector + rootSizeBytes / set.bytesPerSector;
}

// we are putting in the files in a contiguous manner, thus we can simplify this
void FATDriver::indexInFAT (int startSector, int numberSectors) {  // Assume: cluster = sector. TODO: FIX
    SecOff sec (startSector, 0, settings);

    for (int i; i < numberSectors; i++) {
        int entryIndex = i * 2 ;
        int fatSectorOffset = entryIndex / settings.bytesPerSector;
        entryIndex %= settings.bytesPerSector;

        Sector &secVector = disc.getSectorAt (sec + FATsector);
        disc.enforceCorrectSecSize (sec + FATsector);

        uint16_t newValue = (i + 1 == startSector + numberSectors) ? 0xFFFF : (i + 1);

        std::memcpy (secVector.data() + entryIndex, reinterpret_cast <char*>(&newValue), 2); 
    }
}

std::tuple<int, int> FATDriver::calcNextEmptyEntryIndex() {
    for (int i = 0; i <= settings.rootEntries; i++) {
        int sec = rootSector + (i * 32) / settings.bytesPerSector;
        int offset = (i * 32) % settings.bytesPerSector;

        disc.enforceCorrectSecSize ({sec, 0, settings});
        if (disc.getSectorAt ({sec, 0, settings})[offset] == 0)
            return std::tuple <int, int> (sec, offset);
    }
    // we haven't returned, there are no more empty roots
    return std::tuple <int, int> (-1, -1);
}

void FATDriver::writeRootEntry (const std::vector<uint8_t> &fDesc) {
    std::tuple<int, int> nextEmptyRoot = calcNextEmptyEntryIndex();
    if (std::get<0> (nextEmptyRoot) < 0) {
        return;
    }
    
    std::memcpy (disc.getSectorAt ({std::get<0> (nextEmptyRoot), 0, settings }).data() + std::get<1> (nextEmptyRoot), fDesc.data(), 32);
}

void FATDriver::addFile (const std::filesystem::directory_entry &file) {
    if (file.is_directory()) return; // TODO handle dirs

    int fileStartCluster = nextFreeSector;

    disc.writeFileToContiguousSectors ({nextFreeSector, 0, settings}, file);

    std::vector <uint8_t> fDesc = getFDesc (file);
    fDesc [26] = (uint8_t) fileStartCluster;
    fDesc [27] = (uint8_t) fileStartCluster >> 8;

    writeRootEntry (fDesc);
}


inline void FATDriver::readBootSector (const std::string &fileName) {
    std::ifstream file (fileName);
    disc.writeFileToSector ({0, 0, settings}, file);
}

inline void FATDriver::writeSettingsToBootsector () {
    constexpr const int bpbSize = sizeof (DiscSettings);

    SecOff bs (0, 0, settings);
    Sector &bootsecVec = disc.getSectorAt (bs);
    disc.enforceCorrectSecSize(bs);

    std::memcpy (bootsecVec.data() + 3, reinterpret_cast <char*>(&settings), bpbSize);
}

void FATDriver::dumpToFile (const std::string &fileName) {
    disc.dumpToFile (fileName);
}

void FATDriver::loadBootSector (const std::string &fileName) {
    //boot sector is ALWAYS sector 0 (sector 1 in CHL addressing)
    readBootSector (fileName);
    writeSettingsToBootsector();
}
