#include "exportdialog.h"
#include "ui_exportdialog.h"

ExportDialog::ExportDialog(Storage *s, Catalog *catalog, BookItem *book, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog)
{
    ui->setupUi(this);

    setWindowTitle("Список каталогов");

    this->s = s;
    this->book = book;//книга для экспорта
    this->catalogFrom = catalog;

    for(int i = 0; i < s->getCount(); i++){
        ui->lstExport->addItem(s->getNameList().at(i));

//        if(s->getNameList().at(i) == "Библия")
//            ui->lstExport->item(i)->setIcon(QIcon(":/images/bible.png"));

        ui->lstExport->item(i)->setIcon(QIcon(":/images/catalog.png"));
    }
}

ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::on_lstExport_clicked(const QModelIndex &index)
{
    catalogTo = s->getCatalogById(index.row());//получаем каталог, в который перенести книгу
    catalogTo->addBook(book);
    catalogFrom->removeBook(book);
    close();
}
