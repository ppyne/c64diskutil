#ifndef C64DISKGEOSRECORD_H
#define C64DISKGEOSRECORD_H

#include "c64diskblock.h"
#include "geosrecordpointer.h"
#include <QList>

class C64DiskGeosRecord : public C64DiskBlock {
    public:
        C64DiskGeosRecord(C64DiskSector *parent, char *data);
        void populateRecords(QList<GeosRecordPointer> &records);

};

#endif // C64DISKGEOSRECORD_H
