#include "dataaccess.h"
#include "ui_dataaccess.h"
#include <dbmanager.h>
dataaccess::dataaccess(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dataaccess)
{
    ui->setupUi(this);

}

dataaccess::~dataaccess()
{
    delete ui;
}

void dataaccess::on_onSubmit_clicked()
{
    DbManager db;
    db.createTable();
    QString imageColor=ui->imageColor->text();
    QString imageTheme=ui->imageTheme->text();
    QString imageName=ui->imageName->text();
    QString imageHeight=ui->imageHeight->text();
    QString imageWidth=ui->imageWidth->text();


    QString imageIdRef = ui->imageIdRef->text();



    db.addImageData(imageIdRef, imageName, imageColor, imageTheme, imageHeight, imageWidth);
}


void dataaccess::on_onReset_clicked()
{
    dataaccess::close();
}

void dataaccess::on_imageIdRef_windowTitleChanged(const QString &title)
{
    ui->imageIdRef->setText(title);
}
