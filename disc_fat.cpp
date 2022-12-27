#include "disc.hpp"
#include "file_descriptor.hpp"

void writeToSector (std::vector<char>& outSec, int secSize, std::ifstream &file) {
    if (outSec.size() < secSize)
        outSec.resize(secSize);

    file.read (outSec.data(), secSize);
}

// we are putting in the files in a contiguous manner, thus we can simplify this
void Disc::indexInFAT (int startSector, int numberSectors) {  // Assume: cluster = sector. TODO: FIX
    int i = startSector;

    for (; i < startSector + numberSectors; i++) {
        int entryIndex = i * 2 ;
        int fatSectorOffset = entryIndex / settings.bytesPerSector;
        entryIndex %= settings.bytesPerSector;

        if (rawSectors[FATsector + fatSectorOffset].size() < settings.bytesPerSector) {
            rawSectors[FATsector + fatSectorOffset].resize (settings.bytesPerSector);
        }


        uint16_t newValue = (i + 1 == startSector + numberSectors) ? 0xFFFF : (i + 1);

        std::memcpy (rawSectors[FATsector + fatSectorOffset].data() + entryIndex, reinterpret_cast <char*>(&newValue), 2); 
    }
}


// TODO refactor - If jesus sees this code, my place in paradise gets cancelled
void Disc::writeFileToSectors (const std::filesystem::directory_entry &file) {
    int fsize = file.file_size();
    int sectors = (fsize / settings.bytesPerSector) + 1;
    
    if (rawSectors.size() < nextFreeSector + sectors)
        rawSectors.resize(nextFreeSector + sectors);
    
    std::ifstream fs (file.path().string());

    for (int i = 0;  i < sectors; i++)
        writeToSector (rawSectors [nextFreeSector + i], settings.bytesPerSector, fs);

    indexInFAT (nextFreeSector, sectors);

    nextFreeSector += sectors;
}

std::tuple<int, int> Disc::calcNextEmptyRootIndex() {
    for (int i = 0; i <= settings.rootEntries; i++) {
        int sec = rootSector + (i * 32) / settings.bytesPerSector;
        int offset = (i * 32) % settings.bytesPerSector;

        if (rawSectors[sec][offset] == 0)
            return std::tuple <int, int> (sec, offset);
    }
    // we haven't returned, there are no more empty roots
    return std::tuple <int, int> (-1, -1);
}

void Disc::writeRootEntry (const std::vector<uint8_t> &fDesc) {
    std::tuple<int, int> nextEmptyRoot = calcNextEmptyRootIndex();
    if (std::get<0> (nextEmptyRoot) < 0) {
        std::cout << "Can't write file " << std::string ((char*) fDesc.data()) << " to disk. No more root-entries available \n"; // slight off-chance that this segfaults if the FDesc does not contain any zeroes at all. Since I don't store time info yet, these fields will always be 0 and guard us from the segfault TODO: FIX
        return;
    }

    std::memcpy (rawSectors [std::get<0> (nextEmptyRoot)].data() + std::get<1> (nextEmptyRoot), fDesc.data(), 32);
}

void Disc::addFile (const std::filesystem::directory_entry &file) {
    if (file.is_directory()) return; // TODO handle dirs

    int fileStartCluster = nextFreeSector;

    writeFileToSectors (file);
    std::vector <uint8_t> fDesc = getFDesc (file);
    fDesc [26] = (uint8_t) fileStartCluster;
    fDesc [27] = (uint8_t) fileStartCluster >> 8;

    writeRootEntry (fDesc);
}
