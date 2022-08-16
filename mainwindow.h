#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelectionModel>
#include "c64disk.h"
#include "directorymodel.h"
#include <QStringList>
#include <QAction>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void aboutToQuit();
private slots:
    void loaded(QString name, QString label, int count, int size);
    void onTreeViewDoubleClicked(const QModelIndex &index);
    void onRecentFile();
    void onTrewViewSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void on_action_open_image_triggered();
    void on_action_close_image_triggered();
    void on_action_quit_triggered();
    void on_action_info_triggered();
    void on_action_disk_information_triggered();

private:
    Ui::MainWindow *ui;
    C64Disk *Disk;
    QString LastOpenedDir;
    QStringList LastOpenedFiles;
    void addRecentFile(QString path);
    DirectoryModel *Model;
    QItemSelectionModel *SelectionModel;
    void loadImage(QString filename);
};
#endif // MAINWINDOW_H
