QT += core gui
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += sdk_no_version_check

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    c64directoryfile.cpp \
    c64disk.cpp \
    c64diskbam.cpp \
    c64diskblock.cpp \
    c64diskdirentries.cpp \
    c64diskgeosinfo.cpp \
    c64diskgeosrecord.cpp \
    c64diskheader.cpp \
    c64disksector.cpp \
    c64disktrack.cpp \
    c64exception.cpp \
    dialogdiskinfo.cpp \
    dialogfileinfo.cpp \
    directorymodel.cpp \
    directorypage.cpp \
    geosrecordpointer.cpp \
    graphicsdiskgeometry.cpp \
    graphicsdiskgeometryview.cpp \
    graphicssectoritem.cpp \
    main.cpp \
    mainwindow.cpp \
    qhexedit/chunks.cpp \
    qhexedit/commands.cpp \
    qhexedit/qhexedit.cpp \
    utilities.cpp \
    vlirdata.cpp

HEADERS += \
    c64directoryfile.h \
    c64disk.h \
    c64diskbam.h \
    c64diskblock.h \
    c64diskdirentries.h \
    c64diskgeosinfo.h \
    c64diskgeosrecord.h \
    c64diskheader.h \
    c64disksector.h \
    c64disktrack.h \
    c64exception.h \
    dialogdiskinfo.h \
    dialogfileinfo.h \
    directorymodel.h \
    directorypage.h \
    geosrecordpointer.h \
    graphicsdiskgeometry.h \
    graphicsdiskgeometryview.h \
    graphicssectoritem.h \
    mainwindow.h \
    qhexedit/chunks.h \
    qhexedit/commands.h \
    qhexedit/qhexedit.h \
    vlirdata.h

FORMS += \
    dialogdiskinfo.ui \
    dialogfileinfo.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
