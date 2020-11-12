#ifndef BOOKSWORKER_H
#define BOOKSWORKER_H

#include <QObject>
#include <QRunnable>

#include "data/storage.h"
#include "data/searchitem.h"

class BooksWorker : public QObject, public QRunnable
{
    Q_OBJECT

    Storage *s;
    Catalog *currentCatalog;
    BookItem* currentBook;
    ListItem* currentChapter;
    TextItem* currentSection;
    QString searchText;
    int c;

public:
    BooksWorker(Storage *s, BookItem *currentBook,
                QString searchText, QObject *parent = nullptr);

    void run();

private:
    bool checkRegExp(QRegExp rx);
};

#endif // BOOKSWORKER_H
