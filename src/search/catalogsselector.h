#ifndef CATALOGSSELECTOR_H
#define CATALOGSSELECTOR_H

#include <QDialog>

#include "data/storage.h"
#include "data/catalog.h"

namespace Ui {
class CatalogsSelector;
}

class CatalogsSelector : public QDialog
{
    Q_OBJECT

    Storage *s;
    Catalog *currentCatalog;

    QList<Catalog*> catalogsSource;
    QList<Catalog*> catalogsDest;//сформированный список для поиска

public:
    explicit CatalogsSelector(Storage *s, QWidget *parent = 0);
    ~CatalogsSelector();

    void refreshSource();
    void refreshDest();

signals:
    void select(QList<Catalog*>);

private slots:
    void on_lstSource_clicked(const QModelIndex &index);
    void on_lstDest_clicked(const QModelIndex &index);

    void selectCatalogs();
    void selectAll();

private:
    Ui::CatalogsSelector *ui;
};

#endif // CATALOGSSELECTOR_H
