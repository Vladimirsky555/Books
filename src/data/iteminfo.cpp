#include "datatypes.h"

#include <QIODevice>
#include <QDataStream>

ItemInfo::ItemInfo(QObject *parent) :
    QObject(parent)
{
    this->book = "";
    this->title = "";
    this->theme = "";
    this->date = QDate::currentDate();
    this->text = "";
}

ItemInfo::ItemInfo(QByteArray data, QObject *parent)  : QObject(parent)
{
    QDataStream str(&data, QIODevice::ReadOnly);
    str >> book >> title >> theme >> date >> text;
}

void ItemInfo::setBook(QString book)
{
    this->book = book;
}

ItemInfo::ItemInfo(QString book, QString title, QString theme, QDate date, QString text, QObject *parent)
    : QObject(parent) {
    this->book = book;
    this->title = title;
    this->theme = theme;
    this->date = date;
    this->text = text;
}

void ItemInfo::setTitle(QString title)
{
    this->title = title;
}

void ItemInfo::setTheme(QString theme)
{
    this->theme = theme;
}

void ItemInfo::setDate(QDate date)
{
    this->date = date;
}

void ItemInfo::setText(QString text)
{
    this->text = text;
}

QString ItemInfo::getBook()
{
    return this->book;
}

QString ItemInfo::getTitle()
{
    return this->title;
}

QString ItemInfo::getTheme()
{
    return theme;
}

QDate ItemInfo::getDate()
{
    return date;
}

QString ItemInfo::getText()
{
    return text;
}

QByteArray ItemInfo::save()
{
    QByteArray arr;
    QDataStream str(&arr, QIODevice::WriteOnly);
    str << book << title << theme << date << text;
    return arr;
}

QString ItemInfo::getView()
{
    return date.toString(Qt::ISODate) + ": " +book + ", " + title + ", " + theme ;
}

QString ItemInfo::getView_without_date()
{
     return book + ", " + title + ", " + theme ;
}

