#include "storage.h"


Storage::Storage(QObject *parent) : QObject(parent)
{
    loadNamePathList();

    //Грузим из файла названия каталогов и пути к ним
    for(int i = 0; i < pathList.size(); i++)
    {
        Catalog *catalog = new Catalog(nameList[i], pathList[i]);
        loadData(pathList[i]);
        catalog->setBook(currentBooks);
        addCatalog(catalog);
    }

    this->catalogsList = Catalogs();
}

void Storage::loadNamePathList()
{
    QFile f("data/catalogs");
    if(!f.exists()) {
        QDir dir;
        dir.mkpath("data");
        dir.mkpath("data/doc");
        nameList.append(tr("Первый каталог"));
        pathList.append("data/doc/first_catalog");//Чтобы программа не вылетала временный каталог
    }

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

void Storage::saveNamePathList()
{
    QFile f("data/catalogs");
    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream str(&f);

    for(int i = 0; i < getCount(); i++){
        str << getCatalogById(i)->getName() << getCatalogById(i)->getPath();
    }

    f.close();
}

//Загрузка из одного каталога
void Storage::loadData(QString path)
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

void Storage::saveData()
{
    for(int i = 0; i < getCount(); i++){

    QFile f(getCatalogById(i)->getPath());
    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream str(&f);

    currentBooks = getCatalogById(i)->Books();
    for(int i = 0; i < currentBooks.size(); i++){
        str << currentBooks[i]->saveIt();
    }

    f.close();
    }
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

QList<BookItem*> Storage::getCurrentBooks()
{
    return this->currentBooks;
}

void Storage::setCurrentBooks(QList<BookItem *> currentBooks)
{
    this->currentBooks = currentBooks;
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




