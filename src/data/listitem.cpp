#include "listitem.h"

#include <QIODevice>
#include <QDataStream>

ListItem::ListItem(QString name, QObject *parent) : QObject(parent)
{
this->chapterName = name;
}


QString ListItem::getName(){
    return this->chapterName;
}

void ListItem::setName(QString name){
    this->chapterName = name;
}

void ListItem::insertData(QString source, QString newText)
{
    TextItem *temp = new TextItem(newText);

       for(int i = 0; i < sections.count(); i++){
           if(sections[i]->getName().contains(source)){
               sections.insert(i+1,temp);
               return;
           }
       }
}

void ListItem::deleteSection(TextItem *item)
{
    sections.removeOne(item);
    delete item;
}


TextItem* ListItem::getSectionById(int id){
    if(id < sections.count()){
    return sections[id];
    }
    return NULL;
}

TextItem *ListItem::getSectionByName(QString value)
{
    for(int i = 0; i < sections.size(); i++){
        if(sections[i]->getName() == value){
            return sections[i];
        }
    }

    return NULL;
}

void ListItem::insertDataFirst(QString newText)
{
    TextItem *tmp = new TextItem(newText);
    sections.insert(0,tmp);
}


void ListItem::insertDataAtEnd(QString name)
{
    TextItem* ti = new TextItem(name);
    sections.push_back(ti);
}

void ListItem::deleteSectionById(int id)
{
    sections.removeAt(id);
}

void ListItem::up(TextItem *item)
{
    for(int i = 0; i < sections.count(); i++)
        {
            if(item == sections[i] && i > 0)
            {
                sections[i] = sections[i - 1];
                sections[i - 1] = item;
                return;
            }
        }

        return;
}

void ListItem::down(TextItem *item)
{
    for(int i = 0; i < sections.count(); i++)
    {
        if(item == sections[i] && i < sections.count() - 1)
        {
            sections[i] = sections[i + 1];
            sections[i + 1] = item;
            return;
        }
    }

    return;
}

int ListItem::getCount(){
    return sections.size();
}

//void ListItem::clearData(){
//    sections.clear();
//}

ListItem::ListItem(QByteArray arr, QObject *parent) : QObject(parent){
    QDataStream reader(&arr, QIODevice::ReadOnly);

    reader >> this->chapterName;

    while(!reader.atEnd()){
        QByteArray tmp;
        reader >> tmp;
       sections.append(new TextItem(tmp));
    }

}

QByteArray ListItem::saveIt(){
    QByteArray arr;
    QDataStream writer(&arr, QIODevice::WriteOnly);

    writer << this->chapterName;

    for(int i = 0; i < sections.size(); i++){
        writer << this->sections[i]->saveIt();
    }

    return arr;
}
