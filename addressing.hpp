#pragma once
#include "settings.hpp"

class SecOff;
class ClusOff;

// assumes that all addresses are issued from the same raw-disk
// with the same settings, thus we skip checks for same settings in operation overloads

class SecOff {
public:
    DiscSettings& settings;
    int sector;
    int offset;
    SecOff (int sec, int off, DiscSettings &set);
    SecOff operator+ (const SecOff& other);
    SecOff operator+ (const int sec);
    SecOff operator++();
    const ClusOff toClusOff ();
};

class ClusOff {
public:
    DiscSettings& settings;
    int cluster;
    int offset;
    ClusOff (int clus, int off, DiscSettings& set);
    ClusOff operator+ (const ClusOff& other);
    ClusOff operator+ (const int clus);
    ClusOff operator++();
    const SecOff toSecOff();
};
