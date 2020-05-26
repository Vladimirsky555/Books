#ifndef ITEMINFO_H
#define ITEMINFO_H

#include <QObject>
#include <QDate>

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

#endif // ITEMINFO_H
