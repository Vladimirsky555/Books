#include "keywordenter.h"
#include "ui_keywordenter.h"

#include <QMessageBox>

KeyWordEnter::KeyWordEnter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyWordEnter)
{
    ui->setupUi(this);
    setWindowTitle("Ключевое слово для раздела текста");

    ui->btnSetKey->setDefaultAction(ui->actionsetKey);
    connect(ui->actionsetKey, SIGNAL(triggered()), this, SLOT(setKey()));
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

void KeyWordEnter::setKey()
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


