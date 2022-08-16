#ifndef GRAPHICSSECTORITEM_H
#define GRAPHICSSECTORITEM_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QEvent>

class GraphicsSectorItem : public QObject, public QGraphicsPathItem {
    Q_OBJECT
public:
    enum SectorStatus { SS_Undefined, SS_Free, SS_Used, SS_ReservedFree, SS_ReservedUsed };
    GraphicsSectorItem(QObject *parent = nullptr);
    QPainterPath path() const;
    void setPath(QPainterPath &path);
    void setStatus(SectorStatus status);
    SectorStatus status();
    void setId(QString id);
    QString id() const;
    void setTrack(int id);
    int track();
    void setSector(int id);
    int sector();
    static QString sectorStatusToString(SectorStatus status);
protected:
    bool sceneEvent(QEvent *ev) override;
private:
    static QPen statusToPen(SectorStatus status = SS_Undefined, bool selected = false);
    static QBrush statusToBrush(SectorStatus status = SS_Undefined, bool selected = false);
    SectorStatus Status;
    int Track;
    int Sector;
    QString Id;
    QPen Pen;
    QBrush Brush;
signals:
    void onMouseEnterSector(QString id, GraphicsSectorItem::SectorStatus status, int track, int sector);
    void onMouseLeaveSector(QString id, GraphicsSectorItem::SectorStatus status, int track, int sector);
};

#endif // GRAPHICSSECTORITEM_H
