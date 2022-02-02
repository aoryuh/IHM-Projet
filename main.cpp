#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    int i = 42;
//    std::string prefix = PROJECT_PATH;
//    static const QString path = PROJECT_PATH + "p1_db.db";
//    qDebug() << "path --" << path;
    w.show();
    return a.exec();
    //test
    //test22
}
