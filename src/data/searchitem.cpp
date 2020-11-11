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



