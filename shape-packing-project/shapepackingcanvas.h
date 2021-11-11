#ifndef SHAPEPACKINGCANVAS_H
#define SHAPEPACKINGCANVAS_H

#include <QGraphicsView>
#include <QTimer>

class QGraphicsSimpleTextItem;
class ShapePackingPathItem;
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

    QList<ShapePackingPathItem*> paths;
    QList<ShapePackingPathItem*> movingPaths;
//    QList<QPointF> packedPositions;
    ShapePackingPathItem *curPath;

    QList<QGraphicsSimpleTextItem*> coordsTexts;
    QGraphicsSimpleTextItem *curCoordsText;



protected:
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;

    void pack2D(QList<ShapePackingPathItem*> paths);

private:
    QPointF normalized(const QPointF point);

public slots:
    void timerUpdate();

};

#endif // SHAPEPACKINGCANVAS_H
