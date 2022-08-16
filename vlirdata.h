#ifndef VLIRDATA_H
#define VLIRDATA_H

#include <QString>
#include <QByteArray>
#include "geosrecordpointer.h"

class VLIRData {
    public:
        VLIRData(GeosRecordPointer &pointer, QByteArray &data, int index);
        QString &label();
        int track();
        int sector();
        QByteArray &data();
        int index();
    private:
        QString Label;
        int Track;
        int Sector;
        QByteArray Data;
        int Index;
};

#endif // VLIRDATA_H
