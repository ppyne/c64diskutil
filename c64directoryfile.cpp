#include "c64directoryfile.h"
#include "c64disk.h"
#include "c64diskgeosrecord.h"
#include <QDebug>

const char *FILE_TYPE_NAMES[] = {
    "???",
    "DEL",
    "SEQ",
    "PRG",
    "USR",
    "REL",
    "CBM",
    "DIR"
};

const char *GEOS_FILE_TYPE_NAMES[] = {
    "Non Geos",
    "Basic",
    "Assembly",
    "Data",
    "System",
    "Desk accessory",
    "Application",
    "Application data",
    "Font",
    "Printer",
    "Input device",
    "Disk",
    "System boot",
    "Temporary",
    "Auto exec",
    "Unknown"
};

const char *GEOS_FILE_STRUCT_NAMES[] = {
    "Sequence",
    "VLIR",
    "Unknown"
};

C64DirectoryFile::C64DirectoryFile(C64Disk *disk, int firsttrack, int firstsector, char *name, unsigned short int size, C64DirectoryFile::FileType type, bool closed, bool locked) {
    this->Disk = disk;
    this->FirstTrack = firsttrack;
    this->FirstSector = firstsector;
    this->Name = QString(name);
    this->Size = size;
    this->Type = type;
    this->Closed = closed;
    this->Locked = locked;
    this->GeosFType = GFT_NonGEOS;
    this->RawData = NULL;
}

C64DirectoryFile::C64DirectoryFile(
    C64Disk *disk,
    int firsttrack, int firstsector,
    char *name,
    unsigned short int size,
    C64DirectoryFile::FileType type,
    bool closed,
    bool locked,
    C64DiskGeosInfo *geosinfoblock,
    GeosFileType geosfiletype,
    QDateTime geosdate,
    GeosFileStruct geosfilestruct,
    QPixmap &geosicon,
    QString geosclass,
    QString geosauthor,
    QString geosapplication,
    QString geosdescription) {
    this->Disk = disk;
    this->FirstTrack = firsttrack;
    this->FirstSector = firstsector;
    this->Name = QString(name);
    this->Size = size;
    this->Type = type;
    this->Closed = closed;
    this->Locked = locked;
    this->GeosInfoBlock = geosinfoblock;
    this->GeosFType = geosfiletype;
    this->GeosDate = geosdate;
    this->GeosFStruct = geosfilestruct;
    this->GeosIcon = geosicon;
    this->GeosClass = geosclass;
    //qDebug() << "C64DirectoryFile() GeosClass" << this->GeosClass;
    this->GeosAuthor = geosauthor;
    //qDebug() << "C64DirectoryFile() GeosAuthor" << this->GeosAuthor;
    this->GeosApplication = geosapplication;
    //qDebug() << "C64DirectoryFile() GeosApplication" << this->GeosApplication;
    this->GeosDescription = geosdescription;
    //qDebug() << "C64DirectoryFile() GeosDescription" << this->GeosDescription;
    this->RawData = NULL;
    this->populateGeosRecords(this->GeosRecords);
}

C64DirectoryFile::~C64DirectoryFile() {
    GeosRecords.clear();
    if (this->RawData != NULL) delete this->RawData;
}

QString C64DirectoryFile::name() {
    return this->Name;
}

int C64DirectoryFile::size() {
    return this->Size;
}

C64DirectoryFile::FileType C64DirectoryFile::type() {
    return this->Type;
}

QString C64DirectoryFile::typeString() {
    return QString(FILE_TYPE_NAMES[this->type()]);
}

bool C64DirectoryFile::locked() {
    return this->Locked;
}

bool C64DirectoryFile::closed() {
    return this->Closed;
}

bool C64DirectoryFile::isGeosFile() {
    if (this->GeosFType == C64DirectoryFile::GFT_NonGEOS || this->GeosFType == C64DirectoryFile::GFT_Unknown) return false;
    return true;
}

C64DirectoryFile::GeosFileType C64DirectoryFile::geosFileType() {
    if (this->GeosFType >= C64DirectoryFile::GFT_Unknown) return C64DirectoryFile::GFT_Unknown;
    return this->GeosFType;
}

QString C64DirectoryFile::geosFileTypeString() {
    return GEOS_FILE_TYPE_NAMES[this->geosFileType()];
}

QDateTime C64DirectoryFile::geosDate() {
    return this->GeosDate;
}

C64DirectoryFile::GeosFileStruct C64DirectoryFile::geosFileStruct() {
    if (this->GeosFStruct >= C64DirectoryFile::GFS_Unknown) return C64DirectoryFile::GFS_Unknown;
    return this->GeosFStruct;
}

