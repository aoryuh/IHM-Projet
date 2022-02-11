#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dataaccess.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsView>
#include <QPixmap>
#include <QImage>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString sPath = "C:/";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir:: NoDotAndDotDot |QDir::AllDirs);
    dirmodel->setRootPath(sPath);
    ui->treeView->setRootIndex(dirmodel->index(sPath));

    ui->treeView->setModel(dirmodel);

    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir:: NoDotAndDotDot |QDir::Files);
    filemodel->setRootPath(sPath);

    ui->listView->setModel(filemodel);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(filemodel->setRootPath(sPath));

}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QString sName = dirmodel->fileInfo(index).fileName();
    dataaccess data;
    data.setWindowTitle(sName);
    data.on_imageIdRef_windowTitleChanged(sName);
    data.setModal(true);
    data.exec();
}

void MainWindow::on_set_imageView(const QString &path)
{
    QImage* image = new QImage();
    image->load(path);
    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    //QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));

}
