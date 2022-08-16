#ifndef DIRECTORYPAGE_H
#define DIRECTORYPAGE_H

#include <QString>
#include <QByteArray>

class DirectoryPage {
    public:
        DirectoryPage(int track, int sector, QByteArray data, int index);
        QString &label();
        int track();
        int sector();
        QByteArray &data();
        int index();
    private:
        QString Label;
        int Track;
        int Sector;
        QByteArray Data;
        int Index;
};

#endif // DIRECTORYPAGE_H
