#include "shapepackingcanvas.h"
#include "shapepackingpathitem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QtWidgets>
#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <math.h>

ShapePackingCanvas::ShapePackingCanvas() {
    setBackgroundBrush(QBrush(QColor(25,25,25), Qt::SolidPattern)); //dark background
    curPath = nullptr;
    corners.insert(0, QPointF(0,0)); //starts packing at top left corner of canvas

    setScene(&scene);
    setRenderHint(QPainter::Antialiasing);
    setSceneRect(-canvasWidth/2, -canvasHeight/2, canvasWidth, canvasHeight); //positions canvas in the middle of the screen based on canvas size

    animLength = 30;
    framerate = 60;
    mode = 1; //mode 1 = pack the rectangles, mode 2 = the rectangles just sit there where you drew them.
    // (mode 2 used for debugging collision scripts)

    timer.setInterval(1000/framerate);
    connect(&timer, &QTimer::timeout, this, &ShapePackingCanvas::timerUpdate);
    timer.start();
}

QPointF getSizePointF(ShapePackingPathItem *path) {
    return QPointF(path->rect().width(),
                   path->rect().height());
}

void ShapePackingCanvas::mousePressEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos()); //records mouse pos
    mouseClickedPos = pos;

    //creates a new path and sets it as the current path
    QPainterPath path;
    path.addRect(QRectF(pos,QSizeF(0,0)));
    curPath = new ShapePackingPathItem(this);
    curPath->setPath(path);
    curPath->setPen(QPen(Qt::yellow, 3.0));
    scene.addItem(curPath);
    paths.push_back(curPath);

    //draws width x height text near cursor
    curCoordsText = scene.addSimpleText("0\n0");
    curCoordsText->setPos(pos + QPointF(20,0));
    curCoordsText->setBrush(Qt::white);
}

void ShapePackingCanvas::mouseMoveEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos());
    if (curPath == nullptr) return;

    //draws a dynamic "draft" rectangle that changes size with the cursor's movements
    QPainterPath newPath;
    newPath.addRect(QRectF(mouseClickedPos, pos));
    curPath->setPath(newPath);

    //updates coords text
    curCoordsText->setText(QString::number(getSizePointF(curPath).x()) + "\n" +
                        QString::number(getSizePointF(curPath).y()));
    curCoordsText->setPos(pos + QPointF(20,0));
}


void ShapePackingCanvas::mouseReleaseEvent(QMouseEvent *evt) {
    //records start position information about the final rectangle
    curPath->startTLC = mapFromScene(curPath->pos() + curPath->rect().topLeft());
    curPath->startPos = curPath->pos();
    curPath->setBrush(QColor(0,0,255,100));
    curPath->setPen(QPen(Qt::lightGray, 3.0));

    if (mode==1) {
        // if it's able to pack it successfully, set it to "alive" and add it to the list of packed paths.
        if (onlinePack2(packedPaths, curPath)) {
            packedPaths.push_back(curPath);
            curPath->alive = true;
        // else, the path is "dead" and will fade away during the animation.
        } else curPath->alive = false;

    } else if (mode==2) {
        curPath->packedPos = curPath->startTLC;
        packedPaths.push_back(curPath);
    }

    coordsTexts.append(curCoordsText);
    movingPaths.push_back(curPath); //adds path, dead or alive, to movingPaths list.
}

