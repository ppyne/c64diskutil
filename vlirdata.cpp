#include "vlirdata.h"

VLIRData::VLIRData(GeosRecordPointer &pointer, QByteArray &data, int index) {
    this->Track = pointer.track();
    this->Sector = pointer.sector();
    this->Data = data;
    this->Label = QString("Record %1 on track ").arg(index + 1, 3, 10, QLatin1Char('0')) + QString::number(track()) + " sector " + QString::number(sector());
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

QByteArray &VLIRData::data() {
    return this->Data;
}
