#include "iteminfoform.h"
#include "ui_iteminfoform.h"

#include <QWidget>
#include <QMessageBox>
#include <QFile>

#define FILENAME "notes.db"

ItemInfoForm::ItemInfoForm(QString t, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemInfoForm)
{
    ui->setupUi(this);

    setWindowTitle("Новая запись");

    this->t = t;


    loadFromFile();

    refreshList();

    current = NULL;
}

ItemInfoForm::~ItemInfoForm()
{
    delete ui;
    delete current;
}

void ItemInfoForm::loadFromFile() {
    QFile f(FILENAME);
    if (!f.exists()) return;

    f.open(QFile::ReadOnly);
    QDataStream reader(&f);

    while (!reader.atEnd()) {
        QByteArray data;
        reader >> data;

        arr.append(new ItemInfo(data));
    }

    f.close();
}

void ItemInfoForm::saveToFile() {
    QFile f(FILENAME);

    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream writer(&f);

    for (int i = 0; i < arr.size(); i++) {
        writer << arr[i]->save();
    }

    f.close();
}

void ItemInfoForm::refreshList() {
    ui->lstUsers->clear();
    for (int i = 0; i < arr.size(); i++) {
        ui->lstUsers->addItem(arr[i]->getView());
    }

    disableWidget();
}

void ItemInfoForm::disableWidget() {
    ui->edtTheme->clear();
    ui->edtCalendar->setSelectedDate(QDate::currentDate());
    ui->edtText->clear();
    ui->widget->setEnabled(false);
    ui->widget_2->setEnabled(false);

    current = NULL;
}

void ItemInfoForm::shutdown()
{
    close();
}

void ItemInfoForm::on_btnCancel_clicked()
{
    disableWidget();
}

void ItemInfoForm::on_btnAdd_clicked()
{
    disableWidget();
    ui->edtTheme->appendPlainText(this->t);
    ui->widget->setEnabled(true);
    ui->widget_2->setEnabled(true);
}

void ItemInfoForm::on_btnRemove_clicked()
{
    if (ui->lstUsers->currentRow() == -1) return;

    int index = ui->lstUsers->currentRow();
    ItemInfo *i = arr[index];

    arr.removeAt(index);
    delete i;

    disableWidget();
    saveToFile();
    refreshList();
}


void ItemInfoForm::on_lstUsers_clicked()
{
    if (ui->lstUsers->currentRow() == -1) return;

    current = arr[ui->lstUsers->currentRow()];
    ui->edtTheme->clear();
    ui->edtTheme->appendPlainText(current->getTheme());
    ui->edtCalendar->setSelectedDate(current->getDate());
    ui->edtText->setPlainText(current->getText());

    ui->widget->setEnabled(true);
    ui->widget_2->setEnabled(true);
}

void ItemInfoForm::on_btnOk_clicked()
{
    if (ui->edtTheme->toPlainText() == "" ||
            ui->edtTheme->toPlainText() == "") {
        QMessageBox box;
        box.setText("Все поля обязательны");
        box.exec();
        return;
    }

    //Создание нового пользователя
    if (current == NULL) {
        for (int i = 0; i < arr.size(); i++) {
            if (arr[i]->getTheme() == ui->edtTheme->toPlainText()) {
                QMessageBox box;
                box.setText("Поменяйте тему записи");
                box.exec();
                return;
            }
        }

        arr.append(new ItemInfo(ui->edtTheme->toPlainText(),
                                ui->edtCalendar->selectedDate(),
                                ui->edtText->toPlainText()));
        saveToFile();
        refreshList();
    } else { //Обновление пользователя
        if (current->getTheme() != ui->edtTheme->toPlainText()) {
            for (int i = 0; i < arr.size(); i++) {
                if (arr[i]->getTheme() == ui->edtTheme->toPlainText()) {
                    QMessageBox box;
                    box.setText("Поменяйте тему записи");
                    box.exec();
                    return;
                }
            }
        }

        current->setTheme(ui->edtTheme->toPlainText());
        current->setDate(ui->edtCalendar->selectedDate());
        current->setText(ui->edtText->toPlainText());


        saveToFile();
        refreshList();
    }
}



void ItemInfoForm::on_edtCalendar_clicked(const QDate &date)
{
    ui->lstUsers->clear();

    for (int i = 0; i < arr.size(); i++) {
        if(arr[i]->getDate() == date){
        ui->lstUsers->addItem(arr[i]->getView());
        }
    }
}

void ItemInfoForm::on_pushButton_clicked()
{
    refreshList();
}



