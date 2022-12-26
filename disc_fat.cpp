#include "disc.hpp"
#include "file_descriptor.hpp"

void writeToSector (std::vector<char>& outSec, int secSize, const std::ifstream &file) {


}

void Disc::addFile (const std::filesystem::directory_entry &file) {
    int fsize = file.file_size();
    int sectors = fsize / settings.bytesPerSector;
    
    getFDesc (file);
}
