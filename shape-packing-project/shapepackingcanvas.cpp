#include "shapepackingcanvas.h"
//#include "shapepackingpathitem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QtWidgets>
#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <math.h>

ShapePackingCanvas::ShapePackingCanvas() {
    setBackgroundBrush(QBrush(QColor(25,25,25), Qt::SolidPattern));
    curPath = nullptr;

    setScene(&scene);
    setRenderHint(QPainter::Antialiasing);
    setSceneRect(-400,-300,800,600);

    timer.setInterval(1000/60);
    connect(&timer, &QTimer::timeout, this, &ShapePackingCanvas::timerUpdate);
    timer.start();

    pathAnimSpeed = .03;
    mode = 2;
}

QPointF getSizePointF(QGraphicsPathItem *path) {
    return QPointF(path->shape().boundingRect().size().width(),
                   path->shape().boundingRect().size().height());
}

void ShapePackingCanvas::mousePressEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos());
    mouseClickedPos = pos;

    QPainterPath path;
    path.addRect(QRectF(pos,QSizeF(0,0)));
    curPath = scene.addPath(path,QPen(Qt::yellow, 3.0));
    paths.push_back(curPath);

    coordsText = scene.addSimpleText("0\n0");
    coordsText->setPos(pos + QPointF(20,0));
    coordsText->setBrush(Qt::white);

    //mouseClickedPos = evt->lastScenePos().toPoint();
    //create rectangle, set as current item, push to list, set top left corner position
}

void ShapePackingCanvas::mouseMoveEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos());
    //qDebug() << pos; //fix the coordinate systems!
    if (curPath == nullptr) return;

    QPainterPath newPath;
    newPath.addRect(QRectF(mouseClickedPos, pos));
    curPath->setPath(newPath);

    coordsText->setText(QString::number(getSizePointF(curPath).x()) + "\n" +
                        QString::number(getSizePointF(curPath).y()));
    coordsText->setPos(pos + QPointF(20,0));
}


void ShapePackingCanvas::mouseReleaseEvent(QMouseEvent *evt) {
    //QPointF pos = mapToScene(evt->pos());

    curPath->setBrush(QColor(0,0,255,26));
    curPath->setPen(QPen(Qt::lightGray, 3.0));
    //QPointF rectBottomLeftCorner = mapFromScene(curPath->shape().boundingRect().bottomLeft());
    //QPointF sceneBottomLeftCorner = QPointF(0,this->height());

    movingPaths.push_back(curPath);
    //curPath->setPos(sceneBottomLeftCorner - rectBottomLeftCorner);

    scene.removeItem(coordsText);
    //fill in rectangle, set current to nullpointer
}

void ShapePackingCanvas::timerUpdate() {
    if (mode==1) {
        QPointF sceneBottomLeftCorner = QPointF(0,this->height());
        //int frames = 60;
        for (QGraphicsPathItem* path : movingPaths) {
            QPointF rectBottomLeftCorner = mapFromScene(path->pos() + path->shape().boundingRect().bottomLeft());
    //        QVector2D rBLC = QVector2D(rectBottomLeftCorner);
    //        QVector2D sBLC = QVector2D(sceneBottomLeftCorner);
    //        QVector2D deltaV = sBLC - rBLC;
            QPointF delta = sceneBottomLeftCorner - rectBottomLeftCorner;
            path->setPos(path->pos() + 0.5*normalized(delta) + delta*pathAnimSpeed);

            if (delta.isNull()) {
                path->setBrush(QColor(255,0,0,26));
                movingPaths.removeOne(path);
            }
        }
    } else if (mode==2) {
        for (QGraphicsPathItem* path : movingPaths) {
            QPointF rectPos = mapFromScene(path->pos());// + path->shape().boundingRect().center());
            for (QGraphicsPathItem* otherPath : movingPaths) {
                if (true) {
                    QPointF otherPos = mapFromScene(otherPath->pos());// + otherPath->shape().boundingRect().center());
                    QPointF delta = otherPos-rectPos;
                    if (true) {
                        path->setPos(path->pos() + 1 * delta);// / (pow(hypotf(delta.x(),delta.y()),3)));

                    }
                }
            }
        }
    }
}

QPointF ShapePackingCanvas::normalized(const QPointF point) {
    if (!point.isNull()) {
        float mag = hypotf(point.x(), point.y());
        return point/mag;
    }
    else return QPointF(0,0);
}


