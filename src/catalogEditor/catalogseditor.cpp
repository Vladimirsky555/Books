#include "catalogseditor.h"
#include "ui_catalogseditor.h"

#include <QFile>
#include <QMessageBox>

#include "dialog/catalogenter.h"

CatalogsEditor* CatalogsEditor::_instance = NULL;

CatalogsEditor *CatalogsEditor::Current(Storage *s)
{
    if (_instance == NULL)
        _instance = new CatalogsEditor(s);

    return _instance;
}



CatalogsEditor::CatalogsEditor(Storage *s, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CatalogsEditor)
{
    ui->setupUi(this);

    setWindowTitle("Редактор каталогов");

    ui->btnCatalog->setEnabled(false);
    ui->btnDel->setEnabled(false);
<<<<<<< HEAD
    ui->btnUp->setEnabled(false);
    ui->btnDown->setEnabled(false);
    ui->btnRename->setEnabled(false);
=======
>>>>>>> refs/remotes/origin/master

    currentCatalog = NULL;
    this->s = s;

    refresh();
}

CatalogsEditor::~CatalogsEditor()
{
    delete ui;
}

void CatalogsEditor::refresh()
{
    ui->lstCatalogs->clear();
    for(int i = 0; i < s->getCount(); i++)
    {
        ui->lstCatalogs->addItem(s->getCatalogById(i)->getName());

//        if(s->getCatalogById(i)->getPath() == "doc/bible.book")
//            ui->lstCatalogs->item(i)->setIcon(QIcon(":/images/bible.png"));

            ui->lstCatalogs->item(i)->setIcon(QIcon(":/images/catalog.png"));
}

    //При отсутствии элементов программа не запустится
<<<<<<< HEAD
    if(s->getCount() == 0){
=======
    if(s->catalogs.count() == 0){
>>>>>>> refs/remotes/origin/master
        addCatalog("Добавьте первый каталог, потом удалите эту строку", "doc/temp.book");
    }
}

void CatalogsEditor::addCatalog(QString name, QString path)
{
    Catalog *catalog = new Catalog(name, path);
    s->addAtTheEndOfCatalog(catalog);

    QFile f(path);
    f.open(QFile::WriteOnly | QFile::Truncate);
    f.close();

    refresh();
}


void CatalogsEditor::on_btnAdd_clicked()
{
    CatalogEnter ce;
    ce.exec();

    QString name = ce.getName();
    QString path = "doc/" + ce.getPath();

    addCatalog(name, path);
}

void CatalogsEditor::on_btnDel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Вопрос!", "Вы действительно хотите удалить каталог?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QFile(currentCatalog->getPath()).remove();//Удаляем файл
        s->deleteCatalog(currentCatalog);//Удаляем элемент списка и удаляем указатель
        refresh();
    } else {
        return;
    }
}

void CatalogsEditor::shutdown()
{
    close();
}

void CatalogsEditor::on_lstCatalogs_clicked(const QModelIndex &index)
{
<<<<<<< HEAD
    currentCatalog = s->getCatalogById(index.row());
    ui->btnCatalog->setEnabled(true);
    ui->btnDel->setEnabled(true);
    ui->btnUp->setEnabled(true);
    ui->btnDown->setEnabled(true);
    ui->btnRename->setEnabled(true);
=======
    int id = index.row();
    currentCatalog = s->catalogs.at(id);
    ui->btnCatalog->setEnabled(true);
    ui->btnDel->setEnabled(true);
>>>>>>> refs/remotes/origin/master
}

void CatalogsEditor::closeEvent(QCloseEvent *event)
{
    emit shutdownEditor();
    QWidget::closeEvent(event);
}


void CatalogsEditor::on_btnCatalog_clicked()
{
    editor = new catalogEditor(s, currentCatalog);

    connect(this, SIGNAL(shutdownEditor()),
            editor, SLOT(shutdown()));

    editor->show();
}


void CatalogsEditor::on_btnUp_clicked()
{
    s->up(ui->lstCatalogs->currentRow());
    refresh();
}

void CatalogsEditor::on_btnDown_clicked()
{
    s->down(ui->lstCatalogs->currentRow());
    refresh();
}

void CatalogsEditor::on_btnRename_clicked()
{
    NameEnter ne;
    ne.exec();

    currentCatalog->setName(ne.getName());
    refresh();
}
