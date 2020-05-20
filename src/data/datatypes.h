#ifndef DATATYPES_H
#define DATATYPES_H

#include <QObject>
#include <QDate>

class BookItem;
class ListItem;
class TextItem;

//Каталог книг, которому соответствует бинарный файл в ресурсах, поэтому load
//Загружается при запуске программы
class Catalog {
public:
    QList<BookItem*> books;
    QString path;

public:
    Catalog(){}
};


//Книга
class BookItem : QObject{
private:
    QString name;
    QList<ListItem*> items;

public:
    BookItem(QString);
    BookItem(QByteArray arr, QObject *parent = nullptr);

    //User
    QString getName();
    ListItem* getItemById(int);
    ListItem* getItemByName(QString value);
    void putData(QString);
    int getItemsCount();
    QList<ListItem*> getAllItems();
    void clearItems();
    QByteArray saveIt();

    //admin
    void setName(QString);
    void replaceData(QString,QString);
    void insertData(QString,QString);
    void delItem(ListItem*);
    void delItemById(int);
    void up(int id);
    void down(int id);
};



//Главы книги
class ListItem : QObject{
private:
    QString name;
    QList<TextItem*> data;

public:
    ListItem(QString);
    ListItem(QByteArray arr, QObject *parent = nullptr);

    //user
    QString getName();
    TextItem* getItemById(int);
    TextItem* getItemByName(QString value);
    void putData(QString);
    int getItemsCount();
    void clearData();
    QByteArray saveIt();

    //admin
    void setName(QString);
    void replaceData(QString,QString);
    void insertData(QString,QString);
    void delItem(TextItem*);
    void delItemById(int);
    void up(TextItem*item);
    void down(TextItem*item);
};


//Разделы книги и текст
class TextItem : QObject{
private:
    QString name;
    QString data;

public:
    TextItem(QString);
    TextItem(QByteArray arr, QObject *parent = nullptr);

    //user
    QString getName();
    QString getData();
    void setName(QString);
    QByteArray saveIt();

    //admin
    void setData(QString);

};


//Дневник для записей
class ItemInfo : public QObject
{
    Q_OBJECT

    QString book;
    QString title;
    QString theme;
    QDate date;
    QString text;

public:
    explicit ItemInfo(QObject *parent = NULL);
    ItemInfo(QByteArray data, QObject *parent = NULL);
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
