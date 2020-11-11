#ifndef SEARCHITEM_H
#define SEARCHITEM_H

#include <QObject>

#include "catalog.h"
#include "bookitem.h"
#include "listitem.h"
#include "textitem.h"

class SearchItem : public QObject
{
    Q_OBJECT

public:
    Catalog *_catalog;
    BookItem *_book;
    ListItem *_chapter;
    TextItem *_section;
    int textCount;

public:
    SearchItem(QObject *parent = 0);


};

#endif // SEARCHITEM_H
