#include "geosrecordpointer.h"

GeosRecordPointer::GeosRecordPointer(int track, int sector) {
    this->Track = track;
    this->Sector = sector;
}

void GeosRecordPointer::set(int track, int sector) {
    this->Track = track;
    this->Sector = sector;
}

int GeosRecordPointer::track() {
    return this->Track;
}

int GeosRecordPointer::sector() {
    return this->Sector;
}
