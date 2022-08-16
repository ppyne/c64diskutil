#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QLayout>
#include "c64exception.h"
#include <QAbstractItemModel>
#include "dialogfileinfo.h"
#include "dialogdiskinfo.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    connect(QApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(aboutToQuit()));

    ui->setupUi(this);
    ui->action_info->setDisabled(true);

    this->Disk = NULL;

    this->Model = new DirectoryModel(this);
    this->SelectionModel = new QItemSelectionModel((QAbstractItemModel *)this->Model);
    this->ui->treeView->setModel((QAbstractItemModel *)this->Model);
    this->ui->treeView->setSelectionModel(this->SelectionModel);
    this->ui->treeView->setUniformRowHeights(true);
    this->ui->treeView->header()->setStretchLastSection(false);
    this->ui->treeView->setStyleSheet(
        "QTreeView { \
  background-color: whitesmoke; \
  color: black; \
  selection-background-color: #bfbeff; \
  selection-color: black; \
} \
QHeaderView { \
} \
QHeaderView::section { \
  background-color: silver; \
  color: black; \
  padding: 3px 7px; \
} \
");
    for (int i = 0; i < DirectoryModel::ColCount; i++) this->ui->treeView->header()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    connect(ui->treeView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onTreeViewDoubleClicked(const QModelIndex &)));
    connect(this->SelectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)), this, SLOT(onTrewViewSelectionChanged(const QItemSelection &, const QItemSelection &)));

    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry", saveGeometry()).toByteArray());
    restoreState(settings.value("mainWindowState", saveState()).toByteArray());
    this->LastOpenedDir = settings.value("lastOpenedDir", QDir::homePath()).toString();
    QStringList openedfiles = settings.value("lastOpenedFiles", QStringList()).toStringList();
    for (int i = openedfiles.count() - 1; i >= 0 ; i--) addRecentFile(openedfiles.at(i));
}

MainWindow::~MainWindow() {
    if (this->Disk != NULL) delete this->Disk;
    if (this->SelectionModel != NULL) delete this->SelectionModel;
    delete ui;
}

void  MainWindow::loaded(QString name, QString label, int count, int size) {
    this->setWindowTitle(name + " - " + QCoreApplication::applicationName());
    this->ui->statusbar->showMessage(
        QString(label.isEmpty() ? "" : "\"") + label + (label.isEmpty() ? "" : "\": ") +
        QString::number(count) + " file" + (count > 1 ? "s, " : ", ") +
        QString::number(size) + " block" + (size > 1 ? "s" : "") + "."
    );
}

void MainWindow::on_action_open_image_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "Open image", this->LastOpenedDir, "Disk Image Files (*.d64 *.d71 *.d81 *.D64 *.D71 *.D81)");
    if (!filename.isEmpty())
        loadImage(filename);
}

void MainWindow::on_action_close_image_triggered() {
    if (this->Disk != NULL) {
        delete this->Disk;
        this->Disk = NULL;
    }
    this->Model->reset();
    ui->statusbar->clearMessage();
    this->setWindowTitle(QCoreApplication::applicationName());
}


void MainWindow::onTreeViewDoubleClicked(const QModelIndex &index) {
    DialogFileInfo *dfi = new DialogFileInfo(this);
    C64DirectoryFile *df = (C64DirectoryFile *)index.internalPointer();
    dfi->setFileInfo(*df);
    dfi->show();
}

void MainWindow::on_action_quit_triggered() {
    QCoreApplication::quit();
}

void MainWindow::aboutToQuit() {
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());
    settings.setValue("lastOpenedDir", LastOpenedDir);
    settings.setValue("lastOpenedFiles", LastOpenedFiles);
    settings.sync();
}

void MainWindow::addRecentFile(QString path) {
    // Remove the path if already seen
    for (int i = 0; i < LastOpenedFiles.count(); i++) {
        if (LastOpenedFiles.at(i) == path) LastOpenedFiles.removeAt(i);
    }
    // Place it to the top of the list
    LastOpenedFiles.prepend(path);
    // Keep only 5 items
    while (this->LastOpenedFiles.count() > 5) LastOpenedFiles.removeLast();
    // Clear the menu
    ui->menu_open_recent->clear();
    // Recreate the menu
    const Qt::Key keys[] = {
        Qt::Key_1,
        Qt::Key_2,
        Qt::Key_3,
        Qt::Key_4,
        Qt::Key_5
    };
    for (int i = 0; i < LastOpenedFiles.count(); i++) {
        QFileInfo fi(LastOpenedFiles.at(i));
        QAction *a = new QAction(fi.fileName(), this);
        a->setData(LastOpenedFiles.at(i));
        a->setShortcut(QKeySequence(Qt::CTRL | keys[i]));
        ui->menu_open_recent->addAction(a);
        connect(a, SIGNAL(triggered()), this, SLOT(onRecentFile()));
    }
}

void MainWindow::onRecentFile() {
    QAction *action = qobject_cast<QAction *>(sender());
    loadImage(action->data().toString());
}

void MainWindow::loadImage(QString filename) {
    addRecentFile(filename);
    QFileInfo fi = QFileInfo(filename);
    this->LastOpenedDir = fi.absolutePath();
    try {
        if (this->Disk != NULL) {
            delete this->Disk;
            this->Disk = NULL;
        }
        this->Model->reset();
        ui->statusbar->clearMessage();
        this->setWindowTitle(QCoreApplication::applicationName());
        this->Disk = new C64Disk();
        connect(this->Disk, SIGNAL(newDirectoryFile(C64DirectoryFile *)), this->Model, SLOT(insertDirectoryFile(C64DirectoryFile *)));
        connect(this->Disk, SIGNAL(loaded(QString, QString, int, int)), this, SLOT(loaded(QString, QString, int, int)));
        this->Disk->load(filename);
    } catch (C64Exception *e) {
        QMessageBox::critical(this, "Error", e->message());
        delete e;
    } catch (...) {
        QMessageBox::critical(this, "Error", "Unknown exception");
    }
}

void MainWindow::on_action_info_triggered() {
    QModelIndexList indexes = this->SelectionModel->selectedIndexes();
    if (indexes.count()) {
        DialogFileInfo *dfi = new DialogFileInfo(this);
        C64DirectoryFile *df = (C64DirectoryFile *)indexes.at(0).internalPointer();
        dfi->setFileInfo(*df);
        dfi->show();
    }
}

void MainWindow::on_action_disk_information_triggered() {
    if (this->Disk != NULL) {
        DialogDiskInfo *ddi = new DialogDiskInfo(this);
        ddi->setDiskInfo(this->Disk);
        ddi->show();
    }
}

void MainWindow::onTrewViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    Q_UNUSED(deselected);
    if (selected.count()) this->ui->action_info->setEnabled(true);
    else this->ui->action_info->setDisabled(true);
}

