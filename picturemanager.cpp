#include "picturemanager.h"
#include "ui_picturemanager.h"
#include "dataaccess.h"
#include "dbmanager.h"
#include <QApplication>
#include <QClipboard>
#include <QColorSpace>
#include <QDir>
#include <QFileDialog>
#include <QImageReader>
#include <QImageWriter>
#include <QMenuBar>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QStatusBar>
#include <QGraphicsScene>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsBlurEffect>

#if defined(QT_PRINTSUPPORT_LIB)
#  include <QtPrintSupport/qtprintsupportglobal.h>

#  if QT_CONFIG(printdialog)
#    include <QPrintDialog>
#  endif
#endif

pictureManager::pictureManager(QWidget *parent) :
    QMainWindow(parent), imageLabel(new QLabel),
    ui(new Ui::pictureManager)
{
    ui->setupUi(this);

    dirmodel = new QFileSystemModel(this);
    dirmodel->setFilter(QDir:: NoDotAndDotDot |QDir::AllDirs | QDir::Files);
    dirmodel->setRootPath(rootPath);
    ui->treeView->setRootIndex(dirmodel->index(rootPath));

    ui->treeView->setModel(dirmodel);

    imageLabel->setBackgroundRole(QPalette::Base);
            imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            imageLabel->setScaledContents(true);

            ui->scrollArea->setBackgroundRole(QPalette::Dark);
            ui->scrollArea->setWidget(imageLabel);
            ui->scrollArea->setVisible(false);

            createActions();

            resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);

    /*filemodel = new QFileSystemModel(this);
    filemodel->setFilter(QDir:: NoDotAndDotDot |QDir::Files);
    filemodel->setRootPath(rootPath);

    ui->listView->setModel(filemodel);*/




}

pictureManager::~pictureManager()
{
    delete ui;
}


void pictureManager::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString sName = dirmodel->fileInfo(index).fileName();
    dataaccess data;
    data.setWindowTitle(sName);
    data.on_imageIdRef_windowTitleChanged(sName);
    data.setModal(true);
    data.exec();

    /*QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    ui->listView->setRootIndex(filemodel->setRootPath(sPath));*/

}

void pictureManager::on_listWidget_doubleClicked(const QModelIndex &index)
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
    db.createTable();
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
    foreach( const QFileInfo& entry, rootpath.entryInfoList(QDir::Dirs | QDir:: NoDotAndDotDot |QDir:: Readable) ) {
        if(!entry.baseName().isEmpty()){

            QString dirpathname;
            if(entry.absolutePath().size() > 3)
                 dirpathname = entry.absolutePath() + "/" + entry.baseName();

            else
                 dirpathname = entry.absolutePath() + entry.baseName();
            QDir dir(dirpathname);

            getList(dir, name);
        }
    }
}






void pictureManager::on_treeView_clicked(const QModelIndex &index)
{
    QString filename = dirmodel->fileInfo(index).fileName();
    QString filepath = dirmodel->fileInfo(index).absolutePath();
    /*if(filepath.size() > 3)
        filepath = filepath + "/";
    QString filecompletename = filepath + filename;
    qDebug("dir %s",filecompletename.toLocal8Bit().data());

    QImageReader reader(filecompletename);
    reader.setAutoTransform(true);
    QImage newImage = reader.read();
    newImage.convertToColorSpace(QColorSpace::SRgb);
    ui->imageLabel->setPixmap(QPixmap::fromImage(newImage));
    ui->imageLabel->adjustSize();


*/
    if(filepath.size() > 3)
        filepath = filepath + "/";
    QString filecompletename = filepath + filename;
    loadFile(filecompletename);
    imageLabel->adjustSize();

}

bool pictureManager::loadFile(const QString &fileName)
{
    QImageReader reader(fileName);
        reader.setAutoTransform(true);
        const QImage newImage = reader.read();
        if (newImage.isNull()) {
            QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                     tr("Cannot load %1: %2")
                                     .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
            return false;
        }


        setImage(newImage);

        setWindowFilePath(fileName);

        const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
            .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
        statusBar()->showMessage(message);
        return true;
 }

