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

    Catalog *_catalog;
    BookItem *_book;
    ListItem *_chapter;
    TextItem *_section;
    int textCount;

public:
    SearchItem(QObject *parent = 0);

    Catalog* p_catalog();
    void setCatalog(Catalog *value);

    BookItem* p_book();
    void setBook(BookItem *value);

    ListItem* p_chapter();
    void setChapter(ListItem *value);

    TextItem* p_section();
    void setSection(TextItem *value);

    int getTextCount();
    void setTextCount(int value);

};

#endif // SEARCHITEM_H
