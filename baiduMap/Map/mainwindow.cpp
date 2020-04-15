#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebChannel>
#include <QUrl>
#include <QWebEngineHistory>
#include <QWebEnginePage>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString path(qApp->applicationDirPath() + "/index.html");
    qDebug() << path;
    ui->webView->load(QUrl(path));
    ui->webView->show();

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
    m_timer.start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onTimeOut()
{

}
