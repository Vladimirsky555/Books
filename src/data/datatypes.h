#ifndef DATATYPES_H
#define DATATYPES_H

#include <QObject>
#include <QDate>

class BookItem;
class ListItem;
class TextItem;
class Catalog;

class Storage : QObject {
public:
    QList<Catalog*> catalogs;

public:
    Storage();

    Catalog* getCatalogById(int id);
    BookItem* getBookByName(QString name);
//    QString getNameById(int id);
//    QString getPathById(int id);
    QList<Catalog*> Catalogs();
    QStringList getPathList();
    QStringList getNameList();
    int getCount();
};

//Каталог книг, которому соответствует бинарный файл в ресурсах, поэтому load
//Загружается при запуске программы
class Catalog : QObject {
private:
    QList<BookItem*> books;
    QString name;
    QString path;
//    bool newCatalog;


public:
    Catalog(QString name, QString path);

    //User
    QList<BookItem *> Books();
    void setBook(QList<BookItem*> books);
    BookItem* getBookById(int id);
    BookItem *getBookByName(QString name);
    QString getName();
    void setCatalogName(QString name);
    int getCount();
    QString getPath();
    void setPath(QString path);

    //Admin
//    bool isNew();
//    void setNew(bool flag);
    void insertDataFirst(QString newText);
    void insertData(QString source, QString newText);
    void insertDataAtEnd(QString name);
    void up(int id);
    void down(int id);
    void deleteBook(BookItem* book);
};


//Книга
class BookItem : QObject{
private:
    QString bookName;
    QList<ListItem*> chapters;

public:
    BookItem(QString);
    BookItem(QByteArray arr, QObject *parent = nullptr);

    //User
    QString getName();
    ListItem* getChapterById(int);
    ListItem* getChapterByName(QString value);
    int getCount();
   QList<ListItem *> getChapters();//Используется для вывода содержания
    QByteArray saveIt();

    //admin
    void insertDataFirst(QString newText);
    void insertData(QString,QString);
    void insertDataAtEnd(QString);
    void setName(QString);
    void up(int id);
    void down(int id);
    void deleteChapter(ListItem*);
    void deleteChapterById(int);
};


//Главы книги
class ListItem : QObject{
private:
    QString chapterName;
    QList<TextItem*> sections;

public:
    ListItem(QString);
    ListItem(QByteArray arr, QObject *parent = nullptr);

    //user
    QString getName();
    TextItem* getSectionById(int);
    TextItem* getSectionByName(QString value);
    int getCount();
    QByteArray saveIt();

    //admin
    void insertDataFirst(QString newText);
    void insertData(QString source, QString newText);
    void insertDataAtEnd(QString);
    void setName(QString);
    void up(TextItem*item);
    void down(TextItem*item);
    void deleteSection(TextItem*item);
    void deleteSectionById(int id);
};


//Разделы книги и текст
class TextItem : QObject{
private:
    QString sectionName;
    QString text;

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
