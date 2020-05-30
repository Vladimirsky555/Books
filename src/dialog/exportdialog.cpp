#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(Storage *s, Catalog *catalog, BookItem *book, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    this->chapter = NULL;

    setWindowTitle("Список каталогов");

    this->s = s;
    this->book = book;//книга для экспорта
    this->catalogFrom = catalog;

    for(int i = 0; i < s->getCount(); i++)
    {
        ui->lstExport->addItem(s->getNameList().at(i));
        ui->lstExport->item(i)->setIcon(QIcon(":/images/catalog.png"));
    }
}

ExportDialog::ExportDialog(Catalog *catalog, BookItem *book, ListItem *chapter, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);

    setWindowTitle("Список книг");

    this->catalog = catalog;
    this->bookFrom = book;
    this->chapter = chapter;

    for(int i = 0; i < catalog->getCount(); i++)
    {
        ui->lstExport->addItem(catalog->getNameList().at(i));
        ui->lstExport->item(i)->setIcon(QIcon(":/images/book.png"));
    }
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::on_lstExport_clicked(const QModelIndex &index)
{
    if(chapter == NULL){
        catalogTo = s->getCatalogById(index.row());//получаем каталог, в который перенести книгу
        catalogTo->addBook(book);
        catalogFrom->removeBook(book);
        close();
    } else {
        bookTo = catalog->getBookById(index.row());
        bookTo->addChapter(chapter);
        bookFrom->removeChater(chapter);
        close();
    }
}
