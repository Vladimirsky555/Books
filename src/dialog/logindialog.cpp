#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    setWindowTitle("Авторизация");

    this->login = "";
    this->password = "";
    this->isLogined = false;

    ui->edtPassword->setEchoMode(QLineEdit::Password);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

bool LoginDialog::getIsLogined()
{
     return this->isLogined;
}

void LoginDialog::autification()
{
    if(this->login == "" || this->password == ""){
        QMessageBox::information(this, "Информация!", "Введите логин и пароль!");
        return;
    }

    if(this->login == "user" && this->password == "1234")
    {
        this->isLogined = true;
    }
}

void LoginDialog::on_btnEnter_clicked()
{
    this->login = ui->edtLogin->text();
    this->password = ui->edtPassword->text();

    autification();

    close();
}
