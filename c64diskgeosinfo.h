#ifndef C64DISKGEOSINFO_H
#define C64DISKGEOSINFO_H

#include <QPixmap>
#include <QString>
#include "c64diskblock.h"

class C64DiskGeosInfo : public C64DiskBlock {
    public:
        C64DiskGeosInfo(C64DiskSector *parent, char *data);
        QPixmap icon();
        QString className();
        QString author();
        QString application();
        QString description();
        int fileStart();
        int fileEnd();
        int initProg();
};



#endif // C64DISKGEOSINFO_H
