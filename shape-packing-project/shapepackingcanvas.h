#ifndef SHAPEPACKINGCANVAS_H
#define SHAPEPACKINGCANVAS_H

#include <QGraphicsView>
#include <QTimer>
#include <vector>
#include <QSet>
#include <QMultiMap>

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
    int framerate;

    QList<ShapePackingPathItem*> paths;
    QList<ShapePackingPathItem*> movingPaths;
    QList<ShapePackingPathItem*> packedPaths;
    QMultiMap<int, QPointF> corners;
//    QList<QPointF> packedPositions;
    ShapePackingPathItem *curPath;

    QList<QGraphicsSimpleTextItem*> coordsTexts;
    QGraphicsSimpleTextItem *curCoordsText;

    void addNewRect(int w, int h);
    void clear();

    void pack2D(QList<ShapePackingPathItem*> paths);
    void onlinePack(QList<ShapePackingPathItem*> packedPaths, ShapePackingPathItem* curPath);
    void onlinePack2(QList<ShapePackingPathItem*> packedPaths, ShapePackingPathItem* curPath);



protected:
    void mouseMoveEvent(QMouseEvent *evt) override;
    void mousePressEvent(QMouseEvent *evt) override;
    void mouseReleaseEvent(QMouseEvent *evt) override;



private:
    QPointF normalized(const QPointF point);
    int t = 0;

public slots:
    void timerUpdate();

};

#endif // SHAPEPACKINGCANVAS_H
