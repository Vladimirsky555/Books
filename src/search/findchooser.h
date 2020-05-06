#ifndef FINDCHOOSER_H
#define FINDCHOOSER_H

#include <QDialog>
#include <QTimer>

#include "search/listwidget.h"

namespace Ui {
class FindChooser;
}

class FindChooser : public QDialog
{
    Q_OBJECT

    QList<QString> list;
    QList<ListWidget*> lists;

public:
    FindChooser(QList<QString> list, QWidget *parent = 0);
    virtual ~FindChooser();

    QString defineCurrentList(QString listItem);
    void reorderList();

signals:
    void changeList(QList<QString>);

private slots:
    void ChooseList();

private:
    Ui::FindChooser *ui;
};

#endif // FINDCHOOSER_H
