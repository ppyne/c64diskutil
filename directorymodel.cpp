#include "directorymodel.h"
#include <QPixmap>
#include <QDebug>

DirectoryModel::DirectoryModel(QObject *parent) : QAbstractItemModel(parent) {
}
DirectoryModel::~DirectoryModel() {
    while (!this->Files.isEmpty()) delete this->Files.takeFirst();
}

QVariant DirectoryModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
            case ColName:
                return QVariant("Name");
            case ColType:
                return QVariant("Type");
            case ColOpened:
                return QVariant("Not closed");
            case ColLocked:
                return QVariant("Locked");
            case ColSize:
                return QVariant("Size");
            case ColGeosFileStruct:
                return QVariant("Geos file struct");
            case ColGeosFileType:
                return QVariant("Geos file type");
            case ColGeosDate:
                return QVariant("Geos date");
            case ColGeosClass:
                return QVariant("Geos class");
            case ColGeosAuthor:
                return QVariant("Geos author");
            case ColGeosApplication:
                return QVariant("Geos application");
            case ColGeosDescription:
                return QVariant("Geos description");
        }
    }
    if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }
    return QVariant();
}

QModelIndex DirectoryModel::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(column);
    Q_UNUSED(parent);
    return createIndex(row, column, (void *)this->Files.at(row));
}

QModelIndex DirectoryModel::parent(const QModelIndex &index) const {
    Q_UNUSED(index)
    return QModelIndex();
}

int DirectoryModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return this->Files.count();
}

int DirectoryModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return ColCount;
}

QVariant DirectoryModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case ColName:
                return this->Files.at(index.row())->name();
            case ColType:
                return this->Files.at(index.row())->typeString();
            case ColSize:
                return QVariant(this->Files.at(index.row())->size());
            case ColGeosFileType:
                return this->Files.at(index.row())->geosFileTypeString();
        }
        if (this->Files.at(index.row())->isGeosFile()) {
            switch (index.column()) {
                case ColGeosFileStruct:
                    return this->Files.at(index.row())->geosFileStructString();
                case ColGeosDate:
                    return this->Files.at(index.row())->geosDate().toString("yyyy-MM-dd hh:mm");
                case ColGeosClass:
                    return this->Files.at(index.row())->geosClass();
                case ColGeosAuthor:
                    return this->Files.at(index.row())->geosAuthor();
                case ColGeosApplication:
                    return this->Files.at(index.row())->geosApplication();
                case ColGeosDescription:
                    return this->Files.at(index.row())->geosDescription();
            }
        }
    } else if (role == Qt::CheckStateRole) {
        switch (index.column()) {
            case ColOpened:
                return this->Files.at(index.row())->closed()?0:2;
            case ColLocked:
                return this->Files.at(index.row())->locked()?2:0;
        }
    } else if (role == Qt::TextAlignmentRole) {
        switch (index.column()) {
            case ColName:
            case ColGeosClass:
            case ColGeosAuthor:
            case ColGeosApplication:
            case ColGeosDescription:
                return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
            case ColType:
            case ColOpened:
            case ColLocked:
            case ColGeosFileStruct:
            case ColGeosFileType:
            case ColGeosDate:
                return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
            case ColSize:
                return QVariant(Qt::AlignRight | Qt::AlignVCenter);
        }
    } else if (role == Qt::DecorationRole) {
        if (index.column() == ColName) return this->Files.at(index.row())->icon().scaled(24,24);
    }
    return QVariant();
}

void DirectoryModel::insertDirectoryFile(C64DirectoryFile *df) {
    beginInsertRows(QModelIndex(), this->Files.count(), this->Files.count());
    this->Files.append(df);
    endInsertRows();
}

void DirectoryModel::reset() {
    beginResetModel();
    while (!this->Files.isEmpty()) delete this->Files.takeFirst();
    endResetModel();
}

