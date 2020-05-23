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

    QStringList catalogsNamesList;
    QStringList pathList;
    QList<ListWidget*> widgets;

public:
    FindChooser(QStringList catalogsNameList, QStringList pathList, QWidget *parent = 0);
    virtual ~FindChooser();

    void reorderList();

signals:
    void changeList(QList<QString>*);

private slots:
    void ChooseList();

private:
    Ui::FindChooser *ui;
};

#endif // FINDCHOOSER_H
