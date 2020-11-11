#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QRunnable>


#include "data/storage.h"
#include "data/searchitem.h"

class Worker : public QObject, public QRunnable
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
    Worker(Storage *s, Catalog *currentCatalog, QString searchText,
           QObject *parent = nullptr);

//    int findInOneText(int *c, QString txt);
    void run();

private:
    bool checkRegExp(QRegExp rx);

signals:
    void sendCount(int c);
};

#endif // WORKER_H
