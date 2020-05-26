#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>

#include "data/storage.h"
#include "data/bookitem.h"

class dataLoader : public QObject
{
    Q_OBJECT

    Storage *s;
    QList<BookItem*> books;
    QStringList nameList;
    QStringList pathList;

public:
    dataLoader(Storage *s, QObject *parent = nullptr);
    virtual ~dataLoader(){}

    void loadNameList();
    QStringList getNameList();
    void loadCatalog(QString path);
    void loadData();

    void saveCatalogs();
    void saveData();
};

#endif // DATALOADER_H
