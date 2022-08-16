#ifndef GRAPHICSDISKGEOMETRY_H
#define GRAPHICSDISKGEOMETRY_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMap>
#include "graphicssectoritem.h"
#include <QDomDocument>
#include <QDomNodeList>
#include <QPainterPath>
#include "c64disk.h"

class GraphicsDiskGeometry : public QObject {
    Q_OBJECT
public:
    enum GeometryType { GT_Floppy525, GT_Floppy35 };
    GraphicsDiskGeometry(QGraphicsView *view, C64Disk *disk, bool sideB = false, QObject *parent = nullptr);
    ~GraphicsDiskGeometry();
    GeometryType type();
protected:
    QPainterPath svgPathToShape(QString id);
private:
    QMap <QString, GraphicsSectorItem *>Items;
    QGraphicsView *View;
    QGraphicsScene *Scene;
    GeometryType Type;
    QDomDocument *Document;
    QDomNodeList Paths;
    QString ResourceName;
private slots:
    void _onMouseEnterSector(QString id, GraphicsSectorItem::SectorStatus status, int track, int sector);
    void _onMouseLeaveSector(QString id, GraphicsSectorItem::SectorStatus status, int track, int sector);
signals:
    void onMouseEnterSector(QString id, GraphicsSectorItem::SectorStatus status, int track, int sector);
    void onMouseLeaveSector(QString id, GraphicsSectorItem::SectorStatus status, int track, int sector);
};

#endif // GRAPHICSDISKGEOMETRY_H
