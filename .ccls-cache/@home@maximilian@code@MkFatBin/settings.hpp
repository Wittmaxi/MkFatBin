#pragma once
#include <cstdint>

class __attribute__((__packed__)) DiscSettings {
public:
    char OEM[8] = { 'M', 'A', 'X', 'I', 'F', 'A', 'T', 0 };
    uint16_t bytesPerSector = 0x200; // program assumes that bytesPerSector % 32 == 0. If this is untrue, segfaults can happen because of memcpy-boundaries. TODO: fix writing mechanism
    uint8_t sectorsPerCluster = 0x1; 
    uint16_t reservedSectors = 0x1;
    uint8_t numberFAT = 0x2; // by default, write two FATs
    uint16_t rootEntries = 0xE0;
    uint16_t totalSectors = 0x0B40; // Make sure to be formatted as FAT16 TODO: FAT12-Support
    uint8_t mediaDescriptor = 0xF0;
    uint16_t sectorsPerFAT = 0x9;
    uint16_t sectorsPerTrack = 0x12;
    uint16_t headCount = 0x2;
    uint32_t hiddenSectors = 0x0;
    uint32_t largeSectorCount = 0x0;
    // extended BPB
    uint8_t driveNumber = 0x0; // ignore
    uint8_t winNTFlags = 0x0; // ignore
    uint8_t signature = 0x29; // Signature - mandatory value
    uint32_t volumeID = 0x0; // ignore
    char volumeLabel[11] = "VOLUMELABL";
    char systemIdentifierString[8] = "FAT16";
};
