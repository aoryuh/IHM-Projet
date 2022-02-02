#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QSqlDatabase>

class DbManager
{
public:

    DbManager();

    ~DbManager();

    bool isOpen() const;

    bool createTable();

    bool addImageData(const QString& name, const QString& ref_id);

    bool removeImageData(const QString& name);

    bool imageDataExists(const QString& name) const;

    void getAllImageData() const;

    bool removeAllImageData();

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
