#include "datatypes.h"

#include <QIODevice>
#include <QDataStream>

TextItem::TextItem(QString name) : QObject(0){
    this->sectionName = name;
    this->text = "";
}

TextItem::TextItem(QByteArray arr, QObject *parent) : QObject(parent){
    QDataStream reader(&arr, QIODevice::ReadOnly);
    reader >> this->sectionName >> this->text;
}

QString TextItem::getName(){
    return sectionName;
}

QString TextItem::getData(){
    return text;
}

void TextItem::setName(QString name)
{
    this->sectionName = name;
}

void TextItem::setData(QString data)
{
    this->text = data;
}

QByteArray TextItem::saveIt(){
    QByteArray arr;
    QDataStream writer(&arr, QIODevice::WriteOnly);

    writer << this->sectionName << this->text;

    return arr;

}
