#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QFontDialog>
#include <QDir>
#include <QFile>
#include <QRegExp>
#include <QThread>
#include <QThreadPool>
#include <QMutex>

//Конструктор-деструктор
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    s = new Storage();

    currentCatalog = NULL;
    currentBook = NULL;
    currentChapter = NULL;
    currentSection = NULL;

    this->title = " ";
    this->admin = false;

    setWindowTitle("Книги");

    ui->actionCatalogsEditor->setEnabled(false);

    loadNamePathList();

    ui->cbxCatalogs->addItems(nameList);

    if(nameList.at(0) != "Первый каталог"){//Чтобы программа не вылетала
        QMutex mutex(QMutex::Recursive);
        QThreadPool* pool = QThreadPool::globalInstance();

        //Грузим из файла названия каталогов и пути к ним
        for(int i = 0; i < pathList.size(); i++)
        {
            LoadWorker* lw = new LoadWorker(s, nameList[i], pathList[i], &mutex, nullptr);
            lw->setAutoDelete(true);
            pool->start(lw);
        }

        pool->waitForDone();

        currentCatalog = s->getCatalogById(0);
        currentBooks = currentCatalog->Books();
        refreshBooks();
    }

    highlighter = new QRegexpHighlighter(this);
    highlighter->setDocument(ui->edtText->document());
    highlighter->setPattern(ui->edtPattern->text());

    setEnabledAll();

    ui->actionNotes->setEnabled(false);
    ui->actionExport->setEnabled(false);
    ui->actionContent->setEnabled(false);
    ui->btnFont->setEnabled(false);
    ui->edtPattern->setEnabled(false);

    ui->btnRead->setDefaultAction(ui->actionRead);
    connect(ui->actionRead, SIGNAL(triggered()), this, SLOT(read_selected_book()));
}

MainWindow::~MainWindow()
{
    if(login.getIsLogined()){
        saveNamePathList();
//        saveData();
    }
    delete ui;
}

void MainWindow::saveData()
{
    for(int i = 0; i < s->getCount(); i++){

    QFile f(s->getCatalogById(i)->getPath());
    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream str(&f);

    currentBooks = s->getCatalogById(i)->Books();
    for(int i = 0; i < currentBooks.size(); i++){
        str << currentBooks[i]->saveIt();
    }

    f.close();
    }
}

void MainWindow::loadNamePathList()
{
    QFile f("data/catalogs");
    if(!f.exists()) {
        QDir dir;
        dir.mkpath("data");
        dir.mkpath("data/doc");
        nameList.append("Первый каталог");
        pathList.append("data/doc/first_catalog");//Чтобы программа не вылетала временный каталог
        return;
    }

    f.open(QFile::ReadOnly);
    QDataStream str(&f);

    int i = 0;
    while(!str.atEnd()){
        QString tmp;
        str >> tmp;
        nameList.append(tmp);

        str >> tmp;
        pathList.append(tmp);
        i++;
    }

    f.close();
}

void MainWindow::saveNamePathList()
{
    QFile f("data/catalogs");
    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream str(&f);

    for(int i = 0; i < s->getCount(); i++){
        str << s->getCatalogById(i)->getName() << s->getCatalogById(i)->getPath();
    }

    f.close();
}

void MainWindow::refreshCatalogs()
{
    ui->cbxCatalogs->clear();

    for(int i = 0; i < s->getCount(); i++){
        ui->cbxCatalogs->addItem(s->getCatalogById(i)->getName());
    }
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
    for(int i = 0; i < s->getCount(); i++){
        if(booksPath == s->getCatalogById(i)->getPath()){
            currentBooks = s->getCatalogById(i)->Books();
            ui->cbxCatalogs->setCurrentIndex(i);//Устанавливаем каталог в комбобокс, через индекс
        }
    }

    currentBook = book;
    currentChapter = bookChapter;
    currentSection = bookSection;

    refreshBooks();
    refreshChapters();
    refreshSections();

    //Определяем порядковый номер книги в массиве, чтобы выставить комбобокс
    for(int i = 0; i < currentBooks.count(); i++){
        if(currentBooks.at(i) == currentBook)
        ui->cbxBooks->setCurrentIndex(i);//Устанавливаем книгу в комбобокс через индекс
    }

   ui->edtText->setHtml(currentSection->getData());

   ui->lstChapters->setEnabled(true);
   ui->lstSections->setEnabled(true);
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

    for(int i = 0; i < currentBooks.count(); i++){
        ui->cbxBooks->addItem(currentBooks[i]->getName());
    }

    ui->lstSections->clear();
    ui->edtText->clear();
}

