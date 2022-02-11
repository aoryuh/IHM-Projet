#include "mainwindow.h"
#include "imageviewer.h"
#include <QCommandLineParser>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;


    QGuiApplication::setApplicationDisplayName(ImageViewer::tr("Image Viewer"));
     QCommandLineParser commandLineParser;
        commandLineParser.addHelpOption();
        commandLineParser.addPositionalArgument(ImageViewer::tr("[file]"), ImageViewer::tr("C:/"));
        commandLineParser.process(QCoreApplication::arguments());
        ImageViewer imageViewer;
        if (!commandLineParser.positionalArguments().isEmpty()
            && !imageViewer.loadFile(commandLineParser.positionalArguments().front())) {
            return -1;
        }
        imageViewer.show();

    //w.show();
    return a.exec();
    //test
    //test22
}
