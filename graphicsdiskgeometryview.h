#ifndef GRAPHICSDISKGEOMETRYVIEW_H
#define GRAPHICSDISKGEOMETRYVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

class GraphicsDiskGeometryView : public QGraphicsView {
    Q_OBJECT
public:
    GraphicsDiskGeometryView(QWidget *parent = nullptr);
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GRAPHICSDISKGEOMETRYVIEW_H
