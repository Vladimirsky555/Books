#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>
#include "catalog.h"

class Storage : public QObject
{
    Q_OBJECT

    QList<Catalog*> catalogs;


public:
    explicit Storage(QObject *parent = nullptr);

    Catalog* getCatalogById(int id);
    BookItem* getBookByName(QString name);
    QList<Catalog*> Catalogs();
    void addCatalog(Catalog *catalog);
    void addAtTheEndOfCatalog(Catalog *catalog);
    void renameCatalog(Catalog *catalog, QString name);
    QStringList getPathList();
    QStringList getNameList();
    void deleteCatalog(Catalog *catalog);
    void up(int id);
    void down(int id);
    int getCount();

signals:

};

#endif // STORAGE_H
