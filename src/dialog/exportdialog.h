#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>

#include "data/storage.h"
#include "data/catalog.h"
#include "data/bookitem.h"

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT

    Storage *s;

    Catalog *catalogTo;
    Catalog *catalogFrom;
    BookItem *book;//книга для экспорта

    Catalog *catalog;
    BookItem *bookTo;
    BookItem *bookFrom;
    ListItem *chapter;//раздел для экспорта


public:
      ExportDialog(Storage *s, Catalog *catalog, BookItem* book, QWidget *parent = nullptr);
      ExportDialog(Catalog *catalog, BookItem* book, ListItem* chapter, QWidget *parent = nullptr);
    ~ExportDialog();

private slots:
    void on_lstExport_clicked(const QModelIndex &index);

//signals:
//    void changed();

private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
