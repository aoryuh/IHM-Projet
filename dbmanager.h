#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

/**
 * \class DbManager
 *
 * \brief SQL Database Manager class
 *
 * DbManager sets up the connection with SQL database
 * and performs some basics queries. The class requires
 * existing SQL database. You can create it with sqlite:
 * 1. $ sqlite3 p1_db.db
 * 2. sqilte> CREATE TABLE people(ids integer primary key, name text);
 * 3. sqlite> .quit
 */
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
