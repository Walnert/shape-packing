#include "mainwindow.h"
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QTimer>
#include <QDebug>
#include <iostream>
#include <sstream>
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

    label = new QLabel("Click and drag to draw rectangles, or add rectangles manually in the input field in order W1 H1 W2 H2 W3 H3 ...");
    clearButton = new QPushButton("Clear");
    addButton = new QPushButton("Add");
    inputField = new QLineEdit();
    inputField->setPlaceholderText("ex. 50 70 30 80 to place a 50x70 rectangle, then a 30x80 rectangle");

    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearButtonClicked);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addButtonClicked);

    mainLayout->addWidget(controlRow);
        controlRowLayout->addWidget(clearButton);
        controlRowLayout->addWidget(inputField);
        controlRowLayout->addWidget(addButton);
    mainLayout->addWidget(label);
    mainLayout->addWidget(view);
}

MainWindow::~MainWindow() { }

void MainWindow::clearButtonClicked() {
    view->clear();
    label->setText("Canvas cleared.");
}

void MainWindow::addButtonClicked() {
    std::vector<int> v;
    std::stringstream iss(inputField->text().toStdString());
    double number;
    while (iss >> number)
      v.push_back(number);
    //vs.append(l);
    if (v.size() % 2 == 1) {
        //inputField->clear();
        label->setText("Error: must enter an even number of dimensions");
    } else {
        for (int i = 0 ; i < v.size() ; i = i +2) {
            view->addNewRect(v[i], v[i+1]);
        }
        label->setText(QString::number(v.size()/2) + " rectangles added.");
    }
}

