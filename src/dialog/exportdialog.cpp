#include "exportdialog.h"
#include "ui_exportdialog.h"

//Экспорт книг
ExportDialog::ExportDialog(Storage *s, Catalog *catalogFrom, BookItem *book, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);

    setWindowTitle("Список каталогов");
    this->chapter = NULL;
    this->section = NULL;

    this->s = s;
    this->catalogFrom = catalogFrom;
    this->book = book;//книга для экспорта    

    for(int i = 0; i < s->getCount(); i++)
    {
        ui->lstExport->addItem(s->getNameList().at(i));
        ui->lstExport->item(i)->setIcon(QIcon(":/images/catalog.png"));
    }
}


//Экспорт глав
ExportDialog::ExportDialog(Catalog *catalog, BookItem *bookFrom, ListItem *chapter, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);

    setWindowTitle("Список книг");
    this->section = NULL;

    this->catalog = catalog;
    this->bookFrom = bookFrom;
    this->chapter = chapter;

    for(int i = 0; i < catalog->getCount(); i++)
    {
        ui->lstExport->addItem(catalog->getNameList().at(i));
        ui->lstExport->item(i)->setIcon(QIcon(":/images/book.png"));
    }
}


//Экспорт разделов
ExportDialog::ExportDialog(BookItem *book, ListItem *chapterFrom, TextItem *section, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);
    setWindowTitle("Список глав");

    this->book = book;
    this->chapterFrom = chapterFrom;
    this->section = section;

    for(int i = 0; i < book->getCount(); i++)
    {
        ui->lstExport->addItem(book->getNameList().at(i));
        ui->lstExport->item(i)->setIcon(QIcon(":/images/chapter.png"));
    }
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::on_lstExport_clicked(const QModelIndex &index)
{
    if(chapter == NULL && section == NULL){
        catalogTo = s->getCatalogById(index.row());//получаем указатель на каталог, для переноса книги
        catalogTo->addBook(book);
        catalogFrom->removeBook(book);
        close();
    } else if(section == NULL){
        bookTo = catalog->getBookById(index.row());//получаем указатель на книгу, для переноса главы
        bookTo->addChapter(chapter);
        bookFrom->removeChater(chapter);
        close();
    } else {
        chapterTo = book->getChapterById(index.row());//получаем указатель на главу, для переноса раздела
        chapterTo->addSection(section);
        chapterFrom->removeSection(section);
        close();
    }
}
