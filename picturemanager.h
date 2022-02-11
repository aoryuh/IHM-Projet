#ifndef PICTUREMANAGER_H
#define PICTUREMANAGER_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QRubberBand>
#include <QMouseEvent>
#include <QtCore>
#include <QtGui>
#include <QFileSystemModel>
#include <QStandardItemModel>
#include <QList>
#include <QSqlDatabase>
#include <QMainWindow>
#include "dataaccess.h"

#if defined(QT_PRINTSUPPORT_LIB)
#  include <QtPrintSupport/qtprintsupportglobal.h>

#  if QT_CONFIG(printer)
#    include <QPrinter>
#  endif
#endif

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QLabel;
class QScrollBar;
QT_END_NAMESPACE

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

    bool loadFile(const QString &);


private slots:
    void open();
    void saveAs();
    void print();
    void copy();
    void paste();
    void zoomIn();
    void zoomOut();
    void normalSize();
    void fitToWindow();
    void about();
    void rotate();
    void mirrorEffect();
    void blurEffect();
    QImage applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent=0);

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_lineEdit_textChanged(const QString &arg1);

    void getList(QDir rootpath, QString name);

    void on_treeView_clicked(const QModelIndex &index);

private:
    void createActions();
    void createMenus();
    void updateActions();
    bool saveFile(const QString &fileName);
    void setImage(const QImage &newImage);
    void scaleImage(double factor);
    void adjustScrollBar(QScrollBar *scrollBar, double factor);

    QImage image;
    QImage result;
    QLabel *imageLabel;
    double scaleFactor = 1;
    Ui::pictureManager *ui;
    QFileSystemModel *dirmodel;
    QStandardItemModel *filemodel;
    QString rootPath;
    QList<QFileInfo> fileList;
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printer)
    QPrinter printer;
#endif

    QAction *saveAsAct;
    QAction *printAct;
    QAction *copyAct;
    QAction *zoomInAct;
    QAction *zoomOutAct;
    QAction *normalSizeAct;
    QAction *fitToWindowAct;
    QAction *rotateImageAct;
    QAction *mirrorEffectAct;
    QAction *blurActionAct;



};

#endif // PICTUREMANAGER_H
