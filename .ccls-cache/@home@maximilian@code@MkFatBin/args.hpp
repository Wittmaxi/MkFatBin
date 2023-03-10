/* Copyright 2023 Maximilian Wittmer */
#pragma once
#include <iostream>
#include <string>
#include <commander.hpp>
#include "disc.hpp"

struct ProgramSettings {
    DiscSettings flopSet;
    std::string outFile = "out.bin";
    std::string bootSector = "";
    std::string flopDir = "";
    bool execute = true;
};

void displayHelp() {
    std::cout << "\
MkFatBin - Copyright Maximilian Wittmer - 2022 \n \
\n \
Utility to create FAT-formatted binary files which \n \
can be used by QEMU or BOCHs, or can be mounted as \n \
normal files. \n \
\n \
By default, assumes FAT16. (support for further FATs might come later) \n \
\n \
\t -h \n \
\t \t display this message \n\n \
\t -b <file>\n \
\t\t file binary file to use as bootsector \n \
\t\t will overwrite the BIOS parameter-block addresses.\n \
\t\t For use with a bootsector, the first three bytes need to be \n \
\t\t 0x90 (NOP) 0xEB (near Jump) 0x3C (skip the table). \n \
\t\t Can be left out, in which case the boot sector will \
be used only for the BPB. \n\n \
\t -o <file>\n \
\t\t output file. Defaults to out.bin \n\n \
\t -d <directory> \n \
\t\t Directory of the file, which will be formatted onto the floppy.\n \
\t\t Required argument.\n";
}

// some flags can't be processed right now
ProgramSettings processFlags(CMD::commander &cmd) {
    ProgramSettings ret;

    if (cmd.isFlagSet("-h")) {
        displayHelp();
        ret.execute = false;
        return ret;
    }

    if (cmd.isFlagSet("-o"))
        ret.outFile = cmd.getFlagValue("-o");

    if (cmd.isFlagSet("-b"))
        ret.bootSector = cmd.getFlagValue("-b");

    if (cmd.isFlagSet ("-d"))
        ret.flopDir = cmd.getFlagValue("-d");
    else {
        std::cerr << "[ERROR] Expected -d. \n";
        displayHelp();
        ret.execute = false;
    }

    return ret;
}
