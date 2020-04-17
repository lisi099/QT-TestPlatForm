#include "mainwindow.h"
#include <QtWidgets>
#include <QtGui/QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    label_1= new QLabel(this);
    button = new QPushButton(this);
    QIcon icon2D("ARROW6C.ICO");
    button->setIcon(icon2D);
    button->show();
    //label_1->setText("hello world");
    //label_1->show();

}

MainWindow::~MainWindow()
{

}