void pictureManager::setImage(const QImage &newImage)
{
    image = newImage;
    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);
    imageLabel->setPixmap(QPixmap::fromImage(image));
//! [4]
    scaleFactor = 1.0;

    ui->scrollArea->setVisible(true);
    printAct->setEnabled(true);
    fitToWindowAct->setEnabled(true);
    updateActions();

    if (!fitToWindowAct->isChecked())
        imageLabel->adjustSize();
}

bool pictureManager::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void pictureManager::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void pictureManager::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void pictureManager::print()
//! [5] //! [6]
{
    Q_ASSERT(!imageLabel->pixmap(Qt::ReturnByValue).isNull());
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
//! [6] //! [7]
    QPrintDialog dialog(&printer, this);
//! [7] //! [8]
    if (dialog.exec()) {
        QPainter painter(&printer);
        QPixmap pixmap = imageLabel->pixmap(Qt::ReturnByValue);
        QRect rect = painter.viewport();
        QSize size = pixmap.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
        painter.setWindow(pixmap.rect());
        painter.drawPixmap(0, 0, pixmap);
    }
#endif
}

void pictureManager::copy()
{
#ifndef QT_NO_CLIPBOARD
    QGuiApplication::clipboard()->setImage(image);
#endif // !QT_NO_CLIPBOARD
}

#ifndef QT_NO_CLIPBOARD
static QImage clipboardImage()
{
    if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
        if (mimeData->hasImage()) {
            const QImage image = qvariant_cast<QImage>(mimeData->imageData());
            if (!image.isNull())
                return image;
        }
    }
    return QImage();
}
#endif // !QT_NO_CLIPBOARD

void pictureManager::paste()
{
#ifndef QT_NO_CLIPBOARD
    const QImage newImage = clipboardImage();
    if (newImage.isNull()) {
        statusBar()->showMessage(tr("No image in clipboard"));
    } else {
        setImage(newImage);
        setWindowFilePath(QString());
        const QString message = tr("Obtained image from clipboard, %1x%2, Depth: %3")
            .arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
        statusBar()->showMessage(message);
    }
#endif // !QT_NO_CLIPBOARD
}


void pictureManager::zoomIn()
//! [9] //! [10]
{
    scaleImage(1.25);
}

void pictureManager::zoomOut()
{
    scaleImage(0.8);
}

//! [10] //! [11]
void pictureManager::normalSize()
//! [11] //! [12]
{
    imageLabel->adjustSize();
    scaleFactor = 1.0;
}

void pictureManager::fitToWindow()
//! [13] //! [14]
{
    bool fitToWindow = fitToWindowAct->isChecked();
    ui->scrollArea->setWidgetResizable(fitToWindow);
    if (!fitToWindow)
        normalSize();
    updateActions();
}



/*******************************************************************
 * (RANIA)
 * Mirror Effect On the Image
 * @brief ImageViewer::mirrorEffect
 *******************************************************************/
void pictureManager::mirrorEffect()
{
   setImage(image.mirrored(true,false));
}
/*******************************************************************
 * (RANIA)
 * Rotate the image on a 90 degrees angles
 * @brief ImageViewer::rotate
 *******************************************************************/
void pictureManager::rotate()
{
    setImage(image.transformed(QMatrix().rotate(90)));

}



