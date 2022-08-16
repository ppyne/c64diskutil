#include "c64diskdirentries.h"
#include "c64disktrack.h"
#include "c64disk.h"
#include "c64exception.h"
#include <string.h>
#include <QDebug>
#include "c64diskgeosinfo.h"

C64DiskDirEntries::C64DiskDirEntries(C64DiskSector *parent, char *data) : C64DiskBlock{parent, data} {

}

C64DirectoryFile *C64DiskDirEntries::dirEntryAt(int i) {
    if (i < 0 || i > 7) throw new C64Exception("Index out of limits.");
    int offset = i * 32;

    int firsttrack = this->Data[0x03 + offset];
    int firstsector = this->Data[0x04 + offset];

    char name[17];
    name[16] = 0;
    strncpy(name, this->Data + 0x05 + offset, 16);
    C64DiskDirEntries::removeA0Padding(name);
    C64DiskDirEntries::petAsciiToAscii(name);

    unsigned short int size = (unsigned char)this->Data[0x1E + offset] + (unsigned char)this->Data[0x1F + offset] * 256;

    unsigned char t = (unsigned char)this->Data[0x02 + offset];
    unsigned char ft = t & 7;
    bool locked = false;
    bool closed = true;
    C64DirectoryFile::FileType type = C64DirectoryFile::FT_Unknown;
    if (t == 0)
        type = C64DirectoryFile::FT_DEL;
    else {
        if (ft == 1) type = C64DirectoryFile::FT_SEQ;
        else if (ft == 2) type = C64DirectoryFile::FT_PRG;
        else if (ft == 3) type = C64DirectoryFile::FT_USR;
        else if (ft == 4) type = C64DirectoryFile::FT_REL;
        else if (ft == 5 && this->Disk->type() == C64Disk::DT_D81) type = C64DirectoryFile::FT_CBM;
        else if (ft == 6) type = C64DirectoryFile::FT_DIR;
        if ((t & 0x80) == 0) closed = false;
        if ((t & 0x40) != 0) locked = true;
    }

    // Geos type
    unsigned char gt = (unsigned char)this->Data[0x18 + offset];
    C64DirectoryFile::GeosFileType gft = C64DirectoryFile::GFT_NonGEOS;
    if (gt > 0) {
        // File type
        switch (gt) {
            case 1:
                gft = C64DirectoryFile::GFT_Basic;
                break;
            case 2:
                gft = C64DirectoryFile::GFT_Assembly;
                break;
            case 3:
                gft = C64DirectoryFile::GFT_Data;
                break;
            case 4:
                gft = C64DirectoryFile::GFT_System;
                break;
            case 5:
                gft = C64DirectoryFile::GFT_DeskAccessory;
                break;
            case 6:
                gft = C64DirectoryFile::GFT_Application;
                break;
            case 7:
                gft = C64DirectoryFile::GFT_ApplicationData;
                break;
            case 8:
                gft = C64DirectoryFile::GFT_Font;
                break;
            case 9:
                gft = C64DirectoryFile::GFT_Printer;
                break;
            case 10:
                gft = C64DirectoryFile::GFT_InputDevice;
                break;
            case 11:
                gft = C64DirectoryFile::GFT_Disk;
                break;
            case 12:
                gft = C64DirectoryFile::GFT_Boot;
                break;
            case 13:
                gft = C64DirectoryFile::GFT_Temp;
                break;
            case 14:
                gft = C64DirectoryFile::GFT_AutoExec;
                break;
            default:
                gft = C64DirectoryFile::GFT_Unknown;
        }

        // Date
        QDateTime d = QDateTime();
        int y = (unsigned char)this->Data[0x19 + offset];
        int year = y >= 86 ? 1900 + y : 2000 + y;
        int month = (unsigned char)this->Data[0x1A + offset];
        int day = (unsigned char)this->Data[0x1B + offset];
        int hour = (unsigned char)this->Data[0x1C + offset];
        int min = (unsigned char)this->Data[0x1D + offset];

        if (day > 0 && day <= 31 &&
                month > 0 && month <= 12 &&
                hour >= 0 && hour <= 23 &&
                min >= 0 && min <= 59) {
            d.setDate(QDate(year, month, day));
            d.setTime(QTime(hour, min, 0));
        }
        if (!d.isValid()) d = QDateTime::fromMSecsSinceEpoch(0);

        // File structure
        C64DirectoryFile::GeosFileStruct gfs = C64DirectoryFile::GFS_Unknown;
        unsigned char gs = (unsigned char)this->Data[0x17 + offset];
        if (gs == 0) gfs = C64DirectoryFile::GFS_Sequence;
        else if (gs == 1) gfs = C64DirectoryFile::GFS_VLIR;

        // Info block
        unsigned char ibt = (unsigned char)this->Data[0x15 + offset];
        QPixmap icon = QPixmap();
        QString classname = QString();
        QString author = QString();
        QString application = QString();
        QString description = QString();
        C64DiskGeosInfo *ib = NULL;
        if (ibt != 0) {
            unsigned char ibs = (unsigned char)this->Data[0x16 + offset];
            ib = this->Disk->track(ibt)->sector(ibs)->toGeosInfo();
            // Info block Icon
            icon = ib->icon();
            // Info block Class
            classname = ib->className();
            // Info block Author
            if (gft != C64DirectoryFile::GFT_Font) author = ib->author();
            // Info block Application
            if (gft != C64DirectoryFile::GFT_Font) application = ib->application();
            // Info block Description
            description = ib->description();
        }

        return new C64DirectoryFile(this->Disk, firsttrack, firstsector, name, size, type, closed, locked, ib, gft, d, gfs,
                                    icon, classname, author, application, description);
    }

    return new C64DirectoryFile(this->Disk, firsttrack, firstsector, name, size, type, closed, locked);
}

C64DiskDirEntries *C64DiskDirEntries::next() {
    int track = this->nextTrack();
    if (track != 0) {
        int sector = this->nextSector();
        try {
            return this->Disk->track(track)->sector(sector)->toDirEntries();
        } catch (...) {
            qDebug() << "Could not reach track " << track << " sector " << sector << " while browsing the directory track.";
            return NULL;
        }
    }
    return NULL;
}

