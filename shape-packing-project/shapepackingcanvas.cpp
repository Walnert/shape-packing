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

    setScene(&scene);
    setRenderHint(QPainter::Antialiasing);
    setSceneRect(-400,-300,800,600);

    timer.setInterval(1000/60);
    connect(&timer, &QTimer::timeout, this, &ShapePackingCanvas::timerUpdate);
    timer.start();

    pathAnimSpeed = .03;
    mode = 1;
    curCoordsText = scene.addSimpleText("0\n0");

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
    curPath = (ShapePackingPathItem *) scene.addPath(path,QPen(Qt::yellow, 3.0));
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
    //QPointF pos = mapToScene(evt->pos());

    curPath->setBrush(QColor(0,0,255,26));
    curPath->setPen(QPen(Qt::lightGray, 3.0));

    qDebug() << "mouse released BEFORE sort";
    //pack2D(paths);
    qDebug() << "mouse released AFTER sort";


    movingPaths.push_back(curPath);
    //curPath->setPos(sceneBottomLeftCorner - rectBottomLeftCorner);

    coordsTexts.append(curCoordsText);
    //fill in rectangle, set current to nullpointer
}

void ShapePackingCanvas::timerUpdate() {
    for (QGraphicsSimpleTextItem* coordsText : coordsTexts) {
        coordsText->setOpacity(coordsText->opacity()-.02);
        coordsText->moveBy(0,-.2);
        if (coordsText->opacity() < .02) {
            scene.removeItem(coordsText);
            coordsTexts.removeOne(coordsText);
        }
    }

    if (mode==1) {
        QPointF sceneBottomLeftCorner = QPointF(0,this->height());
        //int frames = 60;
        for (ShapePackingPathItem* path : movingPaths) {
            QPointF rectBottomLeftCorner = mapFromScene(path->pos() + path->shape().boundingRect().bottomLeft());
    //        QVector2D rBLC = QVector2D(rectBottomLeftCorner);
    //        QVector2D sBLC = QVector2D(sceneBottomLeftCorner);
    //        QVector2D deltaV = sBLC - rBLC;
            QPointF delta = sceneBottomLeftCorner - rectBottomLeftCorner;//path->packedPos + QPointF(400,400) - rectBottomLeftCorner;
            path->setPos(path->pos() + 1*normalized(delta) + delta*pathAnimSpeed);

            if (delta.isNull()) {
                path->setBrush(QColor(255,0,0,26));
                movingPaths.removeOne(path);
            }
        }
    } else if (mode==2) {
        for (ShapePackingPathItem* path : movingPaths) {
            QPointF rectPos = mapFromScene(path->pos() + path->shape().boundingRect().center());
            for (ShapePackingPathItem* otherPath : movingPaths) {
                if (true) {
                    QPointF otherPos = mapFromScene(otherPath->pos() + otherPath->shape().boundingRect().center());
                    QPointF delta = otherPos-rectPos;
                    if (true) {
                        path->setPos(path->pos() + .01*delta);// / (pow(hypotf(delta.x(),delta.y()),3)));

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

       // random size for this content
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
     //inputContent.Sort();

     // Create some bins! ( 2 bins, 128x128 in this example )
     BinPack2D::CanvasArray<MyContent> canvasArray =
       BinPack2D::UniformCanvasArrayBuilder<MyContent>(this->width(),this->height(),1).Build();

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

       printf("\t%9s of size %3dx%3d at position %3d,%3d,%2d rotated=%s\n",
          myContent.str.c_str(),
          content.size.w,
          content.size.h,
          content.coord.x,
          content.coord.y,
          content.coord.z,
          (content.rotated ? "yes":" no"));

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


