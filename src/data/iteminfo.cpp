#include "datatypes.h"

#include <QIODevice>
#include <QDataStream>

ItemInfo::ItemInfo(QObject *parent) : QObject(0)
{
    this->theme = "";
    this->date = QDate::currentDate();
    this->text = "";
}


ItemInfo::ItemInfo(QByteArray data, QObject *parent)  : QObject(parent) {
    QDataStream reader(&data, QIODevice::ReadOnly);

    reader >> this->theme >> this->date >> this->text;
}

ItemInfo::ItemInfo(QString theme, QDate date, QString text, QObject *parent)  : QObject(parent) {
    this->theme = theme;
    this->date = date;
    this->text = text;
}

void ItemInfo::setTheme(QString theme) {
    this->theme = theme;
}

void ItemInfo::setDate(QDate date) {
    this->date = date;
}

void ItemInfo::setText(QString text) {
    this->text = text;
}

QString ItemInfo::getTheme() {
    return theme;
}

QDate ItemInfo::getDate() {
    return date;
}

QString ItemInfo::getText() {
    return text;
}

QByteArray ItemInfo::save() {
    QByteArray arr;
    QDataStream writer(&arr, QIODevice::WriteOnly);

    writer << this->theme << this->date << this->text;

    return arr;
}

QString ItemInfo::getView() {
    return date.toString() + " - " + theme ;
}
