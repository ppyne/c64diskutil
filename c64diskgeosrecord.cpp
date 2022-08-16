#include "c64diskgeosrecord.h"
#include <QDebug>

C64DiskGeosRecord::C64DiskGeosRecord(C64DiskSector *parent, char *data) : C64DiskBlock{parent, data}  {

}

void C64DiskGeosRecord::populateRecords(QList<GeosRecordPointer> &records) {
    int id = 0;
    for (int i = 0x02; i <= 0xFE; i = i + 0x02) {
        //qDebug() << "VLIR Record" << id << (unsigned char)this->Data[i] << (unsigned char)this->Data[i + 1];
        if ((unsigned char)this->Data[i] == 0 && (unsigned char)this->Data[i + 1] == 0) break;
        if ((unsigned char)this->Data[i] != 0 && (unsigned char)this->Data[i + 1] != 0xFF)
            records.append(GeosRecordPointer((unsigned char)this->Data[i], (unsigned char)this->Data[i + 1], id));
        id++;
    }
}
