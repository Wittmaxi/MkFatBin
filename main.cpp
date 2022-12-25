#include <iostream>
#include <fstream>
#include <vector>
#include <commander.hpp>
#include "disc.hpp"

void displayHelp () {
    std::cout << "\n \
MkFatBin - Copyright Maximilian Wittmer - 2022 \n \
\n \
Utility to create FAT-formatted binary files which \n \
can be used by QEMU or BOCHs, or can be mounted as \n \
normal files. \n \
\n \
By default, assumes FAT12. (support for further FATs might come later) \n \
\n \
\t -h \n \
\t \t display this message \n \
\t -b <file>\n \
\t\t file binary file to use as bootsector \n \
\t\t will overwrite the BIOS parameter-block addresses.\n \
\t\t For use with a bootsector, the first three bytes need to be \n \
\t\t 0x90 (NOP) 0xEB (near Jump) 0x3C (skip the table). \n \
\t\t Can be left out, in which case the boot sector will be used only for the BPB. \n \
\t -o <file>\n \
\t\t output file. Defaults to out.bin" << std::endl; 

}

int main (int argc, char** argv) {
    CMD::commander cmd(argc, argv);
    Disc floppy {DiscSettings()};
    
    if (cmd.isFlagSet ("-h"))
        displayHelp();

    if (cmd.isFlagSet ("-b"))
        floppy.loadBootSector (cmd.getFlagValue("-b"));

    std::string outName = cmd.isFlagSet ("-o") ? cmd.getFlagValue ("-o") : "out.bin";

    floppy.dumpToFile (outName);
}
