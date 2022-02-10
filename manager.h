#ifndef MANAGER_H
#define MANAGER_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include "dataaccess.h"

namespace Ui {
class manager;
}

class manager : public QDialog
{
    Q_OBJECT

public:
    explicit manager(QWidget *parent = nullptr);
    ~manager();
    void on_set_imageView(const QString &path);

    void setRootPath(QString path);

    QString getRootPath();

    void initialize();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::manager *ui;
    QFileSystemModel *dirmodel;
    QFileSystemModel *filemodel;
    QString rootPath;


};
#endif // MANAGER_H
