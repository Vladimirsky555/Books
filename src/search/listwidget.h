#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QWidget>

namespace Ui {
class ListWidget;
}

class ListWidget : public QWidget
{
    Q_OBJECT

    QString text;
    bool isChecked;

public:
    explicit ListWidget(QWidget *parent = 0);
    ~ListWidget();

    bool getIsRight();
    QString getText();

    void setIsRight(bool isChecked);
    void setText(QString text);

private slots:


private:
    Ui::ListWidget *ui;
};

#endif // LISTWIDGET_H
