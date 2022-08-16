#include "directorypage.h"

DirectoryPage::DirectoryPage(int track, int sector, QByteArray data, int index) {
    this->Data = data;
    this->Sector = sector;
    this->Track = track;
    this->Index = index;
    this->Label = QString("Block %1 on track ").arg(index + 1, 3, 10, QLatin1Char('0')) + QString::number(this->track()) + " sector " + QString::number(this->sector());
}

QString &DirectoryPage::label() {
    return this->Label;
}

int DirectoryPage::track() {
    return this->Track;
}

int DirectoryPage::sector() {
    return this->Sector;
}

QByteArray &DirectoryPage::data() {
    return this->Data;
}

int DirectoryPage::index() {
    return Index;
}
