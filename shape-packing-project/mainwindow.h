#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include "shapepackingcanvas.h"

class QPushButton;
class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    ShapePackingCanvas *view;
    QPushButton *clearButton;
    QPushButton *addButton;
    QLineEdit *inputField;

public slots:
    void clearButtonClicked();
    void addButtonClicked();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
