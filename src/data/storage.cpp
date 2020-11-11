#include "storage.h"
#include <QtAlgorithms>

Storage::Storage(QObject *parent) : QObject(parent)
{
    this->c = 0;
}

SearchItem *Storage::getSearchItem(int id)
{
    return searchItems.at(id);
}

void Storage::addSearchItem(SearchItem *si)
{
    searchItems.append(si);
}

int Storage::getSearchItemsCount()
{
    return searchItems.count();
}

void Storage::sortResult()
{
        SearchItem *tmp = new SearchItem();

        for(int i = 0; i < searchItems.count(); i++)
        {
            for(int j = 0; j < searchItems.count() - 1; j++)
            {
                if(searchItems[j]->textCount < searchItems[j + 1]->textCount)
                {
                    tmp = searchItems[j];
                    searchItems[j] = searchItems[j + 1];
                    searchItems[j + 1] = tmp;
                }
            }
        }

//        delete tmp;
}

void Storage::searchItemsClear()
{
    for(int i = 0; i < searchItems.count(); i++)
    {
        delete searchItems[i];
    }
    searchItems.clear();
}

void Storage::setC(int num)
{
    this->c += num;
}

void Storage::setinZeroC()
{
    this->c = 0;
}

int Storage::getC()
{
    return this->c;
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

Catalog *Storage::getCatalogByBook(BookItem *book)
{
    Catalog *catalog = NULL;
    for(int i = 0; i < catalogs.count(); i++){
        for(int j = 0; j < catalogs[i]->getCount(); j++){
            if(catalogs[i]->getBookById(j) == book){
                return catalogs[i];
            }
        }
    }

    return catalog;
}

Catalog *Storage::getCatalogByName(QString name)
{
    for(int i = 0; i < catalogs.count(); i++){
        if(catalogs[i]->getName() == name)
            return catalogs[i];
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


