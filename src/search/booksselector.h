#ifndef BOOKSSELECTOR_H
#define BOOKSSELECTOR_H

#include <QDialog>
#include <QTimer>

#include "data/storage.h"
#include "data/catalog.h"
#include "data/bookitem.h"

namespace Ui {
class BooksSelector;
}

class BooksSelector : public QDialog
{
    Q_OBJECT

    Storage *s;
    Catalog *currentCatalog;
    BookItem *currentBook;

    QList<BookItem*> booksSource;
    QList<BookItem*> booksDest;//сформированный список для поиска

public:
    explicit BooksSelector(Storage *s, QWidget *parent = 0);
    ~BooksSelector();

    void refreshSource();
    void refreshDest();

private slots:
    void on_lstSource_clicked(const QModelIndex &index);
    void on_lstDest_clicked(const QModelIndex &index);
    void selectBooks();

signals:
    void select(QList<BookItem*>);

private:
    Ui::BooksSelector *ui;
};

#endif // BOOKSSELECTOR_H
