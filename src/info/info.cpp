#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
    setWindowTitle(tr("О программе"));
}

Info::~Info()
{
    delete ui;
}
