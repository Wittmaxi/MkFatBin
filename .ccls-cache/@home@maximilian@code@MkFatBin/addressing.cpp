/* Copyright 2023 Maximilian Wittmer */
#include "addressing.hpp"

SecOff::SecOff(DiscSettings &set) : settings(set), sector(0), offset(0) {}

SecOff::SecOff(int sec, int off, DiscSettings &set) :
    settings(set), sector(sec), offset(off) {}

SecOff SecOff::setPos(int sec, int off) {
    sector = sec;
    offset = off;
    return *this;
}

const ClusOff SecOff::toClusOff() {
    // automatically convert to a Data cluster
    int rootDirSectors = settings.rootEntries * 32 / settings.bytesPerSector;
    int firstDataSector = settings.reservedSectors +
        (settings.numberFAT * settings.sectorsPerFAT) +
        rootDirSectors;

    int cluster = ((sector - firstDataSector) / settings.sectorsPerCluster) + 1;

    return ClusOff (cluster, offset, settings);
}

SecOff SecOff::addOff(int off) {
    offset += off;
    sector += offset / settings.bytesPerSector;
    offset = offset % settings.bytesPerSector;
    return *this;
}

SecOff SecOff::operator+ (const SecOff& other) {
    SecOff ret(sector, offset, settings);
    ret.sector += other.sector;
    ret.addOff(other.offset);
    return ret;
}

SecOff SecOff::operator+ (const int sec) {
    SecOff ret(sector, offset, settings);
    ret.sector += sec;
    return ret;
}

SecOff SecOff::operator- (const SecOff& other) {
    SecOff ret(sector, offset, settings);
    ret.sector -= other.sector;
    ret.addOff(-1 * other.offset);
    return ret;
}

SecOff SecOff::operator++ () {
    sector++;
    return *this;
}

SecOff SecOff::operator= (const SecOff& other) {
    sector = other.sector;
    offset = other.offset;
    settings = other.settings;
    return *this;
}

ClusOff::ClusOff(DiscSettings &set) : settings(set), cluster(0), offset(0) {}

ClusOff::ClusOff(int clus, int off, DiscSettings& set) :
    settings(set), cluster(clus), offset(off) {}

ClusOff ClusOff::setPos(int clus, int off) {
    cluster = clus;
    offset = off;
    return *this;
}

const SecOff ClusOff::toSecOff() {
    // Given any valid data cluster number N, the
    // sector number of the first sector of that cluster is
    // computed as follows
    int rootDirSectors = settings.rootEntries * 32 / settings.bytesPerSector;
    int firstDataSector = settings.reservedSectors +
       (settings.numberFAT * settings.sectorsPerFAT) +
       rootDirSectors;
    int sector = ((cluster - 2) * settings.sectorsPerCluster) + firstDataSector;

    return SecOff (sector, offset, settings);
}

ClusOff ClusOff::addOff(int off) {
    offset += off;
    cluster += offset / (settings.bytesPerSector * settings.sectorsPerCluster);
    offset = offset % (settings.bytesPerSector * settings.sectorsPerCluster);
    return *this;
}

ClusOff ClusOff::operator+ (const ClusOff& other) {
    cluster += other.cluster;
    offset += other.offset;
    return *this;
}

ClusOff ClusOff::operator+ (const int clus) {
    ClusOff ret(cluster, offset, settings);
    ret.cluster += clus;
    return ret;
}

ClusOff ClusOff::operator- (const ClusOff& other) {
    ClusOff ret(cluster, offset, settings);
    ret.cluster -= other.cluster;
    ret.addOff(-1 * other.cluster);
    return ret;
}

ClusOff ClusOff::operator++ () {
    cluster++;
    return *this;
}

ClusOff ClusOff::operator= (const ClusOff& other) {
    cluster = other.cluster;
    offset = other.offset;
    settings = other.settings;
    return *this;
}
