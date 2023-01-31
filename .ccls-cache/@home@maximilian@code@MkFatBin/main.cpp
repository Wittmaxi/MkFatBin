/* Copyright 2023 Maximilian Wittmer */
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <commander.hpp>
#include "fat_driver.hpp"
#include "args.hpp"

void traverseFS(const std::string &folderName, FATDriver& floppy) {
    /* ccls-ignore */
    typedef std::filesystem::recursive_directory_iterator rec_dir_iter;

    for (const auto& dirEntry : rec_dir_iter(folderName))
        floppy.addFile(dirEntry);
}

int main(int argc, char** argv) {
    CMD::commander cmd(argc, argv);
    ProgramSettings set = processFlags(cmd);
    if (!set.execute) return 0;

    FATDriver floppy {set.flopSet};

    traverseFS(set.flopDir, floppy);  // here, we fill the floppy with
                                      // all of our files

    if (cmd.isFlagSet ("-b"))
        floppy.loadBootSector(cmd.getFlagValue("-b"));

    floppy.dumpToFile(set.outFile);
    std::cout << "done\n";
}
