#include "datatypes.h"

Catalog::Catalog(QString name, QString path) : QObject(NULL)
{
    this->name = name;
    this->path = path;
}


QString Catalog::getName()
{
    return this->name;
}

void Catalog::setCatalogName(QString name)
{
    this->name = name;
}

int Catalog::getCount()
{
    return books.count();
}

QString Catalog::getPath()
{
    return this->path;
}

void Catalog::setPath(QString path)
{
    this->path = path;
}

//bool Catalog::isNew()
//{
//    return this->newCatalog;
//}

//void Catalog::setNew(bool flag)
//{
//    this->newCatalog = flag;
//}


QList<BookItem *> Catalog::Books()
{
    return this->books;
}

void Catalog::setBook(QList<BookItem *> books)
{
    this->books = books;
}

BookItem *Catalog::getBookById(int id)
{
    if(id < books.count()){
    return books[id];
    }
    return NULL;
}

BookItem *Catalog::getBookByName(QString name)
{
    for(int i = 0; i < books.size(); i++){
        if(books[i]->getName() == name){
            return books[i];
        }
    }

    return NULL;
}

void Catalog::insertDataFirst(QString newText)
{
    BookItem *tmp = new BookItem(newText);
    books.insert(0,tmp);
}

void Catalog::insertData(QString source, QString newText)
{
    BookItem *tmp = new BookItem(newText);

    for(int i = 0; i < books.count(); i++){
        if(books[i]->getName().contains(source)){
            books.insert(i+1,tmp);
            return;
        }
    }
}

void Catalog::insertDataAtEnd(QString name)
{
    BookItem* bi = new BookItem(name);
    books.push_back(bi);
}

void Catalog::up(int id)
{
    BookItem *tmp;
    if(id > 0){
        tmp = books[id];
        books[id] = books[id - 1];
        books[id - 1] = tmp;
    }
}

void Catalog::down(int id)
{
    BookItem *tmp;
    if(id < books.count() - 1){
        tmp = books[id];
        books[id] = books[id + 1];
        books[id + 1] = tmp;
    }
}

void Catalog::deleteBook(BookItem *book)
{
    if(book == NULL)return;
    books.removeOne(book);
    delete book;
}
