#ifndef CATALOG_H
#define CATALOG_H

#include <QObject>
#include "bookitem.h"

class Catalog : public QObject
{
    Q_OBJECT

    QList<BookItem*> books;
    QString name;
    QString path;

public:
    explicit Catalog(QString, QString, QObject *parent = nullptr);
    Catalog(QByteArray arr, QObject *parent = nullptr);

    //User
    QList<BookItem *> Books();
    void setBook(QList<BookItem*> books);
    BookItem* getBookById(int id);
    BookItem *getBookByName(QString name);
    QString getName();
    void setName(QString name);
    void setCatalogName(QString name);
    int getCount();
    QString getPath();
    void setPath(QString path);

    //Admin
    void addBook(BookItem* book);
    void insertDataFirst(QString newText);
    void insertData(QString source, QString newText);
    void insertDataAtEnd(QString name);
    void up(int id);
    void down(int id);
    void deleteBook(BookItem* book);
    void removeBook(BookItem* book);//Удаление только из списка

    QByteArray saveIt();

signals:

};

#endif // CATALOG_H
