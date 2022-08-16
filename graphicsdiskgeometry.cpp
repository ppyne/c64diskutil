#include "graphicsdiskgeometry.h"
#include <QMapIterator>
#include <QFile>
#include <QMap>
#include <QMapIterator>

GraphicsDiskGeometry::GraphicsDiskGeometry(QGraphicsView *view, C64Disk *disk, bool sideB, QObject *parent) : QObject{parent} {
    this->View = view;

    this->Type = GraphicsDiskGeometry::GT_Floppy525;
    if (disk->type() == C64Disk::DT_D81) this->Type = GraphicsDiskGeometry::GT_Floppy35;

    this->View->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    this->Scene = new QGraphicsScene(this->View);
    this->Scene->setBackgroundBrush(Qt::white);
    this->View->setScene(this->Scene);
    this->Document = NULL;

    if (this->type() == GraphicsDiskGeometry::GT_Floppy525) this->ResourceName = ":/images/sectors_525.svg";
    else this->ResourceName = ":/images/sectors_35.svg";

    QFile file(this->ResourceName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open resource " + this->ResourceName;
        return;
    }
    this->Document = new QDomDocument();
    this->Document->setContent(&file);
    this->Paths = this->Document->elementsByTagName("path");

    int basetrack = 1;
    int lasttrack = 35;
    if (disk->type() == C64Disk::DT_D71) {
        if (sideB) {
            basetrack = 36;
            lasttrack = 70;
        }
    } else if (disk->type() == C64Disk::DT_D81) {
        if (sideB) {
            basetrack = 41;
            lasttrack = 80;
        } else lasttrack = 40;
    }
    int physicalindex = 1;

    //qDebug() << "basetrack" << basetrack;
    //qDebug() << "lasttrack" << lasttrack;

    QMap <int, QMap<int, bool>> map;
    disk->populateFreeSectorsMap(map);

    QMapIterator<int, QMap<int, bool>> itrack(map);
    while (itrack.hasNext()) {
        itrack.next();
        //qDebug() << "Track " << itrack.key();
        if (itrack.key() >= basetrack && itrack.key() <= lasttrack) {
            QMapIterator<int, bool> isector(itrack.value());
            while (isector.hasNext()) {
                isector.next();
                GraphicsSectorItem::SectorStatus status = GraphicsSectorItem::SS_Free;
                if (disk->isReservedTrack(physicalindex)) {
                    if (isector.value()) status = GraphicsSectorItem::SS_ReservedFree;
                    else status = GraphicsSectorItem::SS_ReservedUsed;
                } else {
                    if (isector.value()) status = GraphicsSectorItem::SS_Free;
                    else status = GraphicsSectorItem::SS_Used;
                }

                QString id("t" + QString::number(physicalindex) + "s" + QString::number(isector.key()));
                //qDebug() << QString(sideB?"side B":"side A") << "id" << id << "itrack" << itrack.key() << "isector" << isector.key();
                QPainterPath path = this->svgPathToShape(id);

                GraphicsSectorItem *item = new GraphicsSectorItem();
                this->Items[id] = item;
                item->setId(id);
                item->setPath(path);
                item->setTrack(itrack.key());
                item->setSector(isector.key());
                item->setStatus(status);
                item->setToolTip("Track " + QString::number(itrack.key()) + " Sector " + QString::number(isector.key()) + "\nStatus: " + GraphicsSectorItem::sectorStatusToString(status));
                QObject::connect(item, SIGNAL(onMouseEnterSector(QString, GraphicsSectorItem::SectorStatus, int, int)), this, SLOT(_onMouseEnterSector(QString, GraphicsSectorItem::SectorStatus, int, int)));
                QObject::connect(item, SIGNAL(onMouseLeaveSector(QString, GraphicsSectorItem::SectorStatus, int, int)), this, SLOT(_onMouseLeaveSector(QString, GraphicsSectorItem::SectorStatus, int, int)));
                this->Scene->addItem(item);
            }
            physicalindex++;
        }
        if (itrack.key() == lasttrack) break;
    }
}

GraphicsDiskGeometry::~GraphicsDiskGeometry() {
    if (this->Document != NULL) delete this->Document;
    this->Scene->clear();
    this->View->viewport()->update();
    /*QMapIterator<QString, GraphicsSectorItem *> i(this->Items);
    while (i.hasNext()) {
        i.next();
        delete i.value();
    }*/
    this->Items.clear();
}

GraphicsDiskGeometry::GeometryType GraphicsDiskGeometry::type() {
    return this->Type;
}

