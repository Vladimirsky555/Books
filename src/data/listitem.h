#ifndef LISTITEM_H
#define LISTITEM_H

#include <QObject>
#include "textitem.h"

class ListItem : public QObject
{
    Q_OBJECT

    QString chapterName;
    QList<TextItem*> sections;

public:
    explicit ListItem(QString, QObject *parent = nullptr);
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

signals:

};

#endif // LISTITEM_H
