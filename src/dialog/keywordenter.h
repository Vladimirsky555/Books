#ifndef KEYWORDENTER_H
#define KEYWORDENTER_H

#include <QDialog>

namespace Ui {
class KeyWordEnter;
}

class KeyWordEnter : public QDialog
{
    Q_OBJECT

    QString key;

public:
    explicit KeyWordEnter(QWidget *parent = nullptr);
    ~KeyWordEnter();

    QString getKey();
    void setKey(QString key);

private slots:
    void on_btnCancel_clicked();
    void on_btnOK_clicked();

private:
    Ui::KeyWordEnter *ui;
};

#endif // KEYWORDENTER_H
