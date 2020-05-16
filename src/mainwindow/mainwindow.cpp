#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QFontDialog>
#include <QFile>
#include <QRegExp>
#include <QDebug>

//Конструктор-деструктор
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    currentBook = NULL;
    currentList = NULL;
    currentText = NULL;

    title = " ";

    setWindowTitle("Книги");

    ui->cbxList->addItem("Базовые книги");
    ui->cbxList->addItem("Родовое мессианство");
    ui->cbxList->addItem("Сборники речей");
    ui->cbxList->addItem("Сборники цитат");
    ui->cbxList->addItem("О духовном мире");
    ui->cbxList->addItem("Прочие...");
    ui->cbxList->addItem("True Father's Speech(1936-1986)");
    ui->cbxList->addItem("True Father's Speech(1987-2006)");
    ui->cbxList->addItem("Библия");

    //Создаем массив названий файлов для загрузки
    pathList.append(":/doc/basic.book");
    pathList.append(":/doc/tribal_messianship.book");
    pathList.append(":/doc/tfs.book");
    pathList.append(":/doc/tfs_quotes.book");
    pathList.append(":/doc/sw.book");
    pathList.append(":/doc/other.book");
    pathList.append(":/doc/tf_eng1.book");
    pathList.append(":/doc/tf_eng2.book");
    pathList.append(":/doc/bible.book");



    //Загружаем все файлы, только один раз
    for(int i = 0; i < pathList.size(); i++)
    {
        loadItem catalog;
        catalog.path = pathList[i];
        loadFromFile(pathList[i]);
        catalog.books = books;
        LoadItems.append(catalog);
    }

    books = LoadItems[0].books;
    refreshBooks();

    highlighter = new QRegexpHighlighter(this);
    highlighter->setDocument(ui->edtText->document());
    highlighter->setPattern(ui->edtPattern->text());

    setEnabledAll();

    ui->actionNotes->setEnabled(false);
    ui->actionExport->setEnabled(false);
    ui->actionContent->setEnabled(false);
    ui->btnFont->setEnabled(false);
    ui->edtPattern->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//Чтение из файла
void MainWindow::loadFromFile(QString file){
    books.clear();
    QFile f(file);
    if(!f.exists()) return;

    f.open(QFile::ReadOnly);
    QDataStream reader(&f);

    while(!reader.atEnd()){
        QByteArray arr;
        reader >> arr;

        books.append(new BookItem(arr));
    }

    f.close();
}


void MainWindow::setPattern(QString text)
{
    ui->edtPattern->setText(text);
}

void MainWindow::sendPattern(QString value)
{
    ui->edtPattern->setText(value);
}

void MainWindow::setAll(BookItem* book, ListItem* bookChapter, TextItem* bookSection, QString booksPath)
{
    for(int i = 0; i < LoadItems.size(); i++){
        if(booksPath == LoadItems[i].path){
            books = LoadItems[i].books;
            ui->cbxList->setCurrentIndex(i);//Устанавливаем каталог в комбобокс, через индекс
        }
    }

    currentBook = book;
    currentList = bookChapter;
    currentText = bookSection;

    refreshBooks();
    refreshRecords();
    refreshSub();

    //Определяем порядковый номер книги в массиве, чтобы выставить комбобокс
    for(int i = 0; i < books.count(); i++){
        if(books.at(i) == currentBook)
        ui->cbxBooks->setCurrentIndex(i);//Устанавливаем книгу в комбобокс через индекс
    }

   ui->edtText->setHtml(currentText->getData());

   ui->lstRecords->setEnabled(true);
   ui->lstSub->setEnabled(true);
   ui->edtText->setEnabled(true);
}

void MainWindow::on_edtPattern_textChanged(const QString &arg)
{
    highlighter->setPattern(arg);
    highlighter->rehighlight();
}

//Обновление списков
void MainWindow::refreshBooks()
{
    ui->cbxBooks->clear();

    for(int i = 0; i < books.count(); i++){
        ui->cbxBooks->addItem(books[i]->getName());
    }

    ui->lstSub->clear();
    ui->edtText->clear();
}

void MainWindow::refreshRecords(){
    ui->lstRecords->clear();

        for(int i = 0; i < currentBook->getItemsCount(); i++){
            ui->lstRecords->addItem(currentBook->getItemById(i)->getName());
        }

    ui->lstSub->clear();
    ui->edtText->clear();
    ui->lstSub->setEnabled(false);
    ui->edtText->setEnabled(false);
}

void MainWindow::refreshSub(){
    ui->lstSub->clear();
    for(int i = 0; i < currentList->getItemsCount(); i++){
        ui->lstSub->addItem(currentList->getItemById(i)->getName());
    }

    ui->edtText->clear();
    ui->edtText->setEnabled(false);
}

