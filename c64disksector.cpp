#include "c64disksector.h"
#include "c64disktrack.h"
#include "c64disk.h"
#include "c64diskbam.h"
#include "c64diskheader.h"
#include "c64diskdirentries.h"
#include "c64diskgeosinfo.h"
#include "c64diskgeosrecord.h"

C64DiskSector::C64DiskSector(C64DiskTrack *parent, int id, char *data) {
    this->Parent = parent;
    this->ID = id;
    this->Data = data;
    this->Block = new C64DiskBlock(this, this->Data);
    this->parent()->parent()->appendBlock(this->Block);
}

C64DiskSector::~C64DiskSector() {

}

C64DiskTrack *C64DiskSector::parent() {
    return this->Parent;
}

int C64DiskSector::id() {
    return this->ID;
}

int C64DiskSector::nextTrack() {
    return this->Block->nextTrack();
}

int C64DiskSector::nextSector() {
    return this->Block->nextSector();
}

C64DiskBlock *C64DiskSector::toBlock() {
    return this->Block;
}

C64DiskBAM *C64DiskSector::toBAM() {
    return (C64DiskBAM *)this->Block;
}

C64DiskHeader *C64DiskSector::toHeader() {
    return (C64DiskHeader *)this->Block;
}

C64DiskDirEntries *C64DiskSector::toDirEntries() {
    return (C64DiskDirEntries *)this->Block;
}

C64DiskGeosInfo *C64DiskSector::toGeosInfo() {
    return (C64DiskGeosInfo *)this->Block;
}

C64DiskGeosRecord *C64DiskSector::toGeosRecord() {
    return (C64DiskGeosRecord *)this->Block;
}
