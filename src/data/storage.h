#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QDataStream>

#include "catalog.h"

class Storage : public QObject
{
    Q_OBJECT

    QList<Catalog*> catalogs;
    QList<BookItem*> currentBooks;//Массив книг каталога

public:
    QStringList pathList;//Массив путей к каталогам
    QStringList nameList;

    QList<Catalog*> catalogsList;//для поиска по каталогам или одному каталогу
    QList<BookItem*> booksList;//Список книг для поиска


public:
    Storage(QObject *parent = nullptr);

    //Загрузка и сохранение в файл
    void loadNamePathList();
    void saveNamePathList();
    void loadData(QString path);
    void saveData();


    Catalog* getCatalogById(int id);
    Catalog* getCatalogByPath(QString path);
    Catalog* getCatalogByBook(BookItem* book);
    Catalog* getCatalogByName(QString name);
    BookItem* getBookByName(QString name);

    QList<BookItem*> getCurrentBooks();
    void setCurrentBooks(QList<BookItem*> currentBooks);

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
