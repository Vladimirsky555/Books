#include "iteminfoform.h"
#include "ui_iteminfoform.h"

#include <QWidget>
#include <QMessageBox>
#include <QFile>

#define FILENAME "notes.db"

ItemInfoForm::ItemInfoForm(QString book, QString title, QString theme, QWidget *parent) :
    QWidget(parent), ui(new Ui::ItemInfoForm)
{
    ui->setupUi(this);
    setWindowTitle(tr("Новая запись"));

    current = NULL;
    this->book = book;
    this->title = title;
    this->theme = theme;

    loadFromFile();
    refreshList();    
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
    QDataStream str(&f);

    while (!str.atEnd()) {
        QByteArray data;
        str >> data;
        noteList.append(new ItemInfo(data));
    }

    f.close();
}

void ItemInfoForm::saveToFile() {
    QFile file(FILENAME);

    file.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream str(&file);

    for (int i = 0; i < noteList.size(); i++) {
        str << noteList[i]->save();
    }

    file.close();
}

void ItemInfoForm::refreshList() {
    ui->lstNotes->clear();
    for (int i = 0; i < noteList.size(); i++) {
        ui->lstNotes->addItem(noteList.at(i)->getView());
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
    ui->edtTheme->appendPlainText(book + ",");
    ui->edtTheme->appendPlainText(title + ",");
    ui->edtTheme->appendPlainText(theme + ".");
    ui->widget->setEnabled(true);
    ui->widget_2->setEnabled(true);
}

void ItemInfoForm::on_btnRemove_clicked()
{
    if (ui->lstNotes->currentRow() == -1) return;

    int index = ui->lstNotes->currentRow();
    ItemInfo *i = noteList[index];

    noteList.removeAt(index);
    delete i;

    disableWidget();
    saveToFile();
    refreshList();
}

void ItemInfoForm::on_lstNotes_clicked(const QModelIndex &index)
{
    ui->edtTheme->clear();
    if (index.row() == -1) return;
    current = noteList[index.row()];

    ui->edtTheme->appendPlainText(current->getView_without_date());
    ui->edtCalendar->setSelectedDate(current->getDate());
    ui->edtText->setPlainText(current->getText());

    ui->widget->setEnabled(true);
    ui->widget_2->setEnabled(true);
}

void ItemInfoForm::on_btnSave_clicked()
{
    if (theme == "") {
        QMessageBox::information(this, tr("Информация"), tr("Все поля обязательны"));
        return;
    }

    //Создание новой записи
    if (current == NULL) {
        for (int i = 0; i < noteList.size(); i++) {
            if (noteList.at(i)->getTheme() == theme) {
                QMessageBox::information(this, tr("Информация"), tr("Это ещё одна запись в этом разделе!"));
            }
        }

        noteList.append(new ItemInfo(book,title, theme,ui->edtCalendar->selectedDate(),ui->edtText->toPlainText()));
        saveToFile();
        refreshList();
    } else { //Обновление темы
        if (current->getTheme() != theme) {
            for (int i = 0; i < noteList.size(); i++) {
                if (noteList.at(i)->getTheme() == theme) {
                    QMessageBox::information(this, tr("Информация"), tr("Это ещё одна запись в этом разделе!"));
                }
            }
        }

        current->setTitle(title);
        current->setTheme(theme);
        current->setDate(ui->edtCalendar->selectedDate());
        current->setText(ui->edtText->toPlainText());

        saveToFile();
        refreshList();
    }
}

void ItemInfoForm::on_edtCalendar_clicked(const QDate &date)
{
    ui->lstNotes->clear();

    for (int i = 0; i < noteList.size(); i++) {
        if(noteList[i]->getDate() == date){
        ui->lstNotes->addItem(noteList[i]->getView());
        }
    }
}

void ItemInfoForm::on_btnList_clicked()
{
    refreshList();
}