void MainWindow::refreshChapters(){
    ui->lstChapters->clear();

        for(int i = 0; i < currentBook->getCount(); i++)
        {
            ui->lstChapters->addItem(currentBook->getChapterById(i)->getName());
            ui->lstChapters->item(i)->setIcon(QIcon(":/images/chapter.png"));
        }

    ui->lstSections->clear();
    ui->edtText->clear();
    ui->lstSections->setEnabled(false);
    ui->edtText->setEnabled(false);
}

void MainWindow::refreshSections(){
    ui->lstSections->clear();
    for(int i = 0; i < currentChapter->getCount(); i++)
    {
        ui->lstSections->addItem(currentChapter->getSectionById(i)->getName());
         ui->lstSections->item(i)->setIcon(QIcon(":/images/section.png"));
    }

    ui->edtText->clear();
    ui->edtText->setEnabled(false);
}

void MainWindow::read_selected_book()
{
    ui->lstChapters->setEnabled(true);
    currentBook = currentBooks[ui->cbxBooks->currentIndex()];
    title = currentBook->getName();
    ui->actionContent->setEnabled(true);
    ui->lstSections->setEnabled(true);
    refreshChapters();
}

//Клики по полю
void MainWindow::on_lstChapters_clicked(const QModelIndex &index)
{
    ui->actionNotes->setEnabled(false);
    ui->btnFont->setEnabled(true);
    int id = index.row();

    if (id == -1) return;
    currentChapter = currentBook->getChapterById(id);
    ui->lstSections->setEnabled(true);
    ui->edtText->setEnabled(true);
    refreshSections();
}


void MainWindow::on_lstSections_clicked(const QModelIndex &index)
{
    ui->edtPattern->setEnabled(true);
    ui->actionExport->setEnabled(true);

    if (ui->lstSections->currentRow() == -1) return;
    currentSection = currentChapter->getSectionById(index.row());
    ui->actionNotes->setEnabled(true);
    refreshSections();

    ui->edtText->setEnabled(true);
    ui->edtText->setHtml(currentSection->getData());
}

void MainWindow::setEnabledAll()
{
    ui->edtText->setEnabled(false);
    ui->lstSections->setEnabled(false);
    ui->lstChapters->setEnabled(false);
}


void MainWindow::on_actionNotes_triggered()
{
    widget_o = new ItemInfoForm(currentBook->getName(), currentChapter->getName(), currentSection->getName());
    connect(this, SIGNAL(shutdown()),widget_o, SLOT(shutdown()));
    widget_o->show();
}

void MainWindow::on_actionContent_triggered()
{
    widget_list = new List(title, currentBook->getChapters());
    connect(this, SIGNAL(shutdown()),widget_list, SLOT(shutdown()));
    widget_list->exec();
}

void MainWindow::on_cbxCatalogs_currentIndexChanged(int index)
{
    if(s->getCount() == 0)return;

    ui->btnFont->setEnabled(false);
    ui->edtPattern->setEnabled(false);

    ui->lstChapters->setFont(QFont ("MS Shell Dlg 2", 10));
    ui->lstSections->setFont(QFont ("MS Shell Dlg 2", 10));

    ui->lstChapters->clear();
    ui->lstSections->clear();

    setEnabledAll();

    currentCatalog = s->getCatalogById(index);
    currentBooks = currentCatalog->Books();
    title = currentCatalog->getName();

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
    search_window = new SearchWindow(s);

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
        QString txt = currentSection->getData().replace(e," ");

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
    //QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить как"), QString(), tr("DOC (*.doc)"));
    if(filename.isEmpty())return;

    QFile file(filename);

    //Open the file
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;

    QTextStream out(&file);
    out << currentBook->getName() << "\n" <<
           currentChapter->getName() << "\n" <<
           currentSection->getName() << "\n\n" <<
           ui->edtText->toPlainText() << "\n";

    file.close();
}

void MainWindow::on_actionCatalogsEditor_triggered()
{
    CatalogsEditor = CatalogsEditor::Current(s);

    connect(this, SIGNAL(shutdown()),
            CatalogsEditor, SLOT(shutdown()));

    CatalogsEditor->show();
}

void MainWindow::on_actionAuthorization_triggered()
{
    if(admin){
        QMessageBox::information(this, "Сообщение!", "Вы уже авторизированы!");
        return;
    }

    login.exec();

    if(login.getIsLogined())
    {
        ui->actionCatalogsEditor->setEnabled(true);
        admin = true;

        CatalogsEditor = CatalogsEditor::Current(s);

        connect(this, SIGNAL(shutdown()),
                CatalogsEditor, SLOT(shutdown()));

        CatalogsEditor->show();
    }
}
