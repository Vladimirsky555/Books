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

    QList<ItemInfo*> noteList;
    ItemInfo* current;

public:
    QString book;
    QString title;
    QString theme;

public:
    explicit ItemInfoForm(QString book, QString title, QString theme, QWidget *parent = 0);
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
    void on_lstNotes_clicked(const QModelIndex &index);
    void on_btnSave_clicked();
    void on_edtCalendar_clicked(const QDate &date);
    void on_btnList_clicked();

private:
    Ui::ItemInfoForm *ui;
};

#endif // ITEMINFOFORM_H
