#include "vlirdata.h"

VLIRData::VLIRData(GeosRecordPointer &pointer, QByteArray &data, int index) {
    this->Track = pointer.track();
    this->Sector = pointer.sector();
    this->ID = pointer.id();
    this->Data = data;
    this->Label = QString("Record %1 on track ").arg(pointer.id(), 3, 10, QLatin1Char('0')) + QString::number(track()) + " sector " + QString::number(sector());
    this->Index = index;
}

QString &VLIRData::label() {
    return this->Label;
}

int VLIRData::track() {
    return this->Track;
}

int VLIRData::sector() {
    return this->Sector;
}

int VLIRData::index() {
    return this->Index;
}

int VLIRData::id() {
    return this->ID;
}

QByteArray &VLIRData::data() {
    return this->Data;
}
