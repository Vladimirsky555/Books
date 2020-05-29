#include "storage.h"

Storage::Storage(QObject *parent) : QObject(parent)
{

}

Catalog* Storage::getCatalogById(int id)
{
    if(id < catalogs.count()){
        return catalogs[id];
    }
    return NULL;
}

Catalog *Storage::getCatalogByPath(QString path)
{
    for(int i = 0; i < catalogs.size(); i++){
        if(catalogs[i]->getPath() == path){
            return catalogs[i];
        }
    }

    return NULL;
}


BookItem *Storage::getBookByName(QString name)
{
    BookItem *currentBook = NULL;
    for(int i = 0; i < catalogs.count(); i++){
        for(int j = 0; j < catalogs[i]->getCount(); j++){

            currentBook = catalogs[i]->getBookById(j);
            if(currentBook->getName() == name){
                return currentBook;
            }
        }
    }

    return currentBook;
}

//QString Storage::getNameById(int id)
//{
//    return catalogs.at(id)->getName();
//}

//QString Storage::getPathById(int id)
//{
//    return catalogs.at(id)->getPath();
//}

QList<Catalog *> Storage::Catalogs()
{
    return this->catalogs;
}

void Storage::addCatalog(Catalog* catalog)
{
    catalogs.append(catalog);
}

void Storage::addAtTheEndOfCatalog(Catalog *catalog)
{
    catalogs.push_back(catalog);
}

void Storage::renameCatalog(Catalog *catalog, QString name)
{
    catalog->setName(name);
}

QStringList Storage::getPathList()
{
    QStringList pathList;
    for(int i = 0; i < catalogs.count(); i++)
    {
        pathList.append(catalogs.at(i)->getPath());
    }

    return pathList;
}

QStringList Storage::getNameList()
{
    QStringList namesList;
    for(int i = 0; i < catalogs.count(); i++)
    {
        namesList.append(catalogs.at(i)->getName());
    }

    return namesList;
}

void Storage::deleteCatalog(Catalog *catalog)
{
    if(catalog != NULL)
    catalogs.removeOne(catalog);
    delete catalog;
}

void Storage::up(int id)
{
    Catalog *tmp;
    if(id > 0){
        tmp = catalogs[id];
        catalogs[id] = catalogs[id - 1];
        catalogs[id - 1] = tmp;
    }
}

void Storage::down(int id)
{
    Catalog *tmp;
    if(id < catalogs.count() - 1){
        tmp = catalogs[id];
        catalogs[id] = catalogs[id + 1];
        catalogs[id + 1] = tmp;
    }
}

int Storage::getCount()
{
    return catalogs.count();
}


