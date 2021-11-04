#ifndef SHAPEPACKINGCANVAS_H
#define SHAPEPACKINGCANVAS_H

#include <QGraphicsView>
#include <QTimer>

class QGraphicsSimpleTextItem;
class ShapePackingCanvas : public QGraphicsView
{
    Q_OBJECT
public:
    ShapePackingCanvas();
    int mouse_x;
    int mouse_y;
    QPointF mousePos;
    QPointF mouseClickedPos;
    QGraphicsScene scene;
    QTimer timer;

    QList<QGraphicsPathItem*> paths;
    QList<QGraphicsPathItem*> movingPaths;

    QGraphicsPathItem *curPath;
    QGraphicsSimpleTextItem *coordsText;



protected:
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;

public slots:
    void timerUpdate();

};

#endif // SHAPEPACKINGCANVAS_H
