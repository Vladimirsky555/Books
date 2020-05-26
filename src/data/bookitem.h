#ifndef BOOKITEM_H
#define BOOKITEM_H

#include <QObject>
#include "listitem.h"

class BookItem : public QObject
{
    Q_OBJECT

    QString bookName;
    QList<ListItem*> chapters;


public:
    explicit BookItem(QString, QObject *parent = nullptr);
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

signals:

};

#endif // BOOKITEM_H
