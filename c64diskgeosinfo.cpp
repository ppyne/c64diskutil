#include "c64diskgeosinfo.h"
#include <QImage>
#include <QPainter>
#include <QBitmap>
#include <string.h>
#include <QDebug>

C64DiskGeosInfo::C64DiskGeosInfo(C64DiskSector *parent, char *data) : C64DiskBlock{parent, data} {

}

QPixmap C64DiskGeosInfo::icon() {
    unsigned char *data = (unsigned char *)this->Data + 0x05;
    QImage img = QImage(data, 24, 21, 3, QImage::Format_Mono);

    for (int i = 0; i < 21; i++) {
        unsigned char *p = img.scanLine(i);
        for (int j = 0; j < 3; j++) p[j] = ~p[j];
    }

    img = img.convertToFormat(QImage::Format_ARGB32);

    QPixmap pix = QPixmap(24, 24);
    pix.fill(Qt::transparent);
    QPainter painter = QPainter(&pix);
    painter.drawImage(0, 1, img);
    painter.end();
    pix.setMask(pix.createMaskFromColor(Qt::white));

    return pix.scaled(72, 72);
}

QString C64DiskGeosInfo::className() {
    const size_t len = 20;
    char str[len];
    strncpy(str, this->Data + 0x4D, len);
    return C64DiskGeosInfo::fromGeos(str, len);
}

QString C64DiskGeosInfo::author() {
    const size_t len = 20;
    char str[len];
    strncpy(str, this->Data + 0x61, len);
    return C64DiskGeosInfo::fromGeos(str, len);
}

QString C64DiskGeosInfo::application() {
    const size_t len = 20;
    char str[len];
    strncpy(str, this->Data + 0x75, len);
    return C64DiskGeosInfo::fromGeos(str, len);
}

QString C64DiskGeosInfo::description() {
    const size_t len = 96;
    char str[len];
    strncpy(str, this->Data + 0xA0, len);
    return C64DiskGeosInfo::fromGeos(str, len);
}

int C64DiskGeosInfo::fileStart() {
    return (unsigned char)this->Data[0x47] + (unsigned char)this->Data[0x48] * 256;
}

int C64DiskGeosInfo::fileEnd() {
    return (unsigned char)this->Data[0x49] + (unsigned char)this->Data[0x4A] * 256;
}

int C64DiskGeosInfo::initProg() {
    return (unsigned char)this->Data[0x4B] + (unsigned char)this->Data[0x4C] * 256;
}
