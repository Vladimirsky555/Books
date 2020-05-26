#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>

#include "data/storage.h"
#include "data/bookitem.h"


class dataLoader : public QObject
{
    Q_OBJECT

    Storage *s;
    QList<BookItem*> currentBooks;
    QStringList nameList;
    QStringList pathList;


public:
    dataLoader(Storage *s, QStringList nameList, QStringList pathList, QObject *parent = nullptr);
    virtual ~dataLoader(){}

    void loadCatalog(QString path);
    void loadData();

signals:

};

#endif // DATALOADER_H
