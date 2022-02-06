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

    bool addImageData(const QString& ref_id, const QString& name, const QString& color, const QString& theme, const QString& height, const QString& width);

    bool removeImageData(const QString& name);

    bool imageDataExists(const QString& name) const;

    void getAllImageData() const;

    bool removeAllImageData();

    void initialize(const QString& path);

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
