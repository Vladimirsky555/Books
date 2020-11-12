#ifndef CATALOGSWORKER_H
#define CATALOGSWORKER_H

#include <QObject>
#include <QRunnable>

#include "data/storage.h"
#include "data/searchitem.h"

class CatalogsWorker : QObject, public QRunnable
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
    CatalogsWorker(Storage *s, Catalog *currentCatalog, QString searchText,
                   QObject *parent = nullptr);

    void run();

private:
    bool checkRegExp(QRegExp rx);
};

#endif // CATALOGSWORKER_H
