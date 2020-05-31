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

    Storage *s;//список каталогов, в котором приосходят миграции
    Catalog *catalogTo;
    Catalog *catalogFrom;
    BookItem *book;//книга для экспорта в другой каталог, и книга для миграций разделов

    Catalog *catalog;//каталог, в котором происходят миграции
    BookItem *bookTo;
    BookItem *bookFrom;
    ListItem *chapter;//глава для экспорта

    ListItem *chapterTo;
    ListItem *chapterFrom;
    TextItem *section;//раздел для экспорта

public:
    ExportDialog(Storage *s, Catalog *catalogFrom, BookItem* book, QWidget *parent = nullptr);
    ExportDialog(Catalog *catalog, BookItem* bookFrom, ListItem* chapter, QWidget *parent = nullptr);
    ExportDialog(BookItem* book, ListItem* chapterFrom, TextItem *section, QWidget *parent = nullptr);
    ~ExportDialog();

private slots:
    void on_lstExport_clicked(const QModelIndex &index);

private:
    Ui::ExportDialog *ui;
};

#endif // EXPORTDIALOG_H
