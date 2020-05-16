#ifndef DATATYPES_H
#define DATATYPES_H

#include <QObject>
#include <QDate>

class TextItem : QObject{
private:
    QString name;
    QString data;

public:
    TextItem(QString);
    TextItem(QByteArray arr, QObject *parent = nullptr);

    QString getName();
    QString getData();
    void setName(QString);
    void setData(QString);
    QByteArray saveIt();
};

class ListItem : QObject{
private:
    QString name;
    QList<TextItem*> data;
    TextItem *tmp;

public:
    ListItem(QString);
    ListItem(QByteArray arr, QObject *parent = nullptr);

    QString getName();
    void setName(QString);
    void replaceData(QString,QString);
    void insertData(QString,QString);
    void insertDataFirst(QString);
    TextItem* getItemById(int);
    TextItem* getItemByName(QString value);
    void putData(QString);    
    void delItem(TextItem*);
    void delItemById(int);
    void exchange(TextItem*);
    int getItemsCount();
    void clearData();
    QByteArray saveIt();
};


class BookItem : QObject{
private:
    QString name;
    QList<ListItem*> items;
    ListItem *tmp;

public:
    BookItem(QString);
    BookItem(QByteArray arr, QObject *parent = nullptr);

    QString getName();
    void setName(QString);
    void replaceData(QString,QString);
    void insertData(QString,QString);
    void insertDataFirst(QString);
    ListItem* getItemById(int);
    ListItem* getItemByName(QString value);
    void putData(QString);
    void delItem(ListItem*);
    void delItemById(int);
    void exchange(int);
    int getItemsCount();
    QList<ListItem*> getAllItems();
    void clearItems();
    QByteArray saveIt();
};

//Каталог книг, которому соответствует бинарный файл в ресурсах, поэтому load
//Загружается при запуске программы
class loadItem {
public:
    QList<BookItem*> books;
    QString path;

public:
    loadItem(){}
};


class ItemInfo : public QObject
{
    Q_OBJECT

    QString book;
    QString title;
    QString theme;
    QDate date;
    QString text;

public:
    explicit ItemInfo(QObject *parent = nullptr);
    ItemInfo(QByteArray data, QObject *parent = nullptr);
    ItemInfo(QString book, QString title, QString theme, QDate date, QString text, QObject *parent = NULL);

    void setBook(QString book);
    void setTitle(QString title);
    void setTheme(QString theme);
    void setDate(QDate date);
    void setText(QString text);

    QString getBook();
    QString getTitle();
    QString getTheme();
    QDate getDate();
    QString getText();
    QString getView();
    QString getView_without_date();

    QByteArray save();
};
#endif // DATATYPES_H
