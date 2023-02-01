#pragma once
#include <iostream>
#include <filesystem>
#include <vector>
#include <cstring>

std::string getSmallFileName (const std::filesystem::directory_entry &dir) {
    std::string fileName (dir.path().stem().string());
    fileName.resize (8);
    for (auto &i: fileName) // replace all zeroes with 
        if (i == 0) i = ' ';
    std::string extension (dir.path().extension().string());
    extension.resize(4);
    return fileName + extension.substr(1,3); // get rid of the . in fileext
}

uint8_t getFileAttribute (const std::filesystem::directory_entry &dir) {
    if (dir.is_directory())
        return 0x10;
    return 0x0;
}

std::vector<uint8_t> getFDesc (const std::filesystem::directory_entry &dir) {
    std::vector <uint8_t> ret (32, 0);

    std::memcpy (ret.data(), getSmallFileName (dir).data(), 11);
    ret[11] = getFileAttribute (dir);
    ret[12] = 0x0; // reserved
    ret[13] = 0x0; // skip fine-resolution
    // TODO set current time... currently, everything is 000
    ret[14] = 0x0;
    ret[15] = 0x0;
    // TODO set current date
    ret[16] = 0x0;
    ret[17] = 0x0;
    // TODO set last access date
    ret[18] = 0x0;
    ret[19] = 0x0;
    /// TODO: set perms correctly
    ret[20] = 0x0;
    ret[21] = 0x0;
    // TODO last access times
    ret[22] = 0x0;
    ret[23] = 0x0;
    ret[24] = 0x0;
    ret[25] = 0x0;
    // let the caller fixup the segment address
    ret[26] = 0x0;
    ret[27] = 0x0;
    //
    uint32_t fileSize = dir.is_regular_file() ? dir.file_size() : 0;
    std::memcpy (ret.data() + 28, reinterpret_cast<char*>(&fileSize), 4);


    return ret;
}
