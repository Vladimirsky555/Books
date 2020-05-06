#include "datatypes.h"

#include <QIODevice>
#include <QDataStream>


BookItem::BookItem(QString name) : QObject(0){
    this->name = name;
}

QString BookItem::getName()
{
    return this->name;
}

void BookItem::setName(QString name)
{
    this->name = name;
}


ListItem* BookItem::getItemById(int id){
    if(id < items.count()){
    return items[id];
    }
    return NULL;
}

void BookItem::replaceData(QString oldName, QString newName)
{
    for(int i = 0; i < items.count(); i++){
        if(items[i]->getName().contains(oldName)){
            items[i]->setName(newName);
            return;
        }
    }
}

void BookItem::insertData(QString source, QString newText){
    ListItem *tmp = new ListItem(newText);

    for(int i = 0; i < items.count(); i++){
        if(items[i]->getName().contains(source)){
            items.insert(i+1,tmp);
            return;
        }
    }

}

void BookItem::insertDataFirst(QString newText)
{
    ListItem *tmp = new ListItem(newText);
    items.insert(0,tmp);
}

void BookItem::putData(QString name)
{
    ListItem* li = new ListItem(name);
    items.push_back(li);
}

void BookItem::delItem(ListItem *item)
{
    items.removeOne(item);
}

void BookItem::delItemById(int id)
{
    items.removeAt(id);
}

int BookItem::getItemsCount()
{
    return items.size();
}

void BookItem::exchange(int id)
{
    if(id < items.count() - 1){
        tmp = items[id];
        items[id] = items[id + 1];
        items[id + 1] = tmp;
    }
}

QList<ListItem*> BookItem::getAllItems()
{
    return this->items;
}

void BookItem::clearItems(){
    items.clear();
}

ListItem *BookItem::getItemByName(QString value)
{
    for(int i = 0; i < items.size(); i++){
        if(items[i]->getName() == value){
            return items[i];
        }
    }

    return NULL;
}


BookItem::BookItem(QByteArray arr, QObject *parent) : QObject(parent){
    QDataStream str(&arr, QIODevice::ReadOnly);

    str >> this->name;

    while(!str.atEnd()){
        QByteArray tmp;
        str >> tmp;
       items.append(new ListItem(tmp));
    }

}

QByteArray BookItem::saveIt(){
    QByteArray arr;
    QDataStream str(&arr, QIODevice::WriteOnly);

    str << this->name;

    for(int i = 0; i < items.size(); i++){
        str << this->items[i]->saveIt();
    }

    return arr;
}
