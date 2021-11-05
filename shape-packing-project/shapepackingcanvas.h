#ifndef SHAPEPACKINGCANVAS_H
#define SHAPEPACKINGCANVAS_H

#include <QGraphicsView>
#include <QTimer>

class QGraphicsSimpleTextItem;
class QGraphicsPathItem;
class ShapePackingCanvas : public QGraphicsView
{
    Q_OBJECT
public:
    ShapePackingCanvas();
    QPointF mousePos;
    QPointF mouseClickedPos;
    QGraphicsScene scene;
    QTimer timer;

    float pathAnimSpeed;
    int mode;

    QList<QGraphicsPathItem*> paths;
    QList<QGraphicsPathItem*> movingPaths;

    QGraphicsPathItem *curPath;
    QGraphicsSimpleTextItem *coordsText;



protected:
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;

private:
    QPointF normalized(const QPointF point);

public slots:
    void timerUpdate();

};

#endif // SHAPEPACKINGCANVAS_H
