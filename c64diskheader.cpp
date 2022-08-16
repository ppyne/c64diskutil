#include "c64diskheader.h"
#include "c64disk.h"
#include <string.h>

C64DiskHeader::C64DiskHeader(C64DiskSector *parent, char *data) : C64DiskBlock{parent, data} {

}

int C64DiskHeader::nextTrack() {
    if (this->Disk->type() == C64Disk::DT_D64 ||
            this->Disk->type() == C64Disk::DT_D71) return 18;
    else if (this->Disk->type() == C64Disk::DT_D81) return 40;
    return 0;
}

int C64DiskHeader::nextSector() {
    if (this->Disk->type() == C64Disk::DT_D64 ||
            this->Disk->type() == C64Disk::DT_D71) return 1;
    else if (this->Disk->type() == C64Disk::DT_D81) return 3;
    return 255;
}

QString C64DiskHeader::dosVersion() {
    return QString(QChar((unsigned char)this->Data[2]));
}

bool C64DiskHeader::isSoftWriteProteced() {
    if ((this->Disk->type() == C64Disk::DT_D64 ||
            this->Disk->type() == C64Disk::DT_D71) &&
            (this->Data[2] != 0x0 && this->Data[2] != 0x41))
        return true;
    else if ((this->Disk->type() == C64Disk::DT_D81) &&
             (this->Data[2] != 0x0 && this->Data[2] != 0x44))
        return true;
    return false;
}

QString C64DiskHeader::diskName() {
    char str[17];
    str[16] = 0;
    if (this->Disk->type() == C64Disk::DT_D64 ||
            this->Disk->type() == C64Disk::DT_D71)
        strncpy(str, this->Data + 0x90, 16);
    else if (this->Disk->type() == C64Disk::DT_D81)
        strncpy(str, this->Data + 0x04, 16);
    else str[0] = 0;
    C64DiskHeader::removeA0Padding(str);
    return QString(str);
}

QString C64DiskHeader::diskID() {
    char str[3];
    str[2] = 0;
    if (this->Disk->type() == C64Disk::DT_D64 ||
            this->Disk->type() == C64Disk::DT_D71)
        strncpy(str, this->Data + 0xA2, 2);
    else if (this->Disk->type() == C64Disk::DT_D81)
        strncpy(str, this->Data + 0x16, 2);
    else str[0] = 0;
    C64DiskHeader::removeA0Padding(str);
    return QString(str);
}

QString C64DiskHeader::dosType() {
    char str[3];
    str[2] = 0;
    if (this->Disk->type() == C64Disk::DT_D64 ||
            this->Disk->type() == C64Disk::DT_D71)
        strncpy(str, this->Data + 0xA5, 2);
    else if (this->Disk->type() == C64Disk::DT_D81)
        strncpy(str, this->Data + 0x19, 2);
    else str[0] = 0;
    C64DiskHeader::removeA0Padding(str);
    return QString(str);
}

QString C64DiskHeader::geosIDString() {
    // 0xAD - 0xBC
    char str[17];
    memset(str, '\0', 17);
    strncpy(str, this->Data + 0xAD, 16);
    return QString(str);
}

bool C64DiskHeader::isGeosDisk() {
    if (this->geosIDString().startsWith("GEOS format")) return true;
    return false;
}

bool C64DiskHeader::isDoubleSide() {
    if (this->Disk->type() == C64Disk::DT_D71 && (unsigned char)this->Data[03] == 0x80) return true;
    else if (this->Disk->type() == C64Disk::DT_D81) return true;
    return false;
}

QByteArray C64DiskHeader::data() {
    QByteArray data;
    if (this->Disk->type() == C64Disk::DT_D64 || this->Disk->type() == C64Disk::DT_D71) {
        for (int i = 0; i < 212; i++) data.append(this->Data[i]);
    } else if (this->Disk->type() == C64Disk::DT_D81) return C64DiskBlock::data();
    return data;
}
