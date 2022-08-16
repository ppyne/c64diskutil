#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName("C64 Disk Utility");
    QCoreApplication::setOrganizationName("Stellar Crypt");
    QCoreApplication::setOrganizationDomain("stellarcrypt.com");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
