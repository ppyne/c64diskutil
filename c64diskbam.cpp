#include "c64diskbam.h"
#include <string.h>
#include "c64disk.h"
#include "c64disktrack.h"

C64DiskBAM::C64DiskBAM(C64DiskSector *parent, char *data) : C64DiskBlock{parent, data} {

}
int C64DiskBAM::nextTrack() {
    if (this->Disk->type() == C64Disk::DT_D71 && this->Sector->id() == 0 && this->Track->id() == 18) return 53;
    else if (this->Disk->type() == C64Disk::DT_D81 && this->Sector->id() == 1 && this->Track->id() == 40) return 40;
    return 0;
}
int C64DiskBAM::nextSector() {
    if (this->Disk->type() == C64Disk::DT_D71 && this->Sector->id() == 0 && this->Track->id() == 18) return 0;
    else if (this->Disk->type() == C64Disk::DT_D81 && this->Sector->id() == 1 && this->Track->id() == 40) return 2;
    return 255;
}
int C64DiskBAM::freeSectors() {
    int total = 0;
    int track = 1;

    if (this->Disk->type() == C64Disk::DT_D64 && this->Track->id() == 18 && this->Sector->id() == 0) {
        for (int i = 0x04; i <= 0x8C; i = i + 0x04) {
            if (!this->Disk->isReservedTrack(track)) total = total + (unsigned char)this->Data[i];
            track++;
        }
    } else if (this->Disk->type() == C64Disk::DT_D71 && this->Track->id() == 18 && this->Sector->id() == 0) {
        // For tracks 1 to 35
        for (int i = 0x04; i <= 0x8C; i = i + 0x04) {
            if (!this->Disk->isReservedTrack(track)) total = total + (unsigned char)this->Data[i];
            track++;
        }
        // For tracks 36 to 70
        for (int i = 0xDD; i <= 0xFF; i++) {
            if (!this->Disk->isReservedTrack(track)) total = total + (unsigned char)this->Data[i];
            track++;
        }
    } else if (this->Disk->type() == C64Disk::DT_D81) {
        if (this->Sector->id() == 2) track = 41;
        for (int i = 0x10; i <= 0xFA; i = i + 0x06) {
            if (!this->Disk->isReservedTrack(track)) total = total + (unsigned char)this->Data[i];
            track++;
        }
    }
    return total;
}

void C64DiskBAM::populateFreeSectorsMap(QMap <int, QMap<int, bool>> &map) {
    const unsigned char MASK[8] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
    int track = 1;
    if (this->Disk->type() == C64Disk::DT_D64 || this->Disk->type() == C64Disk::DT_D71) {
        int begin = 0x04;
        int end = 0x8C;
        int inc = 0x04;
        int bias = 1;

        if (this->Disk->type() == C64Disk::DT_D71 && this->Track->id() == 53 && this->Sector->id() == 0) {
            track = 36;
            begin = 0x00;
            end = 0x66;
            inc = 0x03;
            bias = 0;
        }

        for (int i = begin; i <= end; i = i + inc) {
            unsigned char *p = (unsigned char *)(this->Data + i + bias);
            QMap<int, bool> sectors;
            int sector = 0;
            for (int j = 0; j < 3; j++) {
                unsigned char *q = p + j;
                int len = 8;
                if (j == 2) {
                    if (track <= 17) len = 5;
                    else if (track <= 24) len = 3;
                    else if (track <= 30) len = 2;
                    else if (track <= 35) len = 1;
                    else if (track <= 52) len = 5;
                    else if (track <= 59) len = 3;
                    else if (track <= 65) len = 2;
                    else if (track <= 70) len = 1;
                }
                for (int k = 0; k < len; k++) {
                    sectors.insert(sector, *q & MASK[k] ? true : false);
                    sector++;
                }
            }
            map.insert(track, sectors);
            track++;
        }
    } else if (this->Disk->type() == C64Disk::DT_D81) {
        if (this->Sector->id() == 2) track = 41;
        for (int i = 0x10; i <= 0xFA; i = i + 0x06) {
            unsigned char *p = (unsigned char *)(this->Data + i + 1);
            QMap<int, bool> sectors;
            int sector = 0;
            for (int j = 0; j < 5; j++) {
                unsigned char *q = p + j;
                for (int k = 0; k < 8; k++) {
                    sectors.insert(sector, *q & MASK[k] ? true : false);
                    sector++;
                }
            }
            map.insert(track, sectors);
            track++;
        }
    }
}
