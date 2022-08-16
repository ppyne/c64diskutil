#include "geosrecordpointer.h"

GeosRecordPointer::GeosRecordPointer(int track, int sector, int id) {
    this->Track = track;
    this->Sector = sector;
    this->ID = id;
}

int GeosRecordPointer::track() {
    return this->Track;
}

int GeosRecordPointer::sector() {
    return this->Sector;
}

int GeosRecordPointer::id() {
    return this->ID;
}
