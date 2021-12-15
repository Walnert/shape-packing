#include "mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QDebug>
//#include <cmath>
//#include <QPen>
//#include <QRect>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    view = new ShapePackingCanvas();

    QWidget *mainWidget = new QWidget();
    QWidget *controlRow = new QWidget();

    QHBoxLayout *controlRowLayout = new QHBoxLayout();
    QVBoxLayout *mainLayout = new QVBoxLayout();

    controlRow->setLayout(controlRowLayout);
    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);

    clearButton = new QPushButton("Clear");
    addButton = new QPushButton("Add");
    inputField = new QLineEdit();
    inputField->setPlaceholderText("W x H");

    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearButtonClicked);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addButtonClicked);


    mainLayout->addWidget(controlRow);
        controlRowLayout->addWidget(clearButton);
        controlRowLayout->addWidget(inputField);
        controlRowLayout->addWidget(addButton);
    mainLayout->addWidget(view);
}

MainWindow::~MainWindow() { }

void MainWindow::clearButtonClicked() {
    view->clear();
}

void MainWindow::addButtonClicked() {
    view->addNewRect(30, 50);
}

