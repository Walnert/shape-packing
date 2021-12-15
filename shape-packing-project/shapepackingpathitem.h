#ifndef SHAPEPACKINGPATHITEM_H
#define SHAPEPACKINGPATHITEM_H

#include <QGraphicsPathItem>

class ShapePackingPathItem : public QGraphicsPathItem
{
public:
    QPointF packedPos;
    int age = 0;
    int ID;
    QPointF startTLC; //starting coords of top left corner
    QPointF startPos;

    ShapePackingPathItem();
    virtual ~ShapePackingPathItem();

    QRectF rect();
    bool intersects(ShapePackingPathItem* path);
};

#endif // SHAPEPACKINGPATHITEM_H
