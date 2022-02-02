#include <dbmanager.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

DbManager::DbManager()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    static const QString path = "../IHM-Projet/p1_db.db";
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Error: connection with database fail";
    }
    else
    {
        qDebug() << "Database: connection ok";
    }
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

bool DbManager::createTable()
{
    bool success = false;

    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS images(ref_id TEXT PRIMARY KEY, name TEXT);");

    if (!query.exec())
    {
        qDebug() << "Couldn't create the table 'images': one might already exist.";
        success = false;
    }

    return success;
}

bool DbManager::addImageData(const QString& name, const QString& ref_id)
{
    bool success = false;

    if (!name.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO images (ref_id, name) VALUES (:ref_id, :name)");
        queryAdd.bindValue(":name", name);
        queryAdd.bindValue(":ref_id", ref_id);

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "add image failed: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "add image failed: name cannot be empty";
    }

    return success;
}

bool DbManager::removeImageData(const QString& name)
{
    bool success = false;

    if (imageDataExists(name))
    {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM people WHERE name = (:name)");
        queryDelete.bindValue(":name", name);
        success = queryDelete.exec();

        if(!success)
        {
            qDebug() << "remove image failed: " << queryDelete.lastError();
        }
    }
    else
    {
        qDebug() << "remove image failed: image doesnt exist";
    }

    return success;
}

void DbManager::getAllImageData() const
{
    qDebug() << "images in db:";
    QSqlQuery query("SELECT * FROM people");
    int idName = query.record().indexOf("name");
    while (query.next())
    {
        QString name = query.value(idName).toString();
        qDebug() << "===" << name;
    }
}

bool DbManager::imageDataExists(const QString& name) const
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT name FROM people WHERE name = (:name)");
    checkQuery.bindValue(":name", name);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }
    else
    {
        qDebug() << "image exists failed: " << checkQuery.lastError();
    }

    return exists;
}

bool DbManager::removeAllImageData()
{
    bool success = false;

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM people");

    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "remove all images failed: " << removeQuery.lastError();
    }

    return success;
}
