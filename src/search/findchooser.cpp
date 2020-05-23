#include "findchooser.h"
#include "ui_findchooser.h"

#include <QPushButton>
#include <QMessageBox>
#include <QDebug>

#include "search/listwidget.h"

FindChooser::FindChooser(QStringList catalogsNameList, QStringList pathList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindChooser)
{
    ui->setupUi(this);

    setWindowTitle("Выбор ресурсов для поиска");

    this->catalogsNamesList = catalogsNameList;
    this->pathList = pathList;

    reorderList();
}

FindChooser::~FindChooser()
{
    delete ui;
}

void FindChooser::reorderList()
{
    int maxY = ui->listWidget->height() + 10;
    widgets.clear();

    for(int i = 0; i < pathList.size(); i++)
    {
        ListWidget *l = new ListWidget(this);

        l->setIsRight(false);
        l->setText(catalogsNamesList[i]);

        widgets.append(l);

        l->setGeometry(l->x(),maxY,l->width(), l->height());
        maxY += l->height() - 15;
        l->show();
    }


    QPushButton *b = new QPushButton(this);
    b->setGeometry(b->x()+8, maxY + 14, b->width(), b->height());
    b->setText("Выбор");
    b->setFont(QFont ("MS Shell Dlg 2", 12));

    connect(b, SIGNAL(clicked(bool)),
            this, SLOT(ChooseList()));

    b->show();
}

void FindChooser::ChooseList()
{
    QList<QString> tmp;

    for(int i = 0; i < widgets.size(); i++){
        if(widgets[i]->getIsRight()){
            tmp.append(pathList[i]);
        }
    }

     emit changeList(&tmp);

    for(int i = 0; i < tmp.size(); i++)
    {
        ui->listWidget->addItem(catalogsNamesList[i]);
    }

     widgets.clear();

     QMessageBox box;
     box.setText("Поиск будет осуществляться только в выбранных ресурсах");
     box.exec();

     close();
}





