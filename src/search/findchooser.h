#ifndef FINDCHOOSER_H
#define FINDCHOOSER_H

#include <QDialog>
#include <QTimer>

#include "data/storage.h"
#include "data/catalog.h"
#include "data/bookitem.h"

class QAction;

namespace Ui {
class FindChooser;
}

class FindChooser : public QDialog
{
    Q_OBJECT

    Storage *s;
    Catalog *currentCatalog;
    BookItem *currentBook;

    QList<BookItem*> booksSource;
    QList<BookItem*> booksDest;//сформированный список для поиска

public:
    FindChooser(Storage *s, QWidget *parent = 0);
    virtual ~FindChooser();

    void refreshSource();
    void refreshDest();

private slots:
    void on_lstSource_clicked(const QModelIndex &index);
    void on_lstDest_clicked(const QModelIndex &index);
    void chooseBooks();

signals:
    void choose(QList<BookItem*>);

private:
    Ui::FindChooser *ui;
};

#endif // FINDCHOOSER_H
