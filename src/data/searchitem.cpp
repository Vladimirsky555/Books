#include "searchitem.h"

SearchItem::SearchItem(QObject *parent) :
    QObject(parent)
{
    QString tmp = "tmp";
    //Заглушка, чтобы программа не вылетала
    this->_catalog = new Catalog(tmp,tmp);
    this->_book = new BookItem(tmp);
    this->_chapter = new ListItem(tmp);
    this->_section = new TextItem(tmp);
}

Catalog *SearchItem::p_catalog()
{
    return this->_catalog;
}

void SearchItem::setCatalog(Catalog *value)
{
    this->_catalog = value;
}

BookItem *SearchItem::p_book()
{
    return this->_book;
}

void SearchItem::setBook(BookItem *value)
{
    this->_book = value;
}

ListItem *SearchItem::p_chapter()
{
    return this->_chapter;
}

void SearchItem::setChapter(ListItem *value)
{
    this->_chapter = value;
}

TextItem *SearchItem::p_section()
{
    return this->_section;
}

void SearchItem::setSection(TextItem *value)
{
    this->_section = value;
}

int SearchItem::getTextCount()
{
    return this->textCount;
}

void SearchItem::setTextCount(int value)
{
    this->textCount = value;
}