QString C64DirectoryFile::geosFileStructString() {
    return GEOS_FILE_STRUCT_NAMES[this->geosFileStruct()];
}

bool C64DirectoryFile::hasGeosIcon() {
    return !this->GeosIcon.isNull();
}

QPixmap C64DirectoryFile::geosIcon() {
    return this->GeosIcon;
}

QString C64DirectoryFile::geosClass() {
    return this->GeosClass;
}

QString C64DirectoryFile::geosAuthor() {
    return this->GeosAuthor;
}

QString C64DirectoryFile::geosApplication() {
    return this->GeosApplication;
}

QString C64DirectoryFile::geosDescription() {
    return this->GeosDescription;
}

QPixmap C64DirectoryFile::icon() {
    if (this->isGeosFile()) return this->geosIcon();
    switch (this->type()) {
        case C64DirectoryFile::FT_DEL:
            return QPixmap(":/icons/icon_DEL.png");
        case C64DirectoryFile::FT_SEQ:
            return QPixmap(":/icons/icon_SEQ.png");
        case C64DirectoryFile::FT_PRG:
            return QPixmap(":/icons/icon_PRG.png");
        case C64DirectoryFile::FT_USR:
            return QPixmap(":/icons/icon_USR.png");
        case C64DirectoryFile::FT_REL:
            return QPixmap(":/icons/icon_REL.png");
        case C64DirectoryFile::FT_CBM:
            return QPixmap(":/icons/icon_FOLDER_C64.png");
        case C64DirectoryFile::FT_DIR:
            return QPixmap(":/icons/icon_FOLDER.png");
        case C64DirectoryFile::FT_Unknown:
            return QPixmap(":/icons/icon_Unknown.png");
    }
    return QPixmap(":/icons/icon_Unknown.png");
}


QByteArray *C64DirectoryFile::rawData() {
    //qDebug() << "C64DirectoryFile::rawData()";
    if (this->RawData == NULL && this->type() != C64DirectoryFile::FT_DEL) {
        if ((!this->isGeosFile() && this->type() != FT_REL && this->type() != FT_CBM && this->type() != FT_DIR) ||
                (this->isGeosFile() && this->geosFileStruct() == GFS_Sequence)) {
            this->RawData = new QByteArray();
            int track = this->FirstTrack;
            int sector = this->FirstSector;
            do {
                C64DiskBlock *block = this->Disk->track(track)->sector(sector)->toBlock();
                int nexttrack = block->nextTrack();
                int nextsector = block->nextSector();
                int len = 254;
                if (nexttrack == 0) len = nextsector;
                //qDebug() << "Appending " << len << " bytes from track " << track << " sector " << sector;
                for (int i = 0; i < len; i++) this->RawData->append(block->rawData()[i]);
                track = nexttrack;
                sector = nextsector;
            } while (track != 0);

        }
    }
    return this->RawData;
}

void C64DirectoryFile::populateGeosRecords(QList<GeosRecordPointer> &records) {
    if (this->type() != C64DirectoryFile::FT_DEL && this->isGeosFile() && this->geosFileStruct() == C64DirectoryFile::GFS_VLIR)
        this->Disk->track(this->FirstTrack)->sector(this->FirstSector)->toGeosRecord()->populateRecords(records);
}

void C64DirectoryFile::populateVLIRDataMap(QMap<QString, VLIRData *> &map) {
    for (int i = 0; i < this->GeosRecords.count(); i++) {
        GeosRecordPointer p = GeosRecords.at(i);
        int track = p.track();
        int sector = p.sector();
        QByteArray data;
        do {
            C64DiskBlock *block = this->Disk->track(track)->sector(sector)->toBlock();
            int nexttrack = block->nextTrack();
            int nextsector = block->nextSector();
            int len = 254;
            if (nexttrack == 0) len = nextsector;
            //qDebug() << "Appending " << len << " bytes from track " << track << " sector " << sector;
            for (int i = 0; i < len; i++) data.append(block->rawData()[i]);
            track = nexttrack;
            sector = nextsector;
        } while (track != 0);
        VLIRData *d = new VLIRData(p, data, i);
        map.insert(d->label(), d);
    }
}

bool C64DirectoryFile::hasGeosBlockInfo() {
    if (this->isGeosFile() && this->GeosInfoBlock != NULL) return true;
    return false;
}

QByteArray C64DirectoryFile::geosInfoBlockData() {
    if (hasGeosBlockInfo()) return this->GeosInfoBlock->data();
    return QByteArray();
}
