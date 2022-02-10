#include "picturemanager.h"
#include "ui_picturemanager.h"
#include "dataaccess.h"
#include "dbmanager.h"

pictureManager::pictureManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pictureManager)
{
    ui->setupUi(this);

    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir:: NoDotAndDotDot |QDir::AllDirs | QDir::Files);
    dirmodel->setRootPath(rootPath);
    ui->treeView->setRootIndex(dirmodel->index(rootPath));

    ui->treeView->setModel(dirmodel);

    /*filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir:: NoDotAndDotDot |QDir::Files);
    filemodel->setRootPath(rootPath);

    ui->listView->setModel(filemodel);*/




}

pictureManager::~pictureManager()
{
    delete ui;
}


void pictureManager::on_treeView_clicked(const QModelIndex &index)
{
    qDebug("pas oublier de remettre on_treeview_clicked");

    /*QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(filemodel->setRootPath(sPath));*/

}

void pictureManager::on_listView_doubleClicked(const QModelIndex &index)
{
    QString sName = dirmodel->fileInfo(index).fileName();
    dataaccess data;
    data.setWindowTitle(sName);
    data.on_imageIdRef_windowTitleChanged(sName);
    data.setModal(true);
    data.exec();
}

/*void pictureManager::on_set_imageView(const QString &path)
{
    QImage* image = new QImage();
    image->load(path);
    QGraphicsScene* scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));

}*/

void pictureManager::setRootPath(QString path){
    rootPath = path;
}

QString pictureManager::getRootPath(){
    return rootPath;
}

void pictureManager::initialize(){
    dirmodel->setRootPath(rootPath);
    ui->treeView->setRootIndex(dirmodel->index(rootPath));

    ui->treeView->setModel(dirmodel);


    /*filemodel->setRootPath(rootPath);

    ui->listView->setModel(filemodel);*/

    /*QDir dir(rootPath);
    getList(dir);*/



    //ui->listView->setModel(fileList);
    getList(this->rootPath, "");
    foreach (QFileInfo file ,fileList){
        QString basename;
        if(file.absolutePath().size() > 3)
            basename = "/" + file.baseName();
        else
            basename = file.baseName();
        QString filepathname = file.absolutePath() + basename + "." + file.completeSuffix();
        ui->listWidget->addItem(filepathname);
    }

    DbManager db;
    db.initialize(rootPath);
    qDebug("db %s",rootPath.toLocal8Bit().data());
}

void pictureManager::on_lineEdit_textChanged(const QString &arg1)
{
    fileList.clear();
    ui->listWidget->clear();

    getList(this->rootPath, ui->lineEdit->displayText());
    foreach (QFileInfo file ,fileList){
        QString basename;
        if(file.absolutePath().size() > 3)
            basename = "/" + file.baseName();
        else
            basename = file.baseName();
        QString filepathname = file.absolutePath() + basename + "." + file.completeSuffix();

        ui->listWidget->addItem(filepathname);
    }

}

void pictureManager::getList(QDir rootpath, QString name){
    foreach( const QFileInfo& entry, rootpath.entryInfoList(QDir::Files | QDir:: NoDotAndDotDot) ) {
        if(entry.baseName().contains(name)){
            fileList.append(entry);
        }
    }
    foreach( const QFileInfo& entry, rootpath.entryInfoList(QDir::Dirs | QDir:: NoDotAndDotDot) ) {

        QString dirpathname = entry.absolutePath() + "/" + entry.baseName();
        QDir dir(dirpathname);
        qDebug("dir %s",dirpathname.toLocal8Bit().data());
        getList(dir, name);
    }
}





