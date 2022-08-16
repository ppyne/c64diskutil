#ifndef DIALOGDISKINFO_H
#define DIALOGDISKINFO_H

#include <QDialog>
#include <QMap>
#include "qhexedit/qhexedit.h"
#include "c64disk.h"
#include "graphicsdiskgeometryview.h"
#include "graphicsdiskgeometry.h"
#include "directorypage.h"

namespace Ui {
    class DialogDiskInfo;
}

class DialogDiskInfo : public QDialog {
        Q_OBJECT
    public:
        explicit DialogDiskInfo(QWidget *parent = nullptr);
        ~DialogDiskInfo();
        void setDiskInfo(C64Disk *disk);
    private slots:
        void on_comboBoxDirectoryPages_textActivated(const QString &arg1);

    private:
        Ui::DialogDiskInfo *ui;
        QHexEdit HexHeader;
        QHexEdit HexDirectory;
        GraphicsDiskGeometryView *GeomViewSideA;
        GraphicsDiskGeometryView *GeomViewSideB;
        GraphicsDiskGeometry *GeomSideA;
        GraphicsDiskGeometry *GeomSideB;
        QMap<QString, DirectoryPage *> _DirectoryPages;
};

#endif // DIALOGDISKINFO_H
