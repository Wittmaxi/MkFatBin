#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <commander.hpp>
#include "disc.hpp"
#include "args.hpp"

void traverseFS (std::string &folderName, Disc& floppy) {
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

    for (const auto& dirEntry : recursive_directory_iterator(folderName))
        floppy.addFile (dirEntry);
}

int main (int argc, char** argv) {
    CMD::commander cmd(argc, argv);
    ProgramSettings set = processFlags (cmd);

    Disc floppy {set.flopSet};

    traverseFS (set.flopDir, floppy); // here, we fill the floppy with all of our files

    if (cmd.isFlagSet ("-b"))
        floppy.loadBootSector (cmd.getFlagValue("-b"));

    floppy.dumpToFile (set.outFile);
}
