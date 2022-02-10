#ifndef PICTUREMANAGER_H
#define PICTUREMANAGER_H

#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QList>
#include <QMainWindow>
#include "dataaccess.h"

namespace Ui {
class pictureManager;
}

class pictureManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit pictureManager(QWidget *parent = nullptr);
    ~pictureManager();
    void on_set_imageView(const QString &path);

    void setRootPath(QString path);

    QString getRootPath();

    void initialize();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_listView_doubleClicked(const QModelIndex &index);

    void on_lineEdit_textChanged(const QString &arg1);

    void getList(QDir rootpath, QString name);

private:
    Ui::pictureManager *ui;
    QFileSystemModel *dirmodel;
    QStandardItemModel *filemodel;
    QString rootPath;
    QList<QFileInfo> fileList;



};

#endif // PICTUREMANAGER_H
