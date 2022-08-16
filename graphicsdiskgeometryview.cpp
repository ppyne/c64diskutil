#include "graphicsdiskgeometryview.h"

GraphicsDiskGeometryView::GraphicsDiskGeometryView(QWidget *parent) : QGraphicsView(parent) {

}

void GraphicsDiskGeometryView::resizeEvent(QResizeEvent *event) {
    const qreal margin = -5.0;
    QMargins margins(margin, margin, margin, margin);
    QSizeF d(sceneRect().width(), sceneRect().height());
    QSizeF b(viewport()->size().grownBy(margins));
    qreal r = 1.0;
    if (d.width() / d.height() > b.width() / b.height()) {
      r = b.width() / d.width();
    } else {
      r = b.height() / d.height();
    }
    QTransform t;
    t.scale(r, r);
    setTransform(t);
    QGraphicsView::resizeEvent(event);
}
