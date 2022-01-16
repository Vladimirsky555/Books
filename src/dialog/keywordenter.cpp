#include "keywordenter.h"
#include "ui_keywordenter.h"

#include <QMessageBox>

KeyWordEnter::KeyWordEnter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyWordEnter)
{
    ui->setupUi(this);
    setWindowTitle("Ключевое слово для раздела текста");
}

KeyWordEnter::~KeyWordEnter()
{
    delete ui;
}

QString KeyWordEnter::getKey()
{
    return this->key;
}

void KeyWordEnter::setKey(QString key)
{
    ui->edtKey->setText(key);
}

void KeyWordEnter::on_btnCancel_clicked()
{
    key = ui->edtKey->text();
    close();
}


void KeyWordEnter::on_btnOK_clicked()
{
    key = ui->edtKey->text();

    if(key == ""){
        QMessageBox mb;
        mb.setText("Пустая строка!");
        mb.exec();
        return;
    }

    close();
}

