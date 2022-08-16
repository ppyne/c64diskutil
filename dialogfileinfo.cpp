#include "dialogfileinfo.h"
#include "ui_dialogfileinfo.h"
#include "utilities.cpp"
#include <QDebug>
#include <QPainter>
#include <QMapIterator>

DialogFileInfo::DialogFileInfo(QWidget *parent) : QDialog(parent), ui(new Ui::DialogFileInfo) {
    ui->setupUi(this);
    ui->checkBoxNotClosed->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->checkBoxNotClosed->setFocusPolicy(Qt::NoFocus);
    ui->checkBoxLocked->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->checkBoxLocked->setFocusPolicy(Qt::NoFocus);
    HexEditContent.setParent(this);
    HexEditContent.setReadOnly(true);
    ui->verticalLayoutHexFileContent->insertWidget(0, &HexEditContent);
    HexEditInfoBlock.setParent(this);
    HexEditInfoBlock.setReadOnly(true);
    ui->verticalLayoutInfoBlock->addWidget(&HexEditInfoBlock);
    HexEditVLIR.setParent(this);
    HexEditVLIR.setReadOnly(true);
    ui->verticalLayoutVLIR->insertWidget(1, &HexEditVLIR);
    _VLIRTotalSize = 0;
}

DialogFileInfo::~DialogFileInfo() {
    delete ui;
}


void DialogFileInfo::on_buttonBox_rejected() {
    qDebug() << "closed";
    close();
}

void DialogFileInfo::setFileInfo(C64DirectoryFile &df) {
    QPixmap pix = QPixmap(72, 63);
    pix.fill(Qt::white);
    QPainter painter = QPainter(&pix);
    painter.drawPixmap(0, -3, df.icon());
    painter.end();
    ui->Icon->setPixmap(pix);
    ui->lineEditName->setText(df.name());
    ui->labelType->setText(df.typeString());
    if (!df.closed()) ui->checkBoxNotClosed->setCheckState(Qt::Checked);
    else ui->checkBoxNotClosed->setCheckState(Qt::Unchecked);
    if (df.locked()) ui->checkBoxLocked->setCheckState(Qt::Checked);
    else ui->checkBoxLocked->setCheckState(Qt::Unchecked);
    ui->labelSize->setText(QString::number(df.size()) + " block" + (df.size() > 1 ? "s" : ""));
    ui->labelGeosFileType->setText(df.geosFileTypeString());
    if (df.isGeosFile()) {
        ui->labelGeosFileStruct->setText(df.geosFileStructString());
        ui->labelGeosDate->setText(df.geosDate().toString("yyyy-MM-dd hh:mm"));
        ui->labelGeosClass->setText(df.geosClass());
        ui->lineEditGeosAuthor->setText(df.geosAuthor());
        ui->labelGeosApplication->setText(df.geosApplication());
        ui->plainTextEdit->setPlainText(df.geosDescription());
    } else {
        ui->labelGeosFileStruct->setText("");
        ui->labelGeosDate->setText("");
        ui->labelGeosClass->setText("");
        ui->lineEditGeosAuthor->setText("");
        ui->labelGeosApplication->setText("");
        ui->plainTextEdit->setPlainText("");
    }
    if (!df.isGeosFile()) {
        ui->tabWidget->removeTab(3); // remove VLIR tab
        ui->tabWidget->removeTab(2); // remove info block tab
        if (df.type() != C64DirectoryFile::FT_REL && df.type() != C64DirectoryFile::FT_CBM && df.type() != C64DirectoryFile::FT_DIR ) {
            QByteArray *ba = df.rawData();
            if (ba != NULL) {
                HexEditContent.setData(*ba);
                if (ba->size() >= 1024)
                    ui->labelContentSize->setText("Size: " + humanSize(ba->size()) + " (" + QString::number(ba->size()) + " byte" + (ba->size() > 1 ? "s" : "") + ")");
                else
                    ui->labelContentSize->setText("Size: " + QString::number(ba->size()) + " byte" + (ba->size() > 1 ? "s" : ""));
            }
        } else ui->tabWidget->removeTab(1); // remove content tab
        removeGeosFields();
    } else {
        if (df.hasGeosBlockInfo()) HexEditInfoBlock.setData(df.geosInfoBlockData());
        if (df.geosFileStruct() == C64DirectoryFile::GFS_Sequence) {
            QByteArray *ba = df.rawData();
            if (ba != NULL) {
                HexEditContent.setData(*ba);
                if (ba->size() >= 1024)
                    ui->labelContentSize->setText("Size: " + humanSize(ba->size()) + " (" + QString::number(ba->size()) + " byte" + (ba->size() > 1 ? "s" : "") + ")");
                else
                    ui->labelContentSize->setText("Size: " + QString::number(ba->size()) + " byte" + (ba->size() > 1 ? "s" : ""));
            }
            ui->tabWidget->removeTab(3); // remove VLIR tab
        } else if (df.geosFileStruct() == C64DirectoryFile::GFS_VLIR) {
            df.populateVLIRDataMap(_VLIRDataMap);
            _VLIRTotalSize = 0;
            QMapIterator<QString, VLIRData *> ivd(_VLIRDataMap);
            while (ivd.hasNext()) {
                ivd.next();
                _VLIRTotalSize += ivd.value()->data().size();
                ui->comboBoxVLIRRecords->addItem(ivd.key(), ivd.key());
            }
            if (!_VLIRDataMap.empty()) on_comboBoxVLIRRecords_textActivated(_VLIRDataMap.firstKey());
            ui->tabWidget->removeTab(1); // remove content tab
        } else ui->tabWidget->removeTab(3); // remove VLIR tab
    }
}

void DialogFileInfo::removeGeosFields() {
    for (int i = ui->formLayout->rowCount() - 1; i >= 5; i--)
        ui->formLayout->removeRow(i);
}

void DialogFileInfo::on_comboBoxVLIRRecords_textActivated(const QString &arg1) {
    VLIRData *vlirdata = _VLIRDataMap.value(arg1);
    HexEditVLIR.setData(vlirdata->data());
    QString str = "Record: " + QString::number(vlirdata->index() + 1) + "/" + QString::number(_VLIRDataMap.count()) + ".";
    if (vlirdata->data().size() >= 1024)
        str += " Size: " + humanSize(vlirdata->data().size()) + " (" +  QString::number(vlirdata->data().size()) + " byte" + (vlirdata->data().size() > 1 ? "s" : "") + ").";
    else
        str += " Size: " +  QString::number(vlirdata->data().size()) + " byte" + (vlirdata->data().size() > 1 ? "s." : ".");
    if (_VLIRTotalSize >= 1024)
        str += " Total size: " + humanSize(_VLIRTotalSize) + " (" +  QString::number(_VLIRTotalSize) + " byte" + (_VLIRTotalSize > 1 ? "s" : "") + ").";
    else
        str += " Total size: " +  QString::number(_VLIRTotalSize) + " byte" + (_VLIRTotalSize > 1 ? "s." : ".");
    ui->labelVLIRSize->setText(str);
    ui->comboBoxVLIRRecords->setFocus();
}

