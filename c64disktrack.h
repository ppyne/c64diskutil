#ifndef C64DISKTRACK_H
#define C64DISKTRACK_H

#include <QList>
#include "c64disksector.h"

class C64Disk;

class C64DiskTrack
{
public:
    C64DiskTrack(C64Disk *parent, int id);
    C64DiskTrack(C64Disk *parent, int id, char *data);
    ~C64DiskTrack();
    int id();
    C64Disk *parent();
    C64DiskSector *sector(int i);
    int sectorsCount();
private:
    QList<C64DiskSector *> Sectors;
    int ID;
    C64Disk *Parent;
    char *Data;
};

#endif // C64DISKTRACK_H
