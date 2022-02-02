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
    db.addImageData("A", "B");
}

