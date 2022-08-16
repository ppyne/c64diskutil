#include "c64disk.h"
#include "c64exception.h"
#include <QDebug>
#include "c64diskheader.h"
#include "c64diskdirentries.h"
#include "c64diskbam.h"
#include <string.h>

#define SECTOR_SIZE 256

const char *DISK_TYPE_NAMES[] = {
    "unknown",
    "d64",
    "d71",
    "d81"
};

C64Disk::C64Disk(QObject *parent) : QObject{parent} {
    this->Type = C64Disk::DT_Unknown;
    this->File = NULL;
}

C64Disk::~C64Disk() {
    while (!this->Tracks.isEmpty()) delete this->Tracks.takeFirst();
    if (this->File != NULL) delete this->File;
}

C64Disk::DiskType C64Disk::type() {
    return this->Type;
}

QString C64Disk::typeString() {
    return DISK_TYPE_NAMES[this->type()];
}

void C64Disk::appendSector(C64DiskSector *sector) {
    this->Sectors.append(sector);
}

void C64Disk::appendBlock(C64DiskBlock *block) {
    this->Blocks.append(block);
}

C64DiskTrack *C64Disk::track(int i) {
    if (this->type() == C64Disk::DT_Unknown) throw new C64Exception("Unknown disk type.");
    int index = i - 1;
    if (index < 0 || index > this->Tracks.count() - 1) throw new C64Exception("Track index out of limits.");
    return this->Tracks.at(index);
}

void C64Disk::load(QString path) {
    this->Path = path;
    this->FileInfo = QFileInfo(this->Path);

    if (!this->FileInfo.exists()) throw new C64Exception("File \"" + this->Path + "\" does not exists.");
    else if (!this->FileInfo.isReadable()) throw new C64Exception("File \"" + this->Path + "\" is not readable.");
    else if (!this->FileInfo.isWritable()) throw new C64Exception("File \"" + this->Path + "\" is not writable.");

    switch(this->FileInfo.size()) {
        case 174848: // standard D64
        case 175531: // D64 with error info (which we just ignore)
            this->Type = C64Disk::DT_D64;
            break;
        case 349696:
            this->Type = C64Disk::DT_D71;
            break;
        case 819200:
            this->Type = C64Disk::DT_D81;
    }

    if (this->Type == C64Disk::DT_Unknown) throw new C64Exception("Cannot guess what type of disk image it is.");

    this->File = new QFile(this->Path);
    if (!this->File->open(QIODevice::ReadWrite)) throw new C64Exception("File \"" + this->Path + "\" could not be opened.");

    char *data;
    C64DiskTrack *track;
    qint64 len;
    if (this->Type == C64Disk::DT_D64) {
        for (int t = 1; t <= 35; t++) {
            if (t <= 17) {
                data = new char[SECTOR_SIZE * 21];
                len = this->File->read(data, SECTOR_SIZE * 21);
            } else if (t <= 24) {
                data = new char[SECTOR_SIZE * 19];
                len = this->File->read(data, SECTOR_SIZE * 19);
            } else if (t <= 30) {
                data = new char[SECTOR_SIZE * 18];
                len = this->File->read(data, SECTOR_SIZE * 18);
            } else if (t <= 35) {
                data = new char[SECTOR_SIZE * 17];
                len = this->File->read(data, SECTOR_SIZE * 17);
            }
            if (len == -1) throw C64Exception("Error while reading the image.");
            track = new C64DiskTrack(this, t, data);
            this->Tracks.append(track);
        }
    } else if (this->Type == C64Disk::DT_D71) {
        for (int t = 1; t <= 70; t++) {
            if (t <= 17) {
                data = new char[SECTOR_SIZE * 21];
                len = this->File->read(data, SECTOR_SIZE * 21);
            } else if (t <= 24) {
                data = new char[SECTOR_SIZE * 19];
                len = this->File->read(data, SECTOR_SIZE * 19);
            } else if (t <= 30) {
                data = new char[SECTOR_SIZE * 18];
                len = this->File->read(data, SECTOR_SIZE * 18);
            } else if (t <= 35) {
                data = new char[SECTOR_SIZE * 17];
                len = this->File->read(data, SECTOR_SIZE * 17);
            } else if (t <= 52) {
                data = new char[SECTOR_SIZE * 21];
                len = this->File->read(data, SECTOR_SIZE * 21);
            } else if (t <= 59) {
                data = new char[SECTOR_SIZE * 19];
                len = this->File->read(data, SECTOR_SIZE * 19);
            } else if (t <= 65) {
                data = new char[SECTOR_SIZE * 18];
                len = this->File->read(data, SECTOR_SIZE * 18);
            } else if (t <= 70) {
                data = new char[SECTOR_SIZE * 17];
                len = this->File->read(data, SECTOR_SIZE * 17);
            }
            if (len == -1) throw C64Exception("Error while reading the image.");
            track = new C64DiskTrack(this, t, data);
            this->Tracks.append(track);
        }
        if (!this->isDoubleSide()) throw C64Exception("The file is a d71 image in single-sided mode (1541 compatible). Only d71 images in double-sided mode are handled at present.");
    } else if (this->Type == C64Disk::DT_D81) {
        for (int t = 1; t <= 80; t++) {
            data = new char[SECTOR_SIZE * 40];
            len = this->File->read(data, SECTOR_SIZE * 40);
            if (len == -1) throw C64Exception("Error while reading the image.");
            track = new C64DiskTrack(this, t, data);
            this->Tracks.append(track);
        }
    }

    int n = 0;
    int size = 0;
    C64DiskDirEntries *des = this->track(this->directoryTrack())->sector(this->directorySector())->toDirEntries();
    do {
        for (int i = 0; i < 8; i++) {
            C64DirectoryFile *df = des->dirEntryAt(i);
            if (df->type() != C64DirectoryFile::FT_Unknown && df->type() != C64DirectoryFile::FT_DEL) {
                n++;
                size += df->size();
                emit this->newDirectoryFile(df);
            } else delete df;
        }
    } while ((des = des->next()) != NULL);
    emit loaded(this->FileInfo.fileName(), this->track(this->headerTrack())->sector(this->headerSector())->toHeader()->diskName(), n, size);

}