//Клики по полю
void MainWindow::on_btnR_clicked()
{
    ui->lstRecords->setEnabled(true);
    currentBook = books[ui->cbxBooks->currentIndex()];
    title = currentBook->getName();
    ui->actionContent->setEnabled(true);
    ui->lstSub->setEnabled(true);
    refreshRecords();
}

void MainWindow::on_lstRecords_clicked(const QModelIndex &index)
{
    ui->actionNotes->setEnabled(false);
    ui->btnFont->setEnabled(true);
    int id = index.row();

    if (id == -1) return;
    currentList = currentBook->getItemById(id);
    ui->lstSub->setEnabled(true);
    ui->edtText->setEnabled(true);
    refreshSub();
}

void MainWindow::on_lstSub_clicked(const QModelIndex &)
{
    ui->edtPattern->setEnabled(true);
    ui->actionExport->setEnabled(true);

    if (ui->lstSub->currentRow() == -1) return;
    currentText = currentList->getItemById(ui->lstSub->currentRow());
    ui->actionNotes->setEnabled(true);
    refreshSub();

    ui->edtText->setEnabled(true);
    ui->edtText->setHtml(currentText->getData());
}

void MainWindow::setEnabledAll()
{
    ui->edtText->setEnabled(false);
    ui->lstSub->setEnabled(false);
    ui->lstRecords->setEnabled(false);
}

void MainWindow::on_actionNotes_triggered()
{
    widget_o = new ItemInfoForm(currentBook->getName(), currentList->getName(), currentText->getName());
    connect(this, SIGNAL(shutdown()),widget_o, SLOT(shutdown()));
    widget_o->show();
}

void MainWindow::on_actionContent_triggered()
{
    widget_list = new List(title, currentBook->getAllItems());
    connect(this, SIGNAL(shutdown()),widget_list, SLOT(shutdown()));
    widget_list->exec();
}

void MainWindow::on_cbxList_currentIndexChanged(int index)
{
    if(LoadItems.count() == 0)return;

    ui->btnFont->setEnabled(false);
    ui->edtPattern->setEnabled(false);

    ui->lstRecords->setFont(QFont ("MS Shell Dlg 2", 10));
    ui->lstSub->setFont(QFont ("MS Shell Dlg 2", 10));

    ui->lstRecords->clear();
    ui->lstSub->clear();

    setEnabledAll();

    switch (index) {
    case 0:
        books = LoadItems[0].books;
        title = "Базовые книги";
        break;

    case 1:
        books = LoadItems[1].books;
        title = "Родовое мессианство";
        break;

    case 2:
        books = LoadItems[2].books;
        title = "Сборники речей";
        break;

    case 3:
        books = LoadItems[3].books;
        title = "Сборники цитат";
        break;

    case 4:
        books = LoadItems[4].books;
        title = "О духовном мире";
        break;

    case 5:
        books = LoadItems[5].books;
        title = "Прочие...)";
        break;

    case 6:
        books = LoadItems[6].books;
        title = "True Father's Speech(1936-1986)";
        break;

    case 7:
        books = LoadItems[7].books;
        title = "True Father's Speech(1986-2006)";
        break;

    case 8:
        books = LoadItems[8].books;
        title = "Библия";
        break;

    default:
        books = LoadItems[0].books;
        title = "Базовые книги";
    }

    setWindowTitle(title);
    refreshBooks();
}


void MainWindow::on_actionInfoDialog_triggered()
{
    Info i;
    i.exec();
}


void MainWindow::on_actionSearch_triggered()
{
    search_window = new SearchWindow(pathList, LoadItems);

    connect(search_window, SIGNAL(sendPattern(QString)),
            this, SLOT(setPattern(QString)));

    connect(search_window, SIGNAL(sendAll(BookItem*,ListItem*,TextItem*,QString)),
            this, SLOT(setAll(BookItem*,ListItem*,TextItem*,QString)));

    connect(this, SIGNAL(shutdown()),
            search_window, SLOT(shutdown()));

    search_window->show();
}



void MainWindow::on_btnFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                &ok, QFont("MS Shell Dlg 2", 12), this);
    if (ok) {
        ui->edtText->setFont(font);

        QRegExp e("font-size");
        QString txt = currentText->getData().replace(e," ");

        ui->edtText->clear();
        ui->edtText->setText(txt);

    } else {
        QMessageBox::information(this,"Сообщение","Шрифт не выбран!");
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    emit shutdown();
    QWidget::closeEvent(event);
}


//Экспорт названия книги, заголовка и текста в файл
void MainWindow::on_actionExport_triggered()
{
    //Save the file to disk
    QString filename = QFileDialog::getSaveFileName(this,"Сохранить как");
    if(filename.isEmpty())return;

    QFile file(filename);

    //Open the file
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;

    QTextStream out(&file);
    out << currentBook->getName() << "\n" <<
           currentList->getName() << "\n" <<
           currentText->getName() << "\n\n" <<
           ui->edtText->toPlainText() << "\n";

    file.close();
}


