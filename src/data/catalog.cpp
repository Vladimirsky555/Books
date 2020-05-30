#include "catalog.h"

#include <QDataStream>
#include <QIODevice>


Catalog::Catalog(QString name, QString path, QObject *parent) : QObject(parent)
{
    this->name = name;
    this->path = path;
}

Catalog::Catalog(QByteArray arr, QObject *parent) :
    QObject(parent){
    {
        QDataStream str(&arr, QIODevice::ReadOnly);

        str >> this->name >> this->path;

        while(!str.atEnd()){
            QByteArray tmp;
            str >> tmp;
            books.append(new BookItem(tmp));
        }
    }
}


QString Catalog::getName()
{
    return this->name;
}

void Catalog::setName(QString name)
{
    this->name = name;
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

void Catalog::addBook(BookItem *book)
{
    if(book != NULL)
    books.push_back(book);
}


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

BookItem *Catalog::insert_Duplicate(QString name)
{
    BookItem* book = new BookItem(name);
    books.push_back(book);
    return book;
}

QStringList Catalog::getNameList()
{
    QStringList namesList;
    for(int i = 0; i < books.count(); i++)
    {
        namesList.append(books.at(i)->getName());
    }

    return namesList;
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

void Catalog::removeBook(BookItem *book)
{
    if(book == NULL)return;
    books.removeOne(book);
}

QByteArray Catalog::saveIt()
{
    QByteArray arr;
    QDataStream str(&arr, QIODevice::WriteOnly);

    str << this->name << path;

    for(int i = 0; i < books.size(); i++){
        str << this->books[i]->saveIt();
    }

    return arr;
}
