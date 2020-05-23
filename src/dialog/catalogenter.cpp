#include "catalogenter.h"
#include "ui_catalogenter.h"

#include <QMessageBox>

CatalogEnter::CatalogEnter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CatalogEnter)
{
    ui->setupUi(this);
     setWindowTitle("Добавить каталог");

    setWindowFlags(Qt::Window
         | Qt::WindowMinimizeButtonHint
         | Qt::WindowMaximizeButtonHint
         | Qt::CustomizeWindowHint);


    name = "";
    path = "";
}

CatalogEnter::~CatalogEnter()
{
    delete ui;
}

QString CatalogEnter::getName()
{
    return this->name;
}

void CatalogEnter::setName(QString name)
{
    this->name = name;
}

QString CatalogEnter::getPath()
{
    return this->path;
}

void CatalogEnter::setPath(QString path)
{
    this->path = path;
}

void CatalogEnter::on_btnCancel_clicked()
{
    close();
}

void CatalogEnter::on_btnSave_clicked()
{
    name = ui->edtName->text();
    path = ui->edtPath->text();

    if(name == "" || path == ""){
        QMessageBox::information(this, "Сообщение!", "Пустая строка!");
        return;
    }

    close();
}
