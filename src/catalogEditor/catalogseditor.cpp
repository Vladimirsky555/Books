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

    setWindowTitle(tr("Редактор каталогов"));

    ui->btnCatalog->setEnabled(false);
    ui->btnDel->setEnabled(false);
    ui->btnUp->setEnabled(false);
    ui->btnDown->setEnabled(false);
    ui->btnRename->setEnabled(false);

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
        ui->lstCatalogs->item(i)->setIcon(QIcon(":/images/catalog.png"));
    }

    if(s->getCount() == 0){
        addCatalog(tr("Добавьте первый каталог, потом удалите эту строку"), "data/doc/temp.book");
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

    if (ce.getName() == "") {
        QMessageBox::information(this, tr("Информация"), tr("Файл не может быть пустым!"));
        return;
    }

    QString name = ce.getName();
    QString path = "data/doc/" + ce.getPath();

    addCatalog(name, path);
}

void CatalogsEditor::on_btnDel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Вопрос!"), tr("Вы действительно хотите удалить каталог?"),
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
    currentCatalog = s->getCatalogById(index.row());
    ui->btnCatalog->setEnabled(true);
    ui->btnDel->setEnabled(true);
    ui->btnUp->setEnabled(true);
    ui->btnDown->setEnabled(true);
    ui->btnRename->setEnabled(true);
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
    if(currentCatalog != NULL){
    ne.setName(currentCatalog->getName());
    } else {
        QMessageBox::information(this, tr("Предупреждение!"), tr("Вы не выбрали ни одного каталога!"));
        return;
    }
    ne.exec();

    currentCatalog->setName(ne.getName());
    refresh();
}

void CatalogsEditor::on_lstCatalogs_doubleClicked(const QModelIndex &)
{
    editor = new catalogEditor(s, currentCatalog);

    connect(this, SIGNAL(shutdownEditor()),
            editor, SLOT(shutdown()));

    editor->show();
}
