#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <QDialog>

namespace Ui {
class dataaccess;
}

class dataaccess : public QDialog
{
    Q_OBJECT

public:
    explicit dataaccess(QWidget *parent = nullptr);
    ~dataaccess();

private slots:
    void on_onSubmit_clicked();

private:
    Ui::dataaccess *ui;
};

#endif // DATAACCESS_H
