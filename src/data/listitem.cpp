#include "datatypes.h"

#include <QIODevice>
#include <QDataStream>


ListItem::ListItem(QString name) : QObject(0){
    this->name = name;
}

QString ListItem::getName(){
    return this->name;
}

void ListItem::setName(QString name){
    this->name = name;
}


TextItem* ListItem::getItemById(int id){
    if(id < data.count()){
    return data[id];
    }
    return NULL;
}

TextItem *ListItem::getItemByName(QString value)
{
    for(int i = 0; i < data.size(); i++){
        if(data[i]->getName() == value){
            return data[i];
        }
    }

    return NULL;
}


void ListItem::putData(QString name){
    TextItem* ti = new TextItem(name);
    data.push_back(ti);
}

void ListItem::delItemById(int id){
    data.removeAt(id);
}

int ListItem::getItemsCount(){
    return data.size();
}

void ListItem::clearData(){
    data.clear();
}

ListItem::ListItem(QByteArray arr, QObject *parent) : QObject(parent){
    QDataStream reader(&arr, QIODevice::ReadOnly);

    reader >> this->name;

    while(!reader.atEnd()){
        QByteArray tmp;
        reader >> tmp;
       data.append(new TextItem(tmp));
    }

}

QByteArray ListItem::saveIt(){
    QByteArray arr;
    QDataStream writer(&arr, QIODevice::WriteOnly);

    writer << this->name;

    for(int i = 0; i < data.size(); i++){
        writer << this->data[i]->saveIt();
    }

    return arr;
}
