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
    for(int i = 0; i < s->catalogs.count(); i++){
        ui->lstCatalogs->addItem(s->catalogs[i]->getName());
    }

    //При отсутствии элементов программа не запустится
    if(s->catalogs.count() == 0){
        addCatalog("Добавьте первый каталог, потом удалите эту строку", "temp.book");
    }
}

void CatalogsEditor::addCatalog(QString name, QString path)
{
    Catalog *catalog = new Catalog(name, path);
    s->catalogs.push_back(catalog);

    QFile f("doc/" + path);
    f.open(QFile::WriteOnly | QFile::Truncate);
    f.close();

    refresh();
}


void CatalogsEditor::on_btnAdd_clicked()
{
    CatalogEnter ce;
    ce.exec();

    QString name = ce.getName();
    QString path = ce.getPath();

    addCatalog(name, path);
}

void CatalogsEditor::on_btnDel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Вопрос!", "Вы действительно хотите удалить каталог?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QFile(currentCatalog->getPath()).remove();//Удаляем файл
        s->catalogs.removeOne(currentCatalog);//Удаляем элемент списка
        delete currentCatalog;//Удаляем указатель
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
    int id = index.row();
    currentCatalog = s->catalogs.at(id);
}


void CatalogsEditor::on_btnCatalog_clicked()
{
    editor = new catalogEditor(currentCatalog);

    connect(this, SIGNAL(shutdown()),
            editor, SLOT(shutdown()));

    editor->show();
}
