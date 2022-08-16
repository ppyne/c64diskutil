#include "graphicssectoritem.h"
#include <QCursor>

GraphicsSectorItem::GraphicsSectorItem(QObject *parent) : QObject(parent) {
    this->setAcceptHoverEvents(true);
    //this->setAcceptedMouseButtons(Qt::LeftButton|Qt::RightButton);
    //this->setFlag(QGraphicsPathItem::ItemIsSelectable, true);
    this->setCursor(QCursor(Qt::PointingHandCursor));
    this->Status = GraphicsSectorItem::SS_Undefined;
    this->Track = 0;
    this->Sector = 0xFF;
    this->setPen(GraphicsSectorItem::statusToPen());
    this->setBrush(GraphicsSectorItem::statusToBrush());
}

QPainterPath GraphicsSectorItem::path() const {
    return QGraphicsPathItem::path();
}

void GraphicsSectorItem::setPath(QPainterPath &path) {
    QGraphicsPathItem::setPath(path);
}

void GraphicsSectorItem::setStatus(GraphicsSectorItem::SectorStatus status) {
    this->Status = status;
    this->setPen(GraphicsSectorItem::statusToPen(status));
    this->setBrush(GraphicsSectorItem::statusToBrush(status));
}

GraphicsSectorItem::SectorStatus GraphicsSectorItem::status() {
    return this->Status;
}

void GraphicsSectorItem::setId(QString id) {
    this->Id = id;
}

QString GraphicsSectorItem::id() const {
    return this->Id;
}

void GraphicsSectorItem::setTrack(int id) {
    this->Track = id;
}

int GraphicsSectorItem::track() {
    return this->Track;
}

void GraphicsSectorItem::setSector(int id) {
    this->Sector = id;
}

int GraphicsSectorItem::sector() {
    return this->Sector;
}

QPen GraphicsSectorItem::statusToPen(SectorStatus status, bool selected) {
    Q_UNUSED(status);
    Q_UNUSED(selected);
    qreal width = 0.25;
    //if (selected) width = 0.5;
    return QPen(Qt::black, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
}

QBrush GraphicsSectorItem::statusToBrush(SectorStatus status, bool selected) {
    Q_UNUSED(status);
    switch (status) {
        case GraphicsSectorItem::SS_Free:
            if (selected) return QBrush(QColor(0, 153, 145));
            return QBrush(QColor(37, 180, 177));
        case GraphicsSectorItem::SS_Used:
            if (selected) return QBrush(QColor(229, 0, 81));
            return QBrush(QColor(236, 103, 122));
        case GraphicsSectorItem::SS_ReservedUsed:
            if (selected) return QBrush(QColor(234, 91, 12));
            return QBrush(QColor(242, 141, 79));
        case GraphicsSectorItem::SS_ReservedFree:
            if (selected) return QBrush(QColor(0, 105, 180));
            return QBrush(QColor(72, 140, 203));
        case GraphicsSectorItem::SS_Undefined:
        default:
            return QBrush();
    }
    return QBrush();
}

bool GraphicsSectorItem::sceneEvent(QEvent *ev) {
    if (ev->type() == QEvent::GraphicsSceneHoverEnter) {
        //qDebug() << ev->type() << this->id();
        this->setPen(GraphicsSectorItem::statusToPen(this->status(), true));
        this->setBrush(GraphicsSectorItem::statusToBrush(this->status(), true));
        emit onMouseEnterSector(this->id(), this->status(), this->track(), this->sector());
        return true;
    } else if (ev->type() == QEvent::GraphicsSceneHoverLeave) {
        //qDebug() << ev->type() << this->id();
        this->setPen(GraphicsSectorItem::statusToPen(this->status(), false));
        this->setBrush(GraphicsSectorItem::statusToBrush(this->status(), false));
        emit onMouseLeaveSector(this->id(), this->status(), this->track(), this->sector());
        return true;
    }
    return  QGraphicsPathItem::sceneEvent(ev);
}

QString GraphicsSectorItem::sectorStatusToString(GraphicsSectorItem::SectorStatus status) {
    switch (status) {
        case GraphicsSectorItem::SS_Free:
            return "free";
        case GraphicsSectorItem::SS_Used:
            return "used";
        case GraphicsSectorItem::SS_ReservedFree:
            return "reserved free";
        case GraphicsSectorItem::SS_ReservedUsed:
            return "reserved used";
        default:
        case GraphicsSectorItem::SS_Undefined:
            return "Undefined";
    }
    return "Undefined";
}
