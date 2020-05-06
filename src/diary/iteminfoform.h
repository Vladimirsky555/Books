#ifndef ITEMINFOFORM_H
#define ITEMINFOFORM_H

#include <QWidget>
#include "data/datatypes.h"

namespace Ui {
class ItemInfoForm;
}

class ItemInfoForm : public QWidget
{
    Q_OBJECT

    QList<ItemInfo*> arr;
    ItemInfo* current;

public:
    QString t;

public:
    explicit ItemInfoForm(QString t, QWidget *parent = 0);
    ~ItemInfoForm();

    void loadFromFile();
    void saveToFile();

    void refreshList();
    void disableWidget();

private slots:
    void shutdown();
    void on_btnCancel_clicked();
    void on_btnAdd_clicked();
    void on_btnRemove_clicked();
    void on_lstUsers_clicked();
    void on_btnOk_clicked();
    void on_edtCalendar_clicked(const QDate &date);
    void on_pushButton_clicked();




private:
    Ui::ItemInfoForm *ui;
};

#endif // ITEMINFOFORM_H
