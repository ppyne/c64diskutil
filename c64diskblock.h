#ifndef C64DISKBLOCK_H
#define C64DISKBLOCK_H

#include <QString>

class C64DiskSector;
class C64DiskTrack;
class C64Disk;

class C64DiskBlock {
    public:
        C64DiskBlock(C64DiskSector *parent, char *data);
        C64DiskSector *parent();
        int nextTrack();
        int nextSector();
        char *rawData();
        static void removeA0Padding(char *str);
        static void petAsciiToAscii(char *str);
        static QString toHex(const char *raw, size_t len);
        static QString fromGeos(const char *raw, size_t len);
        QByteArray data();
        int track();
        int sector();
    protected:
        C64DiskSector *Parent;
        char *Data;
        C64Disk *Disk;
        C64DiskTrack *Track;
        C64DiskSector *Sector;
};

#endif // C64DISKBLOCK_H