void ShapePackingCanvas::timerUpdate() {
    //animates coords text to drift upwards and fade away once the mouse is released.
    for (QGraphicsSimpleTextItem* coordsText : coordsTexts) {
        coordsText->setOpacity(coordsText->opacity()-.02);
        coordsText->moveBy(0,-.2);
        if (coordsText->opacity() < .02) {
            scene.removeItem(coordsText);
            coordsTexts.removeOne(coordsText);
        }
    }
    if (mode==1) {
        for (ShapePackingPathItem* path : movingPaths) {
            //if the path is alive, moves the path linearly from its starting position to its packed position.
            //animation is a function of the rectangle's age (in frames).
            if (path->alive) {
                ++path->age;
                QPointF delta = path->packedPos - path->startTLC;
                path->setPos(path->startPos + delta*path->age/animLength);
                if (path->age >= animLength) { //when the path moves into its final position
                    path->setBrush(QColor(255,0,0,100)); //turn red
                    movingPaths.removeOne(path);
                }
            //if the path is dead, it turns gray and drifts off to heaven. (That's how I thought of it)
            } else {
                path->setBrush(QColor(100,100,100,100));
                path->moveBy(0, -2);
                path->setScale(path->scale()-0.002);
                path->setOpacity(path->opacity()-0.02);
                if (path->opacity() < 0.02) {
                    scene.removeItem(path);
                    movingPaths.removeOne(path);
                }
            }
        }
    } else if (mode==2) {
    return;
    }
}

bool ShapePackingCanvas::onlinePack2(QList<ShapePackingPathItem *> packedPaths, ShapePackingPathItem* curPath) {
    //qDebug() << "Corners BEFORE: " << corners;
    bool ok; //bool ok signals whether the path would fit in a particular location.
    for (auto icorner = corners.begin() ; icorner != corners.end() ; ++icorner) {
        QPointF &corner = *icorner;
        //qDebug() << "trying " << corner;
        curPath->packedPos = corner; //"tries on" the corner by temporarily setting the corner as its packed position.
        ok = true;
        if (!curPath->isInsideCanvas()) { //makes sure the rectangle fits inside the canvas box.
            ok = false;
            continue;
        }
        for (ShapePackingPathItem* path : packedPaths) {
            //qDebug() << "trying path " << path->startTLC;
            if (curPath->intersects(path)) {
                //qDebug() << "failed " << path->startTLC;
                ok = false;
                break;
            }
        }
        if (ok) {
            corners.erase(icorner); //the successful corner is removed from the list of eligible corners.

            //creates two new eligible corners at the top-right and bottom-left corners of the just-packed rectangle.
            QPointF corner1 = curPath->packedPos + QPointF(curPath->rect().width(),0);
            QPointF corner2 = curPath->packedPos + QPointF(0,curPath->rect().height());

            // the corners are organized in a QMultiMap. I weighted the keys so that the shape of the growing mass of rectangles
            // would approximate a square shape. If you try packing many small pixel-like rectangles, you'll see that the shape they
            // make is in fact a rounded square shape.

            corners.insert(hypot(corner1.x(), corner1.y()) - 0.2* corner1.manhattanLength(), corner1);
            corners.insert(hypot(corner2.x(), corner2.y()) - 0.2* corner2.manhattanLength(), corner2);
            //qDebug() << "Corners AFTER: " << corners;
            break;
        }
    }
    if (!ok) {
        paths.removeOne(curPath);
    }
    return ok; //this "ok" value that gets returned dictates whether the rectangle is dead or alive.
}

void ShapePackingCanvas::addNewRect(int w, int h) {
    curPath = new ShapePackingPathItem(this);
    curPath->setPen(QPen(Qt::yellow, 3.0));
    scene.addItem(curPath);
    paths.push_back(curPath);
    QPainterPath newPath;
    newPath.addRect(QRectF(0,0,w,h));

    curPath->setPath(newPath);
    curPath->startTLC = mapFromScene(curPath->pos() + curPath->rect().topLeft());
    curPath->startPos = curPath->pos();

    curPath->setBrush(QColor(0,0,255,26));
    curPath->setPen(QPen(Qt::lightGray, 3.0));

    if (onlinePack2(packedPaths, curPath)) {
        packedPaths.push_back(curPath);
        curPath->alive = true;
    } else curPath->alive = false;

    movingPaths.push_back(curPath);
}

void ShapePackingCanvas::clear() {
    for (ShapePackingPathItem* path : paths) {
        scene.removeItem(path);
    }
    paths.clear();
    packedPaths.clear();
    movingPaths.clear();
    corners.clear();
    corners.insert(0, QPointF(0,0));
    return;
}
