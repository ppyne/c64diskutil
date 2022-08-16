#ifndef DIRECTORYMODEL_H
#define DIRECTORYMODEL_H

#include <QAbstractItemModel>
#include <QList>
#include "c64directoryfile.h"

class DirectoryModel : public QAbstractItemModel {
    Q_OBJECT
public:
    enum Column {
        ColName = 0,
        ColType,
        ColOpened,
        ColLocked,
        ColSize,
        ColGeosFileType,
        ColGeosFileStruct,
        ColGeosDate,
        ColGeosClass,
        ColGeosAuthor,
        ColGeosApplication,
        ColGeosDescription,
        ColCount
    };

    explicit DirectoryModel(QObject *parent = nullptr);
    ~DirectoryModel();

    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public slots:
    void insertDirectoryFile(C64DirectoryFile *df);
    void reset();
private:
    QList<C64DirectoryFile *> Files;
};

#endif // DIRECTORYMODEL_H