QImage pictureManager::applyEffectToImage(QImage src, QGraphicsEffect *effect, int extent)
{
    if(src.isNull()) return QImage();   //No need to do anything else!
    //if(!effect) return src;
    extent=0; //No need to do anything else!
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    QImage res(src.size()+QSize(extent*2, extent*2), QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter ptr(&res);
    scene.render(&ptr, QRectF(), QRectF( -extent, -extent, src.width()+extent*2, src.height()+extent*2 ) );
    return res;
}
/*******************************************************************
 * (RANIA)
 * Blur effect on the image
 * @brief ImageViewer::blurEffect
 *******************************************************************/
void pictureManager::blurEffect()
{
    QGraphicsBlurEffect *blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(8);
    QImage result = applyEffectToImage(image, blur);
    setImage(result);
    result.save("final.png");
}


void pictureManager::about()
//! [15] //! [16]
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
               "and QScrollArea to display an image. QLabel is typically used "
               "for displaying a text, but it can also display an image. "
               "QScrollArea provides a scrolling view around another widget. "
               "If the child widget exceeds the size of the frame, QScrollArea "
               "automatically provides scroll bars. </p><p>The example "
               "demonstrates how QLabel's ability to scale its contents "
               "(QLabel::scaledContents), and QScrollArea's ability to "
               "automatically resize its contents "
               "(QScrollArea::widgetResizable), can be used to implement "
               "zooming and scaling features. </p><p>In addition the example "
               "shows how to use QPainter to print an image.</p>"));
}


void pictureManager::createActions()
//! [17] //! [18]
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &pictureManager::open);
    openAct->setShortcut(QKeySequence::Open);

    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &pictureManager::saveAs);
    saveAsAct->setEnabled(false);

    printAct = fileMenu->addAction(tr("&Print..."), this, &pictureManager::print);
    printAct->setShortcut(QKeySequence::Print);
    printAct->setEnabled(false);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    copyAct = editMenu->addAction(tr("&Copy"), this, &pictureManager::copy);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setEnabled(false);

    QAction *pasteAct = editMenu->addAction(tr("&Paste"), this, &pictureManager::paste);
    pasteAct->setShortcut(QKeySequence::Paste);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    zoomInAct = viewMenu->addAction(tr("Zoom &In (25%)"), this, &pictureManager::zoomIn);
    zoomInAct->setShortcut(QKeySequence::ZoomIn);
    zoomInAct->setEnabled(false);

    zoomOutAct = viewMenu->addAction(tr("Zoom &Out (25%)"), this, &pictureManager::zoomOut);
    zoomOutAct->setShortcut(QKeySequence::ZoomOut);
    zoomOutAct->setEnabled(false);

    normalSizeAct = viewMenu->addAction(tr("&Normal Size"), this, &pictureManager::normalSize);
    normalSizeAct->setShortcut(tr("Ctrl+S"));
    normalSizeAct->setEnabled(false);

    rotateImageAct = viewMenu->addAction(tr("&Rotate"), this, &pictureManager::rotate);
    rotateImageAct->setEnabled(true);
    rotateImageAct->setShortcut(tr("Ctrl+R"));

    mirrorEffectAct = viewMenu->addAction(tr("&Mirror Effect"), this, &pictureManager::mirrorEffect);
    mirrorEffectAct->setEnabled(true);
    mirrorEffectAct->setShortcut(tr("Ctrl+M"));

    blurActionAct = viewMenu->addAction(tr("&Blur Effect"), this, &pictureManager::blurEffect);
    blurActionAct->setEnabled(true) ;
    blurActionAct->setShortcut(tr("Ctrl+B")) ;

    viewMenu->addSeparator();

    fitToWindowAct = viewMenu->addAction(tr("&Fit to Window"), this, &pictureManager::fitToWindow);
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));


    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));

    helpMenu->addAction(tr("&About"), this, &pictureManager::about);
    helpMenu->addAction(tr("About &Qt"), &QApplication::aboutQt);


}
//! [18]

//! [21]
void pictureManager::updateActions()
//! [21] //! [22]
{
    saveAsAct->setEnabled(!image.isNull());
    copyAct->setEnabled(!image.isNull());
    zoomInAct->setEnabled(!fitToWindowAct->isChecked());
    zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
    normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}

void pictureManager::scaleImage(double factor)
//! [23] //! [24]
{
    scaleFactor *= factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap(Qt::ReturnByValue).size());

    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);

    zoomInAct->setEnabled(scaleFactor < 3.0);
    zoomOutAct->setEnabled(scaleFactor > 0.333);
}

void pictureManager::adjustScrollBar(QScrollBar *scrollBar, double factor)
//! [25] //! [26]
{
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}




