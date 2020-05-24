#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

    QString login;
    QString password;
    bool isLogined;

public:
    LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    void autification();
    bool getIsLogined();

private slots:
    void on_btnEnter_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
