#include "nameenter.h"
#include "ui_nameenter.h"

#include <QMessageBox>

NameEnter::NameEnter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NameEnter)
{
    ui->setupUi(this);
    setWindowTitle("Добавление/Вставка/Переименование");

    setWindowFlags(Qt::Window
         | Qt::WindowMinimizeButtonHint
         | Qt::WindowMaximizeButtonHint
         | Qt::CustomizeWindowHint);


    name = "";
}

NameEnter::~NameEnter()
{
    delete ui;
}

void NameEnter::on_btnOK_clicked()
{
    name = ui->edtName->text();

    if(name == ""){
        QMessageBox mb;
        mb.setText("Пустая строка!");
        mb.exec();
        return;
    }

    close();
}

QString NameEnter::getName(){
    return name;
}

void NameEnter::setName(QString name)
{
    ui->edtName->setText(name);
}

void NameEnter::on_btnCancel_clicked()
{
    name = ui->edtName->text();
    close();
}
