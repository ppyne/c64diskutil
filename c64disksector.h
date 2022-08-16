#ifndef C64DISKSECTOR_H
#define C64DISKSECTOR_H

#include "c64diskblock.h"

class C64DiskTrack;
class C64DiskBAM;
class C64DiskHeader;
class C64DiskDirEntries;
class C64DiskGeosInfo;
class C64DiskGeosRecord;

class C64DiskSector {
    public:
        C64DiskSector(C64DiskTrack *parent, int id, char *data);
        ~C64DiskSector();
        C64DiskTrack *parent();
        int id();
        int nextTrack();
        int nextSector();
        C64DiskBlock *toBlock();
        C64DiskBAM *toBAM();
        C64DiskHeader *toHeader();
        C64DiskDirEntries *toDirEntries();
        C64DiskGeosInfo *toGeosInfo();
        C64DiskGeosRecord *toGeosRecord();
    private:
        C64DiskTrack *Parent;
        int ID;
        char *Data;
        C64DiskBlock *Block;
};

#endif // C64DISKSECTOR_H
