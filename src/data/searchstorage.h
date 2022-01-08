#ifndef SEARCHSTORAGE_H
#define SEARCHSTORAGE_H

#include <QObject>

#include "data/searchitem.h"

class SearchStorage : public QObject
{
    Q_OBJECT
public:
    QList<searchItem*> searchItems; //Массив текстов, формирующийся по результатам поиска

public:
    SearchStorage(QObject *parent = nullptr);

    //////////// Раздел текстов ////////////
    //Фунции для организации поиска
    void destroySearchList();
    void addSearchItem(int cnt, Catalog *cat, BookItem *book,
                       ListItem *chapter, TextItem *section);

    void sortResult();

};

#endif // SEARCHSTORAGE_H
