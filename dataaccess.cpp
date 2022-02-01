#include "dataaccess.h"
#include "ui_dataaccess.h"

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
