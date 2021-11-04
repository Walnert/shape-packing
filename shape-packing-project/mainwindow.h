#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "shapepackingcanvas.h"
#include <QTimer>
//#include <QGraphicsView>

//class ShapePackingCanvas;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    ShapePackingCanvas *view;

public slots:
    //void timerUpdate();


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
