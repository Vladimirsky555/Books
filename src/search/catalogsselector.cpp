#include "catalogsselector.h"
#include "ui_catalogsselector.h"

CatalogsSelector::CatalogsSelector(Storage *s, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CatalogsSelector)
{
    ui->setupUi(this);
    this->s = s;
    ui->btnOk->setEnabled(false);
    setWindowTitle(tr("Выбор каталогов для поиска"));

    ui->btnOk->setDefaultAction(ui->actionCatalogsSelect);//привязали к toolbutton
    connect(ui->actionCatalogsSelect, SIGNAL(triggered()),
            this, SLOT(selectCatalogs()));

    ui->btnAll->setDefaultAction(ui->actionAllCatalogsSelect);//привязали к toolbutton
    connect(ui->actionAllCatalogsSelect, SIGNAL(triggered()),
            this, SLOT(selectAll()));


    for( int i = 0; i < s->getCount(); i++)
    {
        catalogsSource.append(s->getCatalogById(i));
    }

    refreshSource();
}

CatalogsSelector::~CatalogsSelector()
{
    delete ui;
}

void CatalogsSelector::refreshSource()
{
    ui->lstSource->clear();
    for(int i = 0; i < catalogsSource.count(); i++){
        ui->lstSource->addItem(catalogsSource[i]->getName());
        ui->lstSource->item(i)->setIcon(QIcon(":/images/catalog.png"));
    }
}

void CatalogsSelector::refreshDest()
{
    ui->lstDest->clear();
    for(int i = 0; i < catalogsDest.count(); i++){
        ui->lstDest->addItem(catalogsDest[i]->getName());
        ui->lstDest->item(i)->setIcon(QIcon(":/images/catalog.png"));
    }
}

void CatalogsSelector::on_lstSource_clicked(const QModelIndex &index)
{
    ui->btnOk->setEnabled(true);

    QString name = ui->lstSource->item(index.row())->text();
    currentCatalog = s->getCatalogByName(name);
    catalogsDest.append(currentCatalog);
    catalogsSource.removeOne(currentCatalog);

    refreshDest();
    refreshSource();
}

void CatalogsSelector::on_lstDest_clicked(const QModelIndex &index)
{
    QString name = ui->lstDest->item(index.row())->text();
    currentCatalog = s->getCatalogByName(name);
    catalogsDest.removeOne(currentCatalog);
    catalogsSource.append(currentCatalog);

    refreshSource();
    refreshDest();
}

void CatalogsSelector::selectCatalogs()
{
    emit select(catalogsDest);
    close();
}

void CatalogsSelector::selectAll()
{
    for( int i = 0; i < s->getCount(); i++)
    {
        catalogsDest.append(s->getCatalogById(i));
    }

    emit select(catalogsDest);
    close();
}