QPainterPath GraphicsDiskGeometry::svgPathToShape(QString id) {
    QString pathData;
    for (int i = 0; i < this->Paths.count(); i++) {
        QDomElement path = this->Paths.at(i).toElement();
        QDomAttr _id = path.attributeNode("id");
        if (_id.value() == id) {
            pathData = path.attributeNode("d").value();
            break;
        }
    }
    //qDebug() << pathData;

    pathData = pathData
               .replace(" ", "")
               .replace("-", ",-")
               .replace("M", ";M")
               .replace("l", ";l")
               .replace("L", ";L")
               .replace("h", ";h")
               .replace("H", ";H")
               .replace("v", ";v")
               .replace("V", ";V")
               .replace("c", ";c")
               .replace("C", ";C")
               .replace("s", ";s")
               .replace("S", ";S")
               .replace("q", ";q")
               .replace("Q", ";Q")
               .replace("t", ";t")
               .replace("T", ";T")
               .replace("a", ";a")
               .replace("A", ";A")
               .replace("z", ";z");

    QStringList parts = pathData.split(";", Qt::SkipEmptyParts);
    // qDebug() << parts;
    QPainterPath painterpath;
    painterpath.moveTo(0, 0);

    QPointF lastccommandparam2; // for S and s command, reflection calculation
    for (int i = 0; i < parts.count(); i++) {
        QString command = parts.at(i).first(1);
        QStringList params = parts.at(i).sliced(1).split(",", Qt::SkipEmptyParts);
        if (command == "M") {
            if (params.count() == 2) {
                qreal x, y;
                x = params.at(0).toDouble();
                y = params.at(1).toDouble();
                painterpath.moveTo(x, y);
                //qDebug() << command << x << y;
            }
        } else if (command == "L") {
            if (params.count() == 2) {
                qreal x, y;
                x = params.at(0).toDouble();
                y = params.at(1).toDouble();
                painterpath.lineTo(x, y);
                //qDebug() << command << x << y;
            }
        } else if (command == 'l') {
            if (params.count() == 2) {
                QPointF pos = painterpath.currentPosition();
                qreal x = pos.x(), y = pos.y();
                x += params.at(0).toDouble();
                y += params.at(1).toDouble();
                painterpath.lineTo(x, y);
                //qDebug() << command << x << y;
            }
        } else if (command == "V") {
            if (params.count() == 1) {
                QPointF pos = painterpath.currentPosition();
                qreal x, y;
                x = pos.x();
                y = params.at(0).toDouble();
                painterpath.lineTo(x, y);
                //qDebug() << command << x << y << id;
            }
        } else if (command == 'v') {
            if (params.count() == 1) {
                QPointF pos = painterpath.currentPosition();
                qreal x = pos.x(), y = pos.y();
                y += params.at(0).toDouble();
                painterpath.lineTo(x, y);
                //qDebug() << command << x << y << id;
            }
        } else if (command == 'c') {
            if (params.count() == 6) {
                QPointF pos = painterpath.currentPosition();
                qreal x1 = pos.x(), y1 = pos.y(), x2 = pos.x(), y2 = pos.y(), x = pos.x(), y = pos.y();
                x1 += params.at(0).toDouble();
                y1 += params.at(1).toDouble();
                x2 += params.at(2).toDouble();
                y2 += params.at(3).toDouble();
                lastccommandparam2 = QPointF(x2, y2);
                x += params.at(4).toDouble();
                y += params.at(5).toDouble();
                painterpath.cubicTo(x1, y1, x2, y2, x, y);
                //qDebug() << command << x1 << y1 << x2 << y2 << x << y;
            }
        } else if (command == 'C') {
            if (params.count() == 6) {
                qreal x1, y1, x2, y2, x, y;
                x1 = params.at(0).toDouble();
                y1 = params.at(1).toDouble();
                x2 = params.at(2).toDouble();
                y2 = params.at(3).toDouble();
                lastccommandparam2 = QPointF(x2, y2);
                x = params.at(4).toDouble();
                y = params.at(5).toDouble();
                painterpath.cubicTo(x1, y1, x2, y2, x, y);
                //qDebug() << command << x1 << y1 << x2 << y2 << x << y;
            }
        } else if (command == 's') {
            if (params.count() == 4) {
                QPointF pos = painterpath.currentPosition();
                qreal x1, y1, x2 = pos.x(), y2 = pos.y(), x = pos.x(), y = pos.y();
                x1 = pos.x() * 2 - lastccommandparam2.x(); // Reflection
                y1 = pos.y() * 2 - lastccommandparam2.y();
                x2 += params.at(0).toDouble();
                y2 += params.at(1).toDouble();
                lastccommandparam2 = QPointF(x2, y2);
                x += params.at(2).toDouble();
                y += params.at(3).toDouble();
                painterpath.cubicTo(x1, y1, x2, y2, x, y);
                //qDebug() << command << x1 << y1 << x2 << y2 << x << y << id;
            }
        } else if (command == 'S') {
            if (params.count() == 4) {
                QPointF pos = painterpath.currentPosition();
                qreal x1, y1, x2, y2, x, y;
                x1 = pos.x() * 2 - lastccommandparam2.x(); // Reflection
                y1 = pos.y() * 2 - lastccommandparam2.y();
                x2 = params.at(0).toDouble();
                y2 = params.at(1).toDouble();
                lastccommandparam2 = QPointF(x2, y2);
                x = params.at(2).toDouble();
                y = params.at(3).toDouble();
                painterpath.cubicTo(x1, y1, x2, y2, x, y);
                //qDebug() << command << x1 << y1 << x2 << y2 << x << y;
            }
        } else if (command == 'z') {
            painterpath.closeSubpath();
            //qDebug() << command ;
        } else
            qDebug() << "Warning: SVG command " << command << " on path id " << id << " not handled.";
    }
    return painterpath;
}

void GraphicsDiskGeometry::_onMouseEnterSector(QString id, GraphicsSectorItem::SectorStatus status, int track, int sector) {
    emit onMouseEnterSector(id, status, track, sector);
}

void GraphicsDiskGeometry::_onMouseLeaveSector(QString id, GraphicsSectorItem::SectorStatus status, int track, int sector) {
    emit onMouseLeaveSector(id, status, track, sector);
}
