#ifndef C64DISK_H
#define C64DISK_H

#include <QObject>
#include <QFileInfo>
#include <QFile>
#include <QList>
#include <QPixmap>
#include <QMap>
#include "c64disktrack.h"
#include "c64disksector.h"
#include "c64diskblock.h"
#include "c64directoryfile.h"
#include "directorypage.h"

class C64Disk : public QObject {
        Q_OBJECT
    public:
        explicit C64Disk(QObject *parent = nullptr);
        ~C64Disk();
        enum DiskType { DT_Unknown, DT_D64, DT_D71, DT_D81 };
        void load(QString path);
        void create(QString path, DiskType type);
        C64Disk::DiskType type();
        C64DiskTrack *track(int i);
        QString typeString();
        QString name();
        QString filename();
        QString path();
        QPixmap icon();
        QString ID();
        bool isSoftLocked();
        bool isGeosDisk();
        QString description();
        QString dosType();
        QString dosVersion();
        int tracksCount();
        int sectorsCount();
        int freeSectors();
        QString geosID();
        QByteArray headerData();
        void directoryData(QMap<QString, DirectoryPage *> &pages);
        bool isReservedTrack(int track);
        bool isDoubleSide();
        void populateFreeSectorsMap(QMap <int, QMap<int, bool>> &fsm);
    protected:
        void appendSector(C64DiskSector *sector);
        void appendBlock(C64DiskBlock *block);
        friend class C64DiskTrack;
        friend class C64DiskSector;
    private:
        QString Path;
        QFileInfo FileInfo;
        C64Disk::DiskType Type;
        QFile *File;
        QList<C64DiskTrack *> Tracks;
        QList<C64DiskSector *> Sectors;
        QList<C64DiskBlock *> Blocks;
        int headerTrack();
        int headerSector();
        int bamTrack();
        int bamSector();
        int directoryTrack();
        int directorySector();
    signals:
        void newDirectoryFile(C64DirectoryFile *df);
        void loaded(QString name, QString label, int count, int size);
};

#endif // C64DISK_H
