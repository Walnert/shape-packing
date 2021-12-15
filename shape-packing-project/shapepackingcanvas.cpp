#include "shapepackingcanvas.h"
#include "shapepackingpathitem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QtWidgets>
#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include <math.h>
#include "BinPack2D.hpp"
//#include <stdio.h>

ShapePackingCanvas::ShapePackingCanvas() {
    setBackgroundBrush(QBrush(QColor(25,25,25), Qt::SolidPattern));
    curPath = nullptr;
    corners.insert(0, QPointF(0,0));

    setScene(&scene);
    setRenderHint(QPainter::Antialiasing);
    setSceneRect(-400,-300,800,600);

    pathAnimSpeed = .04;
    framerate = 60;
    mode = 1;

    //int t = 0;

    timer.setInterval(1000/framerate);
    connect(&timer, &QTimer::timeout, this, &ShapePackingCanvas::timerUpdate);
    timer.start();
    //curCoordsText = scene.addSimpleText("");

}

QPointF getSizePointF(ShapePackingPathItem *path) {
    return QPointF(path->shape().boundingRect().size().width(),
                   path->shape().boundingRect().size().height());
}

void ShapePackingCanvas::mousePressEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos());
    mouseClickedPos = pos;

    QPainterPath path;
    path.addRect(QRectF(pos,QSizeF(0,0)));
    curPath = new ShapePackingPathItem();
    curPath->setPath(path);
    curPath->setPen(QPen(Qt::yellow, 3.0));
    scene.addItem(curPath);
    paths.push_back(curPath);

    curCoordsText = scene.addSimpleText("0\n0");
    curCoordsText->setPos(pos + QPointF(20,0));
    curCoordsText->setBrush(Qt::white);
    //create rectangle, set as current item, push to list, set top left corner position
}

void ShapePackingCanvas::mouseMoveEvent(QMouseEvent *evt) {
    QPointF pos = mapToScene(evt->pos());
    //qDebug() << pos; //fix the coordinate systems!
    if (curPath == nullptr) return;

    QPainterPath newPath;
    newPath.addRect(QRectF(mouseClickedPos, pos));

    curPath->setPath(newPath);

    curCoordsText->setText(QString::number(getSizePointF(curPath).x()) + "\n" +
                        QString::number(getSizePointF(curPath).y()));
    curCoordsText->setPos(pos + QPointF(20,0));
}


void ShapePackingCanvas::mouseReleaseEvent(QMouseEvent *evt) {
    curPath->startTLC = mapFromScene(curPath->pos() + curPath->rect().topLeft());
    curPath->startPos = curPath->pos();

    qDebug() << "start TLC = " << curPath->startTLC;

    curPath->setBrush(QColor(0,0,255,26));
    curPath->setPen(QPen(Qt::lightGray, 3.0));

    onlinePack2(packedPaths, curPath);
    packedPaths.push_back(curPath);

    movingPaths.push_back(curPath);
    //curPath->setPos(sceneBottomLeftCorner - rectBottomLeftCorner);

    coordsTexts.append(curCoordsText);
    //fill in rectangle, set current to nullpointer
}

void ShapePackingCanvas::timerUpdate() {
    addNewRect(1+t, 1+t);
    ++t;
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
            ++path->age;
            //QPointF rectTopLeftCorner = mapFromScene(path->pos() + path->rect().topLeft());
            QPointF delta = path->packedPos - path->startTLC;
            path->setPos(path->startPos + delta*path->age/framerate);

            if (path->age >= 60) {
                //path->setPos(path->packedPos + path->startPos);
                path->setBrush(QColor(255,0,0,26));
                movingPaths.removeOne(path);
            }
        }
    } else if (mode==2) {
    return;
    }
}

void ShapePackingCanvas::onlinePack2(QList<ShapePackingPathItem *> packedPaths, ShapePackingPathItem* curPath) {
    //qDebug() << "Corners BEFORE: " << corners;

    for (auto icorner = corners.begin() ; icorner != corners.end() ; ++icorner) {
        QPointF &corner = *icorner;
        //qDebug() << "trying " << corner;
        curPath->packedPos = corner;
        bool ok = true;
        for (ShapePackingPathItem* path : packedPaths) {
            //qDebug() << "trying path " << path->startTLC;
            if (curPath->intersects(path)) {
                //qDebug() << "failed " << path->startTLC;
                ok = false;
                break;
            }
        }
        if (ok) {
            //qDebug() << "success at corner " << corner << "\n";
            corners.erase(icorner);
            QPointF corner1 = curPath->packedPos + QPointF(curPath->rect().width(),0);
            QPointF corner2 = curPath->packedPos + QPointF(0,curPath->rect().height());
            corners.insert(corner1.manhattanLength(), corner1);
            corners.insert(corner2.manhattanLength(), corner2);
            //qDebug() << "Corners AFTER: " << corners;
            break;
        }
    }
    return;
}

