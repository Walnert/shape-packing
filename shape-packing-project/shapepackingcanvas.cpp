#include "shapepackingcanvas.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QtWidgets>
#include <QPainter>
#include <QGraphicsSimpleTextItem>

ShapePackingCanvas::ShapePackingCanvas() {
    curPath = nullptr;
    //QGraphicsSimpleTextItem coordsText;
    //coordsText->setBrush(Qt::white);

    setScene(&scene);
    setRenderHint(QPainter::Antialiasing);
    setSceneRect(-400,-300,800,600);

    timer.setInterval(1000/60);
    connect(&timer, &QTimer::timeout, this, &ShapePackingCanvas::timerUpdate);
    timer.start();
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
    QPointF pos = mapToScene(evt->pos());

    curPath->setBrush(QColor(0,0,255,26));
    curPath->setPen(QPen(Qt::lightGray, 3.0));
    QPointF rectBottomLeftCorner = mapFromScene(curPath->shape().boundingRect().bottomLeft());
    QPointF sceneBottomLeftCorner = QPointF(0,this->height());

    movingPaths.push_back(curPath);
    //curPath->setPos(sceneBottomLeftCorner - rectBottomLeftCorner);

    scene.removeItem(coordsText);
    //fill in rectangle, set current to nullpointer
}

void ShapePackingCanvas::timerUpdate() {
    QPointF sceneBottomLeftCorner = QPointF(0,this->height());
    //int frames = 60;
    for (QGraphicsPathItem* path : movingPaths) {
        QPointF rectBottomLeftCorner = mapFromScene(path->shape().boundingRect().bottomLeft());
        QPointF delta = sceneBottomLeftCorner - rectBottomLeftCorner;
        path->setPos(path->pos() + delta*0.01);

        //if (delta.x() ==0)
            //movingPaths.pop_front();
    }
}


