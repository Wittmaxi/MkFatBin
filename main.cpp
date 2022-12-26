#include <iostream>
#include <fstream>
#include <vector>
#include <commander.hpp>
#include "disc.hpp"
#include "args.hpp"


int main (int argc, char** argv) {
    CMD::commander cmd(argc, argv);
    ProgramSettings set = processFlags (cmd);

    Disc floppy {set.flopSet};

    if (cmd.isFlagSet ("-b"))
        floppy.loadBootSector (cmd.getFlagValue("-b"));

    floppy.dumpToFile (set.outFile);
}
