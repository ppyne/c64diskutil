#ifndef C64DIRECTORYFILE_H
#define C64DIRECTORYFILE_H

#include <QString>
#include <QDateTime>
#include <QPixmap>
#include <QByteArray>
#include <QList>
#include <QMap>
#include "geosrecordpointer.h"
#include "c64diskgeosinfo.h"
#include "vlirdata.h"

class C64Disk;

class C64DirectoryFile {
    public:
        enum FileType { FT_Unknown, FT_DEL, FT_SEQ, FT_PRG, FT_USR, FT_REL, FT_CBM, FT_DIR };
        enum GeosFileType { GFT_NonGEOS, GFT_Basic, GFT_Assembly, GFT_Data, GFT_System,
                            GFT_DeskAccessory, GFT_Application, GFT_ApplicationData, GFT_Font,
                            GFT_Printer, GFT_InputDevice, GFT_Disk, GFT_Boot, GFT_Temp,
                            GFT_AutoExec, GFT_Unknown
                          };
        enum GeosFileStruct { GFS_Sequence, GFS_VLIR, GFS_Unknown };
        C64DirectoryFile(
            C64Disk *disk,
            int firsttrack,
            int firstsector,
            char *name,
            unsigned short int size,
            C64DirectoryFile::FileType type,
            bool closed,
            bool locked);
        C64DirectoryFile(
            C64Disk *disk,
            int firsttrack,
            int firstsector,
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
            QString geosdescription);
        ~C64DirectoryFile();
        QString name();
        int size();
        FileType type();
        QString typeString();
        bool locked();
        bool closed();
        QPixmap icon();
        bool isGeosFile();
        GeosFileType geosFileType();
        QString geosFileTypeString();
        QDateTime geosDate();
        GeosFileStruct geosFileStruct();
        QString geosFileStructString();
        bool hasGeosIcon();
        QPixmap geosIcon();
        QString geosClass();
        QString geosAuthor();
        QString geosApplication();
        QString geosDescription();
        QByteArray *rawData();
        void populateGeosRecords(QList<GeosRecordPointer> &records);
        void populateVLIRDataMap(QMap<QString, VLIRData *> &map);
        bool hasGeosBlockInfo();
        QByteArray geosInfoBlockData();
    protected:
        C64Disk *Disk;
        int FirstTrack;
        int FirstSector;
        QString Name;
        int Size;
        FileType Type;
        bool Locked;
        bool Closed;
        GeosFileType GeosFType;
        QDateTime GeosDate;
        GeosFileStruct GeosFStruct;
        QPixmap GeosIcon;
        QString GeosClass;
        QString GeosAuthor;
        QString GeosApplication;
        QString GeosDescription;
        QByteArray *RawData;
        QList<GeosRecordPointer> GeosRecords;
        C64DiskGeosInfo *GeosInfoBlock;
};

#endif // C64DIRECTORYFILE_H
