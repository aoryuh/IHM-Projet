#include "manager.h"
#include "ui_manager.h"
#include "dataaccess.h"
#include "dbmanager.h"


manager::manager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::manager)
{
    ui->setupUi(this);

    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir:: NoDotAndDotDot |QDir::AllDirs);
    dirmodel->setRootPath(rootPath);
    ui->treeView->setRootIndex(dirmodel->index(rootPath));

    ui->treeView->setModel(dirmodel);

    filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir:: NoDotAndDotDot |QDir::Files);
    filemodel->setRootPath(rootPath);

    ui->listView->setModel(filemodel);




}

manager::~manager()
{
    delete ui;
}


void manager::on_treeView_clicked(const QModelIndex &index)
{
    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(filemodel->setRootPath(sPath));

}

void manager::on_listView_doubleClicked(const QModelIndex &index)
{
    QString sName = dirmodel->fileInfo(index).fileName();
    dataaccess data;
    data.setWindowTitle(sName);
    data.on_imageIdRef_windowTitleChanged(sName);
    data.setModal(true);
    data.exec();
}

void manager::on_set_imageView(const QString &path)
{
    /*QImage* image = new QImage();
    image->load(path);
    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
*/
}

void manager::setRootPath(QString path){
    rootPath = path;
}

QString manager::getRootPath(){
    return rootPath;
}

void manager::initialize(){
    dirmodel->setRootPath(rootPath);
    ui->treeView->setRootIndex(dirmodel->index(rootPath));

    ui->treeView->setModel(dirmodel);

    filemodel->setRootPath(rootPath);

    ui->listView->setModel(filemodel);
    DbManager db;
    db.initialize(rootPath);
    qDebug("db %s",rootPath.toLocal8Bit().data());

}
