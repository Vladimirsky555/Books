#ifndef CATALOGENTER_H
#define CATALOGENTER_H

#include <QDialog>

namespace Ui {
class CatalogEnter;
}

class CatalogEnter : public QDialog
{
    Q_OBJECT

    QString name;
    QString path;

public:
    explicit CatalogEnter(QWidget *parent = nullptr);
    ~CatalogEnter();

    QString getName();
    void setName(QString name);
    QString getPath();
    void setPath(QString path);

private slots:
    void on_btnCancel_clicked();

    void on_btnSave_clicked();

private:
    Ui::CatalogEnter *ui;
};

#endif // CATALOGENTER_H
