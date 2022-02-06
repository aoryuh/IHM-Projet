#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir:: Drives);
    filemodel->setRootPath("");
    ui->listView->setRootIndex(filemodel->index(""));

    ui->listView->setModel(filemodel);


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    manager window;
    window.setWindowTitle("votre bibliothèque");

    QString path = filemodel->fileInfo(index).absoluteFilePath();
    window.setRootPath(path);
    window.initialize();

    qDebug("yo %s",window.getRootPath().toLocal8Bit().data());
    window.exec();
    this->close();
}
