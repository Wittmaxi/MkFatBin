#pragma once
#include <filesystem>
#include <vector>

class FDesc {
    FDesc (const std::filesystem::path &file);
    writeFDesc (const std::vector &dest);
};
