#include "textitem.h"

#include <QDataStream>
#include <QIODevice>

TextItem::TextItem(QString name, QObject *parent) : QObject(parent)
{
   this->name = name;
   this->text = "";
}

TextItem::TextItem(QByteArray arr, QObject *parent) : QObject(parent){
    QDataStream reader(&arr, QIODevice::ReadOnly);
    reader >> this->name >> this->text;
}



QString TextItem::getName(){
    return name;
}

QString TextItem::getData(){
    return text;
}

void TextItem::setName(QString name)
{
    this->name = name;
}

void TextItem::setData(QString data)
{
    this->text = data;
}

QByteArray TextItem::saveIt()
{
    QByteArray arr;
    QDataStream writer(&arr, QIODevice::WriteOnly);
    writer << this->name << this->text;
    return arr;
}

