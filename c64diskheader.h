#ifndef C64DISKHEADER_H
#define C64DISKHEADER_H

#include "c64diskblock.h"
#include "c64disksector.h"
#include <QString>

class C64DiskHeader : public C64DiskBlock {
    public:
        C64DiskHeader(C64DiskSector *parent, char *data);
        int nextTrack();
        int nextSector();
        QString dosVersion();
        bool isSoftWriteProteced();
        QString diskName();
        QString diskID();
        QString dosType();
        QString geosIDString();
        bool isGeosDisk();
        bool isDoubleSide();
        QByteArray data();
};

#endif // C64DISKHEADER_H
