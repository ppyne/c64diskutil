#include "c64diskgeosrecord.h"

C64DiskGeosRecord::C64DiskGeosRecord(C64DiskSector *parent, char *data) : C64DiskBlock{parent, data}  {

}

void C64DiskGeosRecord::populateRecords(QList<GeosRecordPointer> &records) {
    for (int i = 0x02; i <= 0xFE; i = i + 0x02) {
        if ((unsigned char)this->Data[i] == 0 && (unsigned char)this->Data[i + 1] == 0) break;
        if ((unsigned char)this->Data[i] != 0 && (unsigned char)this->Data[i + 1] != 0xFF)
            records.append(GeosRecordPointer((unsigned char)this->Data[i], (unsigned char)this->Data[i + 1]));
    }
}
