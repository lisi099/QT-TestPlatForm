#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hello.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int a =100;
    int b =1000;
    int c = max(a, b);
    ui->label->setText(QString::number(c));
//    qDebug() <<c;
}

MainWindow::~MainWindow()
{
    delete ui;
}
