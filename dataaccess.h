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
    void on_imageIdRef_windowTitleChanged(const QString &title);

private slots:
    void on_onSubmit_clicked();

    void on_onReset_clicked();



private:
    Ui::dataaccess *ui;
};

#endif // DATAACCESS_H
