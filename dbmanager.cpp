#include <dbmanager.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

DbManager::DbManager()
{

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
    query.prepare("CREATE TABLE IF NOT EXISTS images(ref_id TEXT PRIMARY KEY, name TEXT, color TEXT, theme TEXT, height INTEGER, width INTEGER);");

    if (!query.exec())
    {
        qDebug() << "Couldn't create the table 'images': one might already exist.";
        success = false;
    }

    return success;
}

bool DbManager::addImageData(const QString& ref_id, const QString& name, const QString& color, const QString& theme, const QString& height, const QString& width)
{
    bool success = false;
    //add -> if already exist if (!name.isEmpty() && imageDataExists(name))
    if (!name.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO images (ref_id, name, color, theme, height, width) VALUES (:ref_id, :name, :color, :theme, :height, :width)");
        queryAdd.bindValue(":ref_id", ref_id);
        queryAdd.bindValue(":name", name);
        queryAdd.bindValue(":color", color);
        queryAdd.bindValue(":theme", theme);
        queryAdd.bindValue(":height", height);
        queryAdd.bindValue(":width", width);

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
        queryDelete.prepare("DELETE FROM images WHERE name = (:name)");
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
    QSqlQuery query("SELECT * FROM images");
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
    checkQuery.prepare("SELECT name FROM images WHERE name = (:name)");
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
    removeQuery.prepare("DELETE FROM images");

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

void DbManager::initialize(const QString& path){
    m_db = QSqlDatabase::addDatabase("QSQLITE");
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
