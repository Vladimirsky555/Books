#include "booksselector.h"
#include "ui_booksselector.h"

#include <QMessageBox>
#include <QAction>

BooksSelector::BooksSelector(Storage *s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BooksSelector)
{
    ui->setupUi(this);

    this->s = s;

    ui->btnOk->setEnabled(false);

    setWindowTitle("Выбор книг для поиска");

        ui->btnOk->setDefaultAction(ui->actionBookSelect);//привязали к toolbutton
          connect(ui->actionBookSelect, SIGNAL(triggered()),
                      this, SLOT(selectBooks()));

    int count = 0;
    for( int i = 0; i < s->getCount(); i++){

        currentCatalog = s->getCatalogById(i);
        for(int j = 0; j < currentCatalog->getCount(); j++){

            currentBook = currentCatalog->getBookById(j);
            booksSource.append(currentBook);
            count++;
        }
    }

    refreshSource();
}

BooksSelector::~BooksSelector()
{
    delete ui;
}

void BooksSelector::refreshSource()
{
    ui->lstSource->clear();
    for(int i = 0; i < booksSource.count(); i++){
        ui->lstSource->addItem(booksSource[i]->getName());
        ui->lstSource->item(i)->setIcon(QIcon(":/images/book.png"));
    }
}

void BooksSelector::refreshDest()
{
    ui->lstDest->clear();
    for(int i = 0; i < booksDest.count(); i++){
        ui->lstDest->addItem(booksDest[i]->getName());
        ui->lstDest->item(i)->setIcon(QIcon(":/images/reading.png"));
    }
}

void BooksSelector::on_lstSource_clicked(const QModelIndex &index)
{
    ui->btnOk->setEnabled(true);

    QString name = ui->lstSource->item(index.row())->text();
    currentBook = s->getBookByName(name);
    booksDest.append(currentBook);
    booksSource.removeOne(currentBook);

    refreshDest();
    refreshSource();
}

void BooksSelector::on_lstDest_clicked(const QModelIndex &index)
{
    QString name = ui->lstDest->item(index.row())->text();
    currentBook = s->getBookByName(name);
    booksDest.removeOne(currentBook);
    booksSource.append(currentBook);

    refreshSource();
    refreshDest();
}

void BooksSelector::selectBooks()
{
    emit select(booksDest);
    close();
}
