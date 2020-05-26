#include "dataloader.h"

#include <QFile>
#include <QDataStream>

dataLoader::dataLoader(Storage *s, QObject *parent)
    : QObject(parent)
{
    this->s = s;
}

void dataLoader::loadNameList()
{
    QFile f("catalogs");
    if(!f.exists()) return;

    f.open(QFile::ReadOnly);
    QDataStream str(&f);

    int i = 0;
    while(!str.atEnd()){
        QString tmp;
        str >> tmp;
        nameList.append(tmp);

        str >> tmp;
        pathList.append(tmp);
        i++;
    }

    f.close();
}

QStringList dataLoader::getNameList()
{
    return nameList;
}


void dataLoader::loadCatalog(QString path)
{
    books.clear();
    QFile f(path);
    if(!f.exists()) return;

    f.open(QFile::ReadOnly);
    QDataStream reader(&f);

    while(!reader.atEnd()){
        QByteArray arr;
        reader >> arr;
        books.append(new BookItem(arr));
    }

    f.close();
}

void dataLoader::loadData()
{
    //Загружаем все файлы, только один раз
    for(int i = 0; i < pathList.size(); i++)
    {
        Catalog *catalog = new Catalog(nameList.at(i), pathList[i]);
        loadCatalog(pathList[i]);
        catalog->setBook(books);
        s->addCatalog(catalog);
    }
}

void dataLoader::saveCatalogs()
{
    QFile f("catalogs");
    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream str(&f);

    for(int i = 0; i < s->getCount(); i++){
        str << s->getCatalogById(i)->getName() << s->getCatalogById(i)->getPath();
    }

    f.close();
}

void dataLoader::saveData()
{
    for(int i = 0; i < s->getCount(); i++){

    QFile f(s->getCatalogById(i)->getPath());
    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream str(&f);

    books = s->getCatalogById(i)->Books();
    for(int i = 0; i < books.size(); i++){
        str << books[i]->saveIt();
    }

    f.close();
    }
}


