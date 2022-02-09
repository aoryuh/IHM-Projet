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
    pictureManager *w = new pictureManager(this);

    w->setWindowTitle("votre bibliothèque");

    QString path = filemodel->fileInfo(index).absoluteFilePath();
    w->setRootPath(path);
    w->initialize();

    qDebug("yo %s",w->getRootPath().toLocal8Bit().data());
    w->show();

}
