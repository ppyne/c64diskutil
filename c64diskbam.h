#ifndef C64DISKBAM_H
#define C64DISKBAM_H

#include "c64diskblock.h"
#include "c64disksector.h"
#include <QString>
#include <QMap>

class C64DiskBAM : public C64DiskBlock {
    public:
        C64DiskBAM(C64DiskSector *parent, char *data);
        int nextTrack();
        int nextSector();
        int freeSectors();
        void populateFreeSectorsMap(QMap <int, QMap<int, bool>> &map);
};

#endif // C64DISKBAM_H
