#include "addressing.hpp"

SecOff::SecOff (int sec, int off, DiscSettings &set) : settings (set) {
    sector = sec;
    offset = off;
}

const ClusOff SecOff::toClusOff () {
    int cluster = sector / settings.sectorsPerCluster;

    return ClusOff (cluster, offset, settings);
}

SecOff SecOff::operator+ (const SecOff& other) {
    sector += other.sector;
    offset += other.offset;
    return *this;
}

SecOff SecOff::operator+ (const int sec) {
    SecOff ret (sector, offset, settings);
    ret.sector += sec;
    return ret;
}

SecOff SecOff::operator++ () {
    sector++;
    return *this;
}

ClusOff::ClusOff (int clus, int off, DiscSettings& set) : settings (set) {
    settings = set;
    cluster = clus;
    offset = off;
}

const SecOff ClusOff::toSecOff () {
    int sector = cluster * settings.sectorsPerCluster;

    return SecOff (sector, offset, settings);
};

ClusOff ClusOff::operator+ (const ClusOff& other) {
    cluster += other.cluster;
    offset += other.offset;
    return *this;
}

ClusOff ClusOff::operator+ (const int clus) {
    ClusOff ret (cluster, offset, settings);
    ret.cluster += clus;
    return ret;
}

ClusOff ClusOff::operator++ () {
    cluster ++;
    return *this;
};
