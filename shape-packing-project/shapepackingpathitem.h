#ifndef SHAPEPACKINGPATHITEM_H
#define SHAPEPACKINGPATHITEM_H

#include <QGraphicsPathItem>

class ShapePackingCanvas;

class ShapePackingPathItem : public QGraphicsPathItem
{
public:

    QPointF packedPos;
    int age = 0;
    int ID;
    QPointF startTLC; //starting coords of top left corner
    QPointF startPos;
    bool alive; //whether the rect fits in the screen or not

    ShapePackingCanvas* myCanvas;

    ShapePackingPathItem(ShapePackingCanvas* canvas); //the canvas is an argument in order to pass on canvas properties to the rect.
    virtual ~ShapePackingPathItem();

    QRectF rect();
    bool intersects(ShapePackingPathItem* path);
    bool isInsideCanvas();
};

#endif // SHAPEPACKINGPATHITEM_H
