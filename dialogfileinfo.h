#ifndef DIALOGFILEINFO_H
#define DIALOGFILEINFO_H

#include <QDialog>
#include "c64directoryfile.h"
#include "qhexedit/qhexedit.h"
#include <QMap>
#include "vlirdata.h"

namespace Ui {
    class DialogFileInfo;
}

class DialogFileInfo : public QDialog {
        Q_OBJECT
    public:
        explicit DialogFileInfo(QWidget *parent = nullptr);
        ~DialogFileInfo();
        void setFileInfo(C64DirectoryFile &df);
    private slots:
        void on_buttonBox_rejected();
        void on_comboBoxVLIRRecords_textActivated(const QString &arg1);
    private:
        Ui::DialogFileInfo *ui;
        QHexEdit HexEditContent;
        QHexEdit HexEditInfoBlock;
        QHexEdit HexEditVLIR;
        void removeGeosFields();
        QMap<QString, VLIRData *> _VLIRDataMap;
        int _VLIRTotalSize;
};

#endif // DIALOGFILEINFO_H
