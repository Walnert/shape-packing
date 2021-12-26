#include "shapepackingpathitem.h"
#include "shapepackingcanvas.h"
#include <QDebug>

ShapePackingPathItem::ShapePackingPathItem(ShapePackingCanvas* canvas) {
    packedPos = QPointF(400,400); //placeholder position, does not get used unless there's a bug
    myCanvas = canvas; //sets up a pointer to the canvas in order to access the canvas's dimensions
}

ShapePackingPathItem::~ShapePackingPathItem() {}

QRectF ShapePackingPathItem::rect() { //helper function to shorten frequent commands
    return this->shape().boundingRect();
}

bool ShapePackingPathItem::intersects(ShapePackingPathItem *path) {
    int ax1 = path->packedPos.x();
    int ax2 = ax1 + path->rect().width();
    int ay1 = path->packedPos.y();
    int ay2 = ay1 + path->rect().height();

    int bx1 = this->packedPos.x();
    int bx2 = bx1 + this->rect().width();
    int by1 = this->packedPos.y();
    int by2 = by1 + this->rect().height();

    // if it's NOT overlapping, then return false. Otherwise, return true.

    if (ay2 <= by1 || by2 <= ay1) return false;
    if (ax2 <= bx1 || bx2 <= ax1) return false;

    return true;
}

bool ShapePackingPathItem::isInsideCanvas() {
    int x1 = this->packedPos.x();
    int x2 = x1 + this->rect().width();
    int y1 = this->packedPos.y();
    int y2 = y1 + this->rect().height();

    int width = myCanvas->width();
    int height = myCanvas->height();

    //returns true as long as the bottom-right corner of the rectangle is inside the canvas.

    if (x2 < width && y2 <= height) return true;
    return false;
}
