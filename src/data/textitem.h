#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QObject>

class TextItem : public QObject
{
    Q_OBJECT

    QString name;
    QString text;

public:
    TextItem(QString name, QObject *parent = nullptr);
    TextItem(QByteArray arr, QObject *parent = nullptr);

    //user
    QString getName();
    QString getData();
    void setName(QString);
    QByteArray saveIt();

    //admin
    void setData(QString);
};

#endif // TEXTITEM_H
