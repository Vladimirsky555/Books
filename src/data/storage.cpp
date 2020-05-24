#include "datatypes.h"


Storage::Storage() : QObject(0)
{

}

Catalog* Storage::getCatalogById(int id)
{
    if(id < catalogs.count()){
        return catalogs[id];
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

int Storage::getCount()
{
    return catalogs.count();
}

