#include "shapepackingpathitem.h"
#include <QDebug>

ShapePackingPathItem::ShapePackingPathItem()
{
    age = 0;
    packedPos = QPointF(69,69);
}

ShapePackingPathItem::~ShapePackingPathItem() {

}

QRectF ShapePackingPathItem::rect() {
    return this->shape().boundingRect();
}

bool ShapePackingPathItem::intersects(ShapePackingPathItem *path) {
    int ax1 = path->packedPos.x();
    int ax2 = ax1 + path->rect().width();
    int ay1 = path->packedPos.y();
    int ay2 = ay1 + path->rect().height();

    //qDebug() << ax1 << ay1 << ax2 << ay2;

    int bx1 = this->packedPos.x();
    int bx2 = bx1 + this->rect().width();
    int by1 = this->packedPos.y();
    int by2 = by1 + this->rect().height();

    //qDebug() << bx1 << by1 << bx2 << by2;

    bool intersects_x;
    bool intersects_y;


    intersects_y = !(ay2 <= by1 || by2 <= ay1);
    intersects_x = !(ax2 <= bx2 || bx2 <= ax1);


    return (intersects_x && intersects_y);
}
