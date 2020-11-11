#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>

#include "catalog.h"
#include "searchitem.h"

class Storage : public QObject
{
    Q_OBJECT

    QList<Catalog*> catalogs;
    QList<SearchItem*> searchItems;
    int c;//сколько всего найдено элементов

public:
    Storage(QObject *parent = nullptr);

    //Функции результатов поиска
    SearchItem* getSearchItem(int id);
    void addSearchItem(SearchItem *si);
    int getSearchItemsCount();
    void sortResult();
    void searchItemsClear();
    void setC(int num);
    void setinZeroC();
    int getC();

    //Функции данных
    Catalog* getCatalogById(int id);
    Catalog* getCatalogByPath(QString path);
    Catalog* getCatalogByBook(BookItem* book);
    Catalog* getCatalogByName(QString name);
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
};

#endif // STORAGE_H
