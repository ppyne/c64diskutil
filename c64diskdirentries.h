#ifndef C64DISKDIRENTRIES_H
#define C64DISKDIRENTRIES_H

#include "c64diskblock.h"
#include "c64disksector.h"
#include "c64directoryfile.h"

class C64DiskDirEntries : public C64DiskBlock {
    public:
        C64DiskDirEntries(C64DiskSector *parent, char *data);
        C64DirectoryFile *dirEntryAt(int i);
        C64DiskDirEntries *next();
};

#endif // C64DISKDIRENTRIES_H
