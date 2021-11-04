#include "mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QDebug>
#include <cmath>
#include <QPen>
#include <QRect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    view = new ShapePackingCanvas();
    setCentralWidget(view);
}

MainWindow::~MainWindow() { }

