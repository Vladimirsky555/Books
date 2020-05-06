#include "listwidget.h"
#include "ui_listwidget.h"

ListWidget::ListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ListWidget)
{
    ui->setupUi(this);

    this->text = " ";
    this->isChecked = false;
}

ListWidget::~ListWidget()
{
    delete ui;
}


bool ListWidget::getIsRight()
{
    return ui->checkBox->isChecked();
}

void ListWidget::setIsRight(bool isChecked)
{
    ui->checkBox->setChecked(isChecked);
}

QString ListWidget::getText()
{
    return ui->edtText->text();
}


void ListWidget::setText(QString text)
{
    ui->edtText->setText(text);
}



