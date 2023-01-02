#include "fat_driver.hpp"
#include "file_descriptor.hpp"

FATDriver::FATDriver (DiscSettings set) : settings (set), disc(set), nextFreeSector (settings), rootSector(settings), FATSector (settings), firstDataCluster (settings) {
    writeSettingsToBootsector();

    FATSector.setPos (set.reservedSectors + set.hiddenSectors);
    rootSector = FATSector + set.numberFAT * set.sectorsPerFAT;
    int bytesPerCluster = set.bytesPerSector * set.sectorsPerCluster;
    nextFreeSector = rootSector + (set.rootEntries * 32) / set.bytesPerSector;
    firstDataCluster = nextFreeSector.toClusOff();

    setupFAT ();
}

inline void FATDriver::setupFAT () {
    ClusOff address {-1, 0, settings};
    writeFATEntry (address, 0xFFFF);
    ++address;
    writeFATEntry (address, 0xFFFF);

}

void FATDriver::writeFATEntry (ClusOff addressCluster, uint16_t value) {
    SecOff offsetInFATMap = FATSector;
    std::cout << "writing to cluster " << addressCluster.cluster << "\n";
    offsetInFATMap.addOff ((addressCluster.cluster + 1) * 2); // each cluster takes up two bytes in the FAT
    std::vector<uint8_t> toWrite ((uint8_t*)&value, (uint8_t*)&value + 2);
    disc.writeArrayToContiguousSectors (offsetInFATMap, toWrite);
}

// we are putting in the files in a contiguous manner, thus we can simplify this
void FATDriver::indexInFAT (ClusOff startCluster, int numberClusters) {
    for (uint16_t i = 1; i <= numberClusters; i++) {
        uint16_t newValue = (i + 1 == numberClusters) ? 0xFFFF : (startCluster.cluster + i + 1);
        writeFATEntry (startCluster + i + 1, newValue);
    }
}

SecOff FATDriver::calcNextEmptyEntryIndex() {
    SecOff sec = rootSector;
    for (int i = 0; i <= settings.rootEntries; i++) {
        if (disc.getSectorAt (sec)[sec.offset] == 0) return sec;

        sec.addOff(32);
    }
    // we haven't returned, there are no more empty roots
    return SecOff (-1, -1, settings);
}

void FATDriver::writeRootEntry (const std::vector<uint8_t> &fDesc) {
    SecOff nextEmptyRoot = calcNextEmptyEntryIndex();
    if (nextEmptyRoot.sector < 0)
        return;
    
    disc.writeArrayToContiguousSectors (nextEmptyRoot, fDesc);
}

void FATDriver::addFile (const std::filesystem::directory_entry &file) {
    if (file.is_directory()) return; // TODO handle dirs
    
    std::ifstream fs (file.path().string());


    ClusOff fileStartCluster = nextFreeSector.toClusOff();
    disc.writeFileToContiguousSectors (nextFreeSector, file);

    int sizeInSectors = file.file_size() / settings.bytesPerSector + 1;
    nextFreeSector = nextFreeSector + sizeInSectors;

    indexInFAT (fileStartCluster, sizeInSectors / settings.sectorsPerCluster);

    std::vector <uint8_t> fDesc = getFDesc (file);
    std::cout << "NFS = " << nextFreeSector.sector << "\n";
    std::cout << "FSC = " << fileStartCluster.cluster << "\n";
    uint16_t filePos = fileStartCluster.cluster + 1;
    std::cout << filePos << " FILEPOS \n";
    fDesc [26] = (uint8_t) filePos;
    fDesc [27] = (uint8_t) filePos >> 8;
    std::cout << "writing root entry \n";

    writeRootEntry (fDesc);
    std::cout << "root entry written \n"; 
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
