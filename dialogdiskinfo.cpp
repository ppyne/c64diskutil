#include "dialogdiskinfo.h"
#include "ui_dialogdiskinfo.h"
#include <QMapIterator>

DialogDiskInfo::DialogDiskInfo(QWidget *parent) : QDialog(parent), ui(new Ui::DialogDiskInfo) {
    ui->setupUi(this);
    this->ui->checkBoxSoftLocked->setAttribute(Qt::WA_TransparentForMouseEvents);
    this->ui->checkBoxSoftLocked->setFocusPolicy(Qt::NoFocus);
    this->ui->checkBoxGeosDisk->setAttribute(Qt::WA_TransparentForMouseEvents);
    this->ui->checkBoxGeosDisk->setFocusPolicy(Qt::NoFocus);
    this->HexHeader.setParent(this);
    this->HexHeader.setReadOnly(true);
    this->ui->verticalLayoutTabHeader->addWidget(&this->HexHeader);
    this->HexDirectory.setParent(this);
    this->HexDirectory.setReadOnly(true);
    this->ui->verticalLayoutTabDirectory->insertWidget(1, &this->HexDirectory);
    this->GeomViewSideA = new GraphicsDiskGeometryView(this);
    this->GeomViewSideB = new GraphicsDiskGeometryView(this);
    this->ui->horizontalLayoutSideA->addWidget(this->GeomViewSideA);
    this->ui->horizontalLayoutSideB->addWidget(this->GeomViewSideB);
    this->GeomSideA = NULL;
    this->GeomSideB = NULL;
}

DialogDiskInfo::~DialogDiskInfo() {
    QMapIterator<QString, DirectoryPage *> idp(_DirectoryPages);
    while (idp.hasNext()) {
        idp.next();
        delete idp.value();
    }
    _DirectoryPages.clear();

    delete this->GeomViewSideA;
    delete this->GeomViewSideB;
    delete ui;
}

void DialogDiskInfo::setDiskInfo(C64Disk *disk) {
    ui->labelIcon->setPixmap(disk->icon());
    ui->lineEditName->setText(disk->name());
    ui->labelID->setText(disk->ID());
    if (disk->type() == C64Disk::DT_D71) {
        if (disk->isDoubleSide()) ui->labelType->setText(disk->typeString() + " double sided");
        else ui->labelType->setText(disk->typeString() + " single sided (C1541 compatible)");
    } else ui->labelType->setText(disk->typeString());
    ui->labelDescription->setText(disk->description());
    ui->labelDOS->setText("Type \"" + disk->dosType() + "\", version \"" + disk->dosVersion() + "\"");
    if (disk->isGeosDisk()) ui->checkBoxGeosDisk->setCheckState(Qt::Checked);
    else ui->checkBoxGeosDisk->setCheckState(Qt::Unchecked);
    if (disk->isSoftLocked()) ui->checkBoxSoftLocked->setCheckState(Qt::Checked);
    else ui->checkBoxSoftLocked->setCheckState(Qt::Unchecked);
    if (disk->isGeosDisk()) ui->labelGeosID->setText(disk->geosID());
    else ui->labelGeosID->setText("None");
    ui->labelGeometry->setText(QString::number(disk->tracksCount()) + " tracks, " + QString::number(disk->sectorsCount()) + " sectors");
    int fb = disk->freeSectors();
    ui->labelBlocks->setText(QString::number(fb) + " free block" + (fb > 1 ? "s on " : " on ") + QString::number(disk->sectorsCount()) + " blocks");
    ui->lineEditFilename->setText(disk->filename());
    ui->lineEditPath->setText(disk->path());
    if (!disk->isDoubleSide()) {
        this->GeomSideA = new GraphicsDiskGeometry(this->GeomViewSideA, disk);
        ui->tabBAMSides->removeTab(1);
    } else {
        this->GeomSideA = new GraphicsDiskGeometry(this->GeomViewSideA, disk);
        this->GeomSideB = new GraphicsDiskGeometry(this->GeomViewSideB, disk, true);
    }
    this->HexHeader.setData(disk->headerData());
    disk->directoryData(_DirectoryPages);
    QMapIterator<QString, DirectoryPage *> idp(_DirectoryPages);
    while (idp.hasNext()) {
        idp.next();
        ui->comboBoxDirectoryPages->addItem(idp.key(), idp.key());
    }
    if (!_DirectoryPages.empty()) on_comboBoxDirectoryPages_textActivated(_DirectoryPages.firstKey());
}

void DialogDiskInfo::on_comboBoxDirectoryPages_textActivated(const QString &arg1) {
    DirectoryPage *page = _DirectoryPages.value(arg1);
    this->HexDirectory.setData(page->data());
    ui->labelDirectorySize->setText("Block: " + QString::number(page->index() + 1) + "/" + QString::number(_DirectoryPages.count()));
}

