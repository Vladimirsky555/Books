#include "datatypes.h"

#include <QIODevice>
#include <QDataStream>

TextItem::TextItem(QString name) : QObject(0){
    this->name = name;
    this->data = "";
}

TextItem::TextItem(QByteArray arr, QObject *parent) : QObject(parent){
    QDataStream reader(&arr, QIODevice::ReadOnly);
    reader >> this->name >> this->data;
}

QString TextItem::getName(){
    return name;
}

QString TextItem::getData(){
    return data;
}

void TextItem::setData(QString data)
{
    this->data = data;
}

QByteArray TextItem::saveIt(){
    QByteArray arr;
    QDataStream writer(&arr, QIODevice::WriteOnly);

    writer << this->name << this->data;

    return arr;

}
