#include "bookitem.h"

#include <QIODevice>
#include <QDataStream>

BookItem::BookItem(QString name, QObject *parent) : QObject(parent)
{
     this->bookName = name;
}


QString BookItem::getName()
{
    return this->bookName;
}

void BookItem::setName(QString name)
{
    this->bookName = name;
}


ListItem* BookItem::getChapterById(int id){
    if(id < chapters.count()){
    return chapters[id];
    }
    return NULL;
}

void BookItem::insertData(QString source, QString newText){
    ListItem *tmp = new ListItem(newText);

    for(int i = 0; i < chapters.count(); i++){
        if(chapters[i]->getName().contains(source)){
            chapters.insert(i+1,tmp);
            return;
        }
    }
}

void BookItem::insertDataFirst(QString newText)
{
    ListItem *tmp = new ListItem(newText);
    chapters.insert(0,tmp);
}


void BookItem::insertDataAtEnd(QString name)
{
    ListItem* li = new ListItem(name);
    chapters.push_back(li);
}

void BookItem::deleteChapter(ListItem *item)
{
    chapters.removeOne(item);
    delete item;
}

void BookItem::deleteChapterById(int id)
{
    chapters.removeAt(id);
}

void BookItem::up(int id)
{
    ListItem *tmp;
    if(id > 0){
            tmp = chapters[id];
            chapters[id] = chapters[id - 1];
            chapters[id - 1] = tmp;
        }
}

void BookItem::down(int id)
{
    ListItem *tmp;
    if(id < chapters.count() - 1){
        tmp = chapters[id];
        chapters[id] = chapters[id + 1];
        chapters[id + 1] = tmp;
    }
}

int BookItem::getCount()
{
    return chapters.size();
}

QList<ListItem *> BookItem::getChapters()
{
    return this->chapters;
}


ListItem *BookItem::getChapterByName(QString value)
{
    for(int i = 0; i < chapters.size(); i++){
        if(chapters[i]->getName() == value){
            return chapters[i];
        }
    }

    return NULL;
}




BookItem::BookItem(QByteArray arr, QObject *parent) : QObject(parent){
    QDataStream str(&arr, QIODevice::ReadOnly);

    str >> this->bookName;

    while(!str.atEnd()){
        QByteArray tmp;
        str >> tmp;
       chapters.append(new ListItem(tmp));
    }
}

QByteArray BookItem::saveIt(){
    QByteArray arr;
    QDataStream str(&arr, QIODevice::WriteOnly);

    str << this->bookName;

    for(int i = 0; i < chapters.size(); i++){
        str << this->chapters[i]->saveIt();
    }

    return arr;
}
