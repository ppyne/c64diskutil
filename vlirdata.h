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
        int id();
    private:
        QString Label;
        int Track;
        int Sector;
        QByteArray Data;
        int Index;
        int ID;
};

#endif // VLIRDATA_H