int C64Disk::headerTrack() {
    if (this->type() == C64Disk::DT_Unknown) throw new C64Exception("Unknown disk type.");
    if (this->type() == C64Disk::DT_D81) return 40;
    return 18;
}

int C64Disk::headerSector() {
    if (this->type() == C64Disk::DT_Unknown) throw new C64Exception("Unknown disk type.");
    return 0;
}

int C64Disk::bamTrack() {
    return headerTrack();
}

int C64Disk::bamSector() {
    if (this->type() == C64Disk::DT_Unknown) throw new C64Exception("Unknown disk type.");
    if (this->type() == C64Disk::DT_D81) return 1;
    return 0;
}

int C64Disk::directoryTrack() {
    return headerTrack();
}

int C64Disk::directorySector() {
    if (this->type() == C64Disk::DT_Unknown) throw new C64Exception("Unknown disk type.");
    if (this->type() == C64Disk::DT_D81) return 3;
    return 1;
}

QString C64Disk::name() {
    return this->track(this->headerTrack())->sector(this->headerSector())->toHeader()->diskName();
}

QString C64Disk::filename() {
    return this->FileInfo.fileName();
}

QString C64Disk::path() {
    return this->FileInfo.absolutePath();
}

QPixmap C64Disk::icon() {
    if (this->type() == C64Disk::DT_Unknown) throw new C64Exception("Unknown disk type.");
    if (this->type() == C64Disk::DT_D81) return QPixmap(":/icons/icon_FLOPPY_35.png");
    return QPixmap(":/icons/icon_FLOPPY_525.png");
}

QString C64Disk::ID() {
    return this->track(this->headerTrack())->sector(this->headerSector())->toHeader()->diskID();
}

bool C64Disk::isSoftLocked() {
    return this->track(this->headerTrack())->sector(this->headerSector())->toHeader()->isSoftWriteProteced();
}

bool C64Disk::isGeosDisk() {
    return this->track(this->headerTrack())->sector(this->headerSector())->toHeader()->isGeosDisk();
}

QString C64Disk::description() {
    if (this->type() == C64Disk::DT_Unknown) throw new C64Exception("Unknown disk type.");
    if (this->type() == C64Disk::DT_D64) return QString("5¼” floppy disk image for Commodore 1541 drive");
    else if (this->type() == C64Disk::DT_D71)  return QString("5¼” floppy disk image for Commodore 1571 drive");
    return QString("3½” floppy disk image for Commodore 1581 drive");
}

QString C64Disk::dosType() {
    return this->track(this->headerTrack())->sector(this->headerSector())->toHeader()->dosType();
}

QString C64Disk::dosVersion() {
    return this->track(this->headerTrack())->sector(this->headerSector())->toHeader()->dosVersion();
}

int C64Disk::tracksCount() {
    return this->Tracks.count();
}

int C64Disk::sectorsCount() {
    return this->Sectors.count();
}

int C64Disk::freeSectors() {
    int track = this->bamTrack();
    int sector = this->bamSector();
    int total = 0;
    while (track != 0) {
        C64DiskBAM *bam = this->track(track)->sector(sector)->toBAM();
        total = total + bam->freeSectors();
        if (this->type() == C64Disk::DT_D71 && track == 18 && track == 0) {
            // Yes free sectors on d71 are all on the header block
            track = 0;
            sector = 0xff;
        } else {
            track = bam->nextTrack();
            sector = bam->nextSector();
        }
    }
    return total;
}

void C64Disk::populateFreeSectorsMap(QMap <int, QMap<int, bool>> &fsm) {
    C64DiskBAM *bam = this->track(this->bamTrack())->sector(this->bamSector())->toBAM();
    bam->populateFreeSectorsMap(fsm);
    if (bam->nextTrack() != 0) {
        bam = this->track(bam->nextTrack())->sector(bam->nextSector())->toBAM();
        bam->populateFreeSectorsMap(fsm);
    }
}

QString C64Disk::geosID() {
    return this->track(this->headerTrack())->sector(this->headerSector())->toHeader()->geosIDString();
}

QByteArray C64Disk::headerData() {
    return this->track(this->headerTrack())->sector(this->headerSector())->toHeader()->data();
}

void C64Disk::directoryData(QMap<QString, DirectoryPage *> &pages) {
    C64DiskDirEntries *des = this->track(this->directoryTrack())->sector(this->directorySector())->toDirEntries();
    int i = 0;
    do {
        DirectoryPage *page = new DirectoryPage(des->track(), des->sector(), des->data(), i);
        pages.insert(page->label(), page);
        i++;
    } while ((des = des->next()) != NULL);
}

bool C64Disk::isReservedTrack(int track) {
    if (this->type() == C64Disk::DT_D64 && track == 18) return true;
    else if (this->type() == C64Disk::DT_D71 && (track == 18 || track == 53)) return true;
    else if (this->type() == C64Disk::DT_D81 && track == 40) return true;
    return false;
}

bool C64Disk::isDoubleSide() {
    return this->track(this->headerTrack())->sector(this->headerSector())->toHeader()->isDoubleSide();
}