QPointF ShapePackingCanvas::normalized(const QPointF point) {
    if (!point.isNull()) {
        float mag = hypotf(point.x(), point.y());
        return point/mag;
    }
    else return QPointF(0,0);
}

void ShapePackingCanvas::addNewRect(int w, int h) {
    //QPainterPath path;
    //path.addRect(QRectF(pos,QSizeF(0,0)));
    curPath = new ShapePackingPathItem();
    //curPath->setPath(path);
    curPath->setPen(QPen(Qt::yellow, 3.0));
    scene.addItem(curPath);
    paths.push_back(curPath);
    QPainterPath newPath;
    newPath.addRect(QRectF(0,0,w,h));

    curPath->setPath(newPath);
    curPath->startTLC = mapFromScene(curPath->pos() + curPath->rect().topLeft());
    curPath->startPos = curPath->pos();

    //qDebug() << "start TLC = " << curPath->startTLC;

    curPath->setBrush(QColor(0,0,255,26));
    curPath->setPen(QPen(Qt::lightGray, 3.0));

    onlinePack2(packedPaths, curPath);
    packedPaths.push_back(curPath);

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
    t=0;
    corners.insert(0, QPointF(0,0));
    return;
}

void ShapePackingCanvas::pack2D(QList<ShapePackingPathItem *> paths) {
    class MyContent {
     public:
     std::string str;
     MyContent() : str("default string") {}
     MyContent(const std::string &str) : str(str) {}
     };

     //srandom(0x69);

     // Create some 'content' to work on.
     BinPack2D::ContentAccumulator<MyContent> inputContent;

     for(ShapePackingPathItem* path : paths) {

       //printf("hello");

       int width  = path->shape().boundingRect().size().width();//getSizePointF(path).x();
       int height = path->shape().boundingRect().size().height();//getSizePointF(path).y();

       // whatever data you want to associate with this content
       std::stringstream ss;
       ss << "box " << width << " " << height;
       MyContent mycontent( ss.str().c_str() );

       // Add it
       inputContent += BinPack2D::Content<MyContent>(mycontent, BinPack2D::Coord(), BinPack2D::Size(width, height), false );
     }

     // Sort the input content by size... usually packs better.
     inputContent.Sort();

     // Create some bins! ( 2 bins, 128x128 in this example )
     BinPack2D::CanvasArray<MyContent> canvasArray =
       BinPack2D::UniformCanvasArrayBuilder<MyContent>(this->width(),this->height(),1).Build();
       qDebug() << this->width() << this->height();

     // A place to store content that didnt fit into the canvas array.
     BinPack2D::ContentAccumulator<MyContent> remainder;

     // try to pack content into the bins.
     bool success = canvasArray.Place( inputContent, remainder );

     // A place to store packed content.
     BinPack2D::ContentAccumulator<MyContent> outputContent;

     // Read all placed content.
     canvasArray.CollectContent( outputContent );

     // parse output.
     typedef BinPack2D::Content<MyContent>::Vector::iterator binpack2d_iterator;
     printf("PLACED:\n");
     int i = 0;
     for( binpack2d_iterator itor = outputContent.Get().begin(); itor != outputContent.Get().end(); itor++ ) {


       BinPack2D::Content<MyContent> &content = *itor;

       // retreive your data.
       MyContent &myContent = content.content;

//       printf("\t%9s of size %3dx%3d at position %3d,%3d,%2d rotated=%s\n",
//          myContent.str.c_str(),
//          content.size.w,
//          content.size.h,
//          content.coord.x,
//          content.coord.y,
//          content.coord.z,
//          (content.rotated ? "yes":" no"));

       qDebug() << myContent.str.c_str() << " of size " << content.size.w << "x" << content.size.h << " at position " << content.coord.x << ", " << content.coord.y;
       qDebug() << paths;


       paths[i]->packedPos = QPointF(content.coord.x, content.coord.y);
       qDebug() << paths[i]->packedPos;
       qDebug() << i;
       ++i;


     printf("NOT PLACED:\n");
     for( binpack2d_iterator itor = remainder.Get().begin(); itor != remainder.Get().end(); itor++ ) {

       const BinPack2D::Content<MyContent> &content = *itor;

       const MyContent &myContent = content.content;

       printf("\t%9s of size %3dx%3d\n",
          myContent.str.c_str(),
          content.size.w,
          content.size.h);
     }

     return;
   }
}


