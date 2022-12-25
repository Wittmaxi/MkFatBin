#include <iostream>
#include <fstream>
#include <vector>
#include <commander.hpp>
#include "disc.hpp"

int main (int argc, char** argv) {
    CMD::commander (argc, argv);
    Disc floppy {DiscSettings()};

    floppy.loadBootSector ("bootsector.bin");
    std::cout << "hellow" << std::endl; 
    floppy.dumpToFile ("flop.bin");
}
