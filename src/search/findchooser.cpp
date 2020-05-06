#include "findchooser.h"
#include "ui_findchooser.h"

#include <QPushButton>
#include <QMessageBox>

#include "search/listwidget.h"

FindChooser::FindChooser(QList<QString> list, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindChooser)
{
    ui->setupUi(this);
    this->list = list;
    setWindowTitle("Выбор ресурсов для поиска");
    reorderList();
}

FindChooser::~FindChooser()
{
    delete ui;
}

void FindChooser::reorderList()
{
    int maxY = ui->listWidget->height() + 10;
    lists.clear();

    for(int i = 0; i < list.size(); i++)
    {
        ListWidget *l = new ListWidget(this);

        l->setIsRight(false);
        l->setText(defineCurrentList(list[i]));

        lists.append(l);

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

    for(int i = 0; i < lists.size(); i++){
        if(lists[i]->getIsRight() == true){
            tmp.append(list[i]);
        }
    }

     emit changeList(tmp);


    for(int i = 0; i < tmp.size(); i++)
    {
        ui->listWidget->addItem(defineCurrentList(tmp[i]));
    }


     lists.clear();

     QMessageBox box;
     box.setText("Поиск будет осуществляться только в выбранных ресурсах");
     box.exec();

     close();
}


QString FindChooser::defineCurrentList(QString listItem){
    QString currentList;

    if(listItem == ":/doc/basic.book"){
        currentList = "Базовые книги";
    }else if(listItem == ":/doc/tribal_messianship.book"){
        currentList = "Родовое мессианство";
    }else if(listItem == ":/doc/tfs.book"){
        currentList = "Сборники речей";
    }else if(listItem == ":/doc/tfs_quotes.book"){
        currentList = "Сборники цитат";
    }else if(listItem == ":/doc/sw.book"){
        currentList = "О духовном мире";
    }else if(listItem == ":/doc/other.book"){
        currentList = "Прочие...";
    }else if(listItem == ":/doc/tf_eng1.book"){
        currentList = "True Father's Speech(1936-1986)";
    }else if(listItem == ":/doc/tf_eng2.book"){
        currentList = "True Father's Speech(1987-2006)";
    }else if(listItem == ":/doc/bible.book"){
        currentList = "Библия";
    }

    return currentList;
}


