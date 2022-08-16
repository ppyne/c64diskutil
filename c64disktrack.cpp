#include "c64disktrack.h"
#include "c64disk.h"
#include "c64exception.h"
#include <QDebug>

#define SECTOR_SIZE 256

C64DiskTrack::C64DiskTrack(C64Disk *parent, int id) {
    this->Parent = parent;
    this->ID = id;
    this->Data = NULL;
}
C64DiskTrack::C64DiskTrack(C64Disk *parent, int id, char *data) {
    this->Parent = parent;
    this->ID = id;
    this->Data = data;
    C64DiskSector *sector;
    int len = 0;
    if (this->parent()->type() == C64Disk::DT_D64) {
        if (this->id() <= 17) len = 21;
        else if (this->id() <= 24) len = 19;
        else if (this->id() <= 30) len = 18;
        else if (this->id() <= 35) len = 17;
    } else if (this->parent()->type() == C64Disk::DT_D71) {
        if (this->id() <= 17) len = 21;
        else if (this->id() <= 24) len = 19;
        else if (this->id() <= 30) len = 18;
        else if (this->id() <= 35) len = 17;
        else if (this->id() <= 52) len = 21;
        else if (this->id() <= 59) len = 19;
        else if (this->id() <= 65) len = 18;
        else if (this->id() <= 70) len = 17;
    } else if (this->parent()->type() == C64Disk::DT_D81) len = 40;
    for (int s = 0; s < len; s++) {
        /*if (this->id() == 18) {
            qDebug() << "Extracting Track 18 Sector " << s << " telling next coords track " << (int)data[0] << " sector " << (int)data[1] << data - this->Data;
        }*/
        sector = new C64DiskSector(this, s, data);
        this->Sectors.append(sector);
        this->parent()->appendSector(sector);
        data = data + SECTOR_SIZE;
    }
}
C64DiskTrack::~C64DiskTrack() {
    while (!this->Sectors.isEmpty()) delete this->Sectors.takeFirst();
    if (this->Data != NULL) delete []this->Data;
}
int C64DiskTrack::id() {
    return this->ID;
}
C64Disk *C64DiskTrack::parent() {
    return this->Parent;
}
C64DiskSector *C64DiskTrack::sector(int i) {
    if (i < 0 || i > this->Sectors.count() - 1) throw new C64Exception("Track index out of limits.");
    //qDebug() << "returning sector " << i << " from track " << this->id();
    return this->Sectors.at(i);
}
int C64DiskTrack::sectorsCount() {
    return this->Sectors.count();
}
