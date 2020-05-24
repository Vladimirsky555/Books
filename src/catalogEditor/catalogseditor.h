#ifndef CATALOGSEDITOR_H
#define CATALOGSEDITOR_H

#include <QWidget>

#include "data/datatypes.h"
#include "catalogeditor.h"

namespace Ui {
class CatalogsEditor;
}

class CatalogsEditor : public QWidget
{
    Q_OBJECT

    Storage *s;
    Catalog *currentCatalog;
    catalogEditor *editor;
    static CatalogsEditor *_instance;

    explicit CatalogsEditor(Storage *s, QWidget *parent = nullptr);
    ~CatalogsEditor();

public:
     static CatalogsEditor *Current(Storage *s);

     void refresh();
     void addCatalog(QString name, QString path);

private slots:
     void on_btnAdd_clicked();
     void on_btnDel_clicked();
     void shutdown();
     void on_btnCatalog_clicked();
     void on_lstCatalogs_clicked(const QModelIndex &index);

signals:
     void shutdownEditor();

private:
    Ui::CatalogsEditor *ui;

//signals:
//    void changed();

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // CATALOGSEDITOR_H
