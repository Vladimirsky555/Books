#ifndef NAMEENTER_H
#define NAMEENTER_H

#include <QDialog>

namespace Ui {
class NameEnter;
}

class NameEnter : public QDialog
{
    Q_OBJECT

    QString name;

public:
    explicit NameEnter(QWidget *parent = 0);
    ~NameEnter();

    QString getName();
    void setName(QString);

private slots:
    void on_btnOK_clicked();
    void on_btnCancel_clicked();

private:
    Ui::NameEnter *ui;
};

#endif // NAMEENTER_H
