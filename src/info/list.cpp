#include "list.h"
#include "ui_list.h"

List::List(QString title, QList<ListItem*> items, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::List)
{
    ui->setupUi(this);

    setWindowTitle(title + " (Содержание)");

    this->items = items;

    highlighter = new QRegexpHighlighter(this);
    highlighter->setDocument(ui->lstItems->document());
    highlighter->setPattern(ui->edtPattern->text());

    for(int i = 0; i < items.count(); i++){

        current = items[i];

        if(i != 0){
        ui->lstItems->append(" ");
        }

        ui->lstItems->append(current->getName());

        for(int j = 0; j < current->getItemsCount(); j++)
        {
            if(current->getItemById(j)->getName() != "Вступление"){
                ui->lstItems->append(current->getItemById(j)->getName());
            }
        }
    }
}

List::~List()
{
    delete ui;
}

void List::on_edtPattern_textChanged(const QString &arg1)
{
    highlighter->setPattern(arg1);
    highlighter->rehighlight();
}

void List::shutdown()
{
    close();
}
