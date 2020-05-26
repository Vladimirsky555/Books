#include "dataloader.h"

#include <QFile>
#include <QDataStream>

dataLoader::dataLoader(Storage *s, QStringList nameList, QStringList pathList, QObject *parent)
    : QObject(parent)
{
    this->s = s;
    this->nameList = nameList;
    this->pathList = pathList;
}


void dataLoader::loadCatalog(QString path)
{
    currentBooks.clear();
    QFile f(path);
    if(!f.exists()) return;

    f.open(QFile::ReadOnly);
    QDataStream reader(&f);

    while(!reader.atEnd()){
        QByteArray arr;
        reader >> arr;
        currentBooks.append(new BookItem(arr));
    }

    f.close();
}

void dataLoader::loadData()
{
    //Загружаем все файлы, только один раз
    for(int i = 0; i < pathList.size(); i++)
    {
        Catalog *catalog = new Catalog(nameList[i], pathList[i]);
        loadCatalog(pathList[i]);
        catalog->setBook(currentBooks);
        s->addCatalog(catalog);
    }
}


