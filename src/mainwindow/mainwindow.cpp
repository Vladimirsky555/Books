#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QFontDialog>
#include <QRegExp>
#include <QDesktopWidget>

//Конструктор-деструктор
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Книги"));

    this->setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    this->size(),
                    qApp->desktop()->availableGeometry()
                    )
                );

    currentCatalog = NULL;
    currentBook = NULL;
    currentChapter = NULL;
    currentSection = NULL;
    this->title = " ";
    this->admin = false;

    s = new Storage();


    //Переключатель перевода с русского на английский
    /*
     * Поключаем к сигналу изменения пункта комбобокса лямбда-функцию, в которой будет
     * изменяться перевод приложения.
     * Поскольку QComboBox имеет перегрузку сигнатуры сигнала, то нам необходимо
     * скастовать сигнал к нужной сигнатуре. В данном случае будем использовать
     * название пункта при его изменении.
     */

    ui->cbxLan->addItems(QStringList() << "ru" << "en");

    connect(ui->cbxLan, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            [=](const QString &str)
    {
        translator.load(":lang/books_" + str + ".qm");//Загружаем перевод
        qApp->installTranslator(&translator);//Устанавливаем перевод в приложение
    });

    translator.load(":lang/books_ru.qm");//Загружаем перевод
    qApp->installTranslator(&translator);//Устанавливаем перевод в приложение


    ui->actionCatalogsEditor->setEnabled(false);

    ui->cbxCatalogs->addItems(s->nameList);

    currentCatalog = s->getCatalogById(0);
    s->setCurrentBooks(currentCatalog->Books());
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

    ui->btnRead->setDefaultAction(ui->actionRead);
    connect(ui->actionRead, SIGNAL(triggered()), this, SLOT(read_selected_book()));
}

MainWindow::~MainWindow()
{
    if(login.getIsLogined()){
        s->saveNamePathList();
//        saveData();
    }
    delete ui;
}



void MainWindow::refreshCatalogs()
{
    ui->cbxCatalogs->clear();

    for(int i = 0; i < s->getCount(); i++)
    {
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
            s->setCurrentBooks(s->getCatalogById(i)->Books());
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
    for(int i = 0; i < s->getCurrentBooks().count(); i++){
        if(s->getCurrentBooks().at(i) == currentBook)
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

    for(int i = 0; i < s->getCurrentBooks().count(); i++){
        ui->cbxBooks->addItem(s->getCurrentBooks().at(i)->getName());
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
    //currentBook = s->currentBooks[ui->cbxBooks->currentIndex()];
    currentBook = s->getCurrentBooks().at(ui->cbxBooks->currentIndex());
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
    s->setCurrentBooks(currentCatalog->Books());
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
        QMessageBox::information(this, tr("Сообщение") , tr("Шрифт не выбран!"));
    }
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    emit shutdown();
    QWidget::closeEvent(event);
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);//переведём окно заново
    }
}

//Экспорт названия книги, заголовка и текста в файл
void MainWindow::on_actionExport_triggered()
{
    //Save the file to disk
    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить как"));
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
        QMessageBox::information(this, tr("Сообщение!"), tr("Вы уже авторизированы!"));
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

void MainWindow::on_cbxLan_currentIndexChanged(int index)
{

}

