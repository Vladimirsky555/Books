#ifndef LIST_H
#define LIST_H

#include <QDialog>

#include "data/datatypes.h"
#include "helpers/qregexphighlighter.h"

namespace Ui {
class List;
}

class List : public QDialog
{
    Q_OBJECT

    QList<ListItem*> items;
    ListItem* current;
    QRegexpHighlighter *highlighter;

public:
    explicit List(QString title, QList<ListItem*> items, QWidget *parent = 0);
    ~List();

private slots:
    void on_edtPattern_textChanged(const QString &arg1);
    void shutdown();

private:
    Ui::List *ui;
};

#endif // LIST_H
