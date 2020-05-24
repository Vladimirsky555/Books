#include "findchooser.h"
#include "ui_findchooser.h"

#include <QMessageBox>
#include <QAction>
#include <QDebug>


FindChooser::FindChooser(Storage *s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindChooser)
{
    ui->setupUi(this);

    this->s = s;

    ui->btnOk->setEnabled(false);

    setWindowTitle("Выбор книг (книги) для поиска");

        ui->btnOk->setDefaultAction(ui->actionChoose);//привязали к toolbutton
          connect(ui->actionChoose, SIGNAL(triggered()),
                      this, SLOT(chooseBooks()));

    int count = 0;
    for( int i = 0; i < s->getCount(); i++){

        currentCatalog = s->catalogs[i];
        for(int j = 0; j < currentCatalog->getCount(); j++){

            currentBook = currentCatalog->getBookById(j);
            booksSource.append(currentBook);
            ui->lstSource->addItem(currentBook->getName());
            ui->lstSource->item(count)->setIcon(QIcon(":/images/book.png"));
            count++;
        }
    }
}

FindChooser::~FindChooser()
{
    delete ui;
}

void FindChooser::refreshSource()
{
    ui->lstSource->clear();
    for(int i = 0; i < booksSource.count(); i++){
        ui->lstSource->addItem(booksSource[i]->getName());
        ui->lstSource->item(i)->setIcon(QIcon(":/images/book.png"));
    }
}

void FindChooser::refreshDest()
{
    ui->lstDest->clear();
    for(int i = 0; i < booksDest.count(); i++){
        ui->lstDest->addItem(booksDest[i]->getName());
        ui->lstDest->item(i)->setIcon(QIcon(":/images/reading.png"));
    }
}


void FindChooser::chooseBooks()
{
    emit choose(booksDest);
    close();
}

void FindChooser::on_lstSource_clicked(const QModelIndex &index)
{
    ui->btnOk->setEnabled(true);

    QString name = ui->lstSource->item(index.row())->text();
    currentBook = s->getBookByName(name);
    booksDest.append(currentBook);
    booksSource.removeOne(currentBook);

    refreshDest();
    refreshSource();
}


void FindChooser::on_lstDest_clicked(const QModelIndex &index)
{
    QString name = ui->lstDest->item(index.row())->text();
    currentBook = s->getBookByName(name);
    booksDest.removeOne(currentBook);
    booksSource.append(currentBook);

    refreshSource();
    refreshDest();
}
