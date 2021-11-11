#ifndef SHAPEPACKINGPATHITEM_H
#define SHAPEPACKINGPATHITEM_H

#include <QGraphicsPathItem>

class ShapePackingPathItem : public QGraphicsPathItem
{
public:
    QPointF packedPos;
    int age = 0;

    ShapePackingPathItem();
    virtual ~ShapePackingPathItem();
};

#endif // SHAPEPACKINGPATHITEM_H
