#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QObject>

class TextItem : public QObject
{
    Q_OBJECT

    QString sectionName;
    QString text;

public:
    explicit TextItem(QString text, QObject *parent = nullptr);
    TextItem(QByteArray arr, QObject *parent = nullptr);

    //user
    QString getName();
    QString getData();
    void setName(QString);
    QByteArray saveIt();

    //admin
    void setData(QString);

signals:

};

#endif // TEXTITEM_H
