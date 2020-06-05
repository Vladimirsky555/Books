#include "catalogeditor.h"
#include "ui_catalogeditor.h"

#include <QMessageBox>
#include <QFile>
#include <QAction>
#include <QMenu>
#include <QModelIndex>
#include <QIntValidator>

catalogEditor::catalogEditor(Storage *s, Catalog *catalog, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::catalogEditor)
{
    ui->setupUi(this);

    setWindowTitle(catalog->getName());

    this->s = s; //передать для виджета экпорта книг в другие каталоги
    this->catalog = catalog;

    currentBook = NULL;
    currentChapter = NULL;
    currentSection = NULL;

    ui->lstChapters->setEnabled(false);
    ui->lstSections->setEnabled(false);
    ui->edtText->setEnabled(false);

    refreshBooks();
    addActions();

    ui->lstBooks->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->lstChapters->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->lstSections->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequsted(QPoint)));
}

catalogEditor::~catalogEditor()
{
    delete ui;
}


//Одиночные клики по полю
void catalogEditor::on_lstBooks_clicked(const QModelIndex &index)
{    
    int id = index.row();
    ui->lstChapters->setEnabled(true);

    if (id == -1) return;
    currentBook = catalog->getBookById(id);
    ui->edtFirstSection->setValidator(new QIntValidator(1, currentBook->getCount()));
    ui->edtLastSection->setValidator(new QIntValidator(1, currentBook->getCount()));

    ui->lstChapters->setEnabled(true);
    ui->lstSections->setEnabled(true);
    ui->edtText->setEnabled(true);

    refreshChapters();
}


void catalogEditor::on_lstChapters_clicked(const QModelIndex &index)
{
    int id = index.row();
    if (id == -1) return;
    currentChapter = currentBook->getChapterById(id);
    ui->edtFirstSection->setValidator(new QIntValidator(1, currentChapter->getCount()));
    ui->edtLastSection->setValidator(new QIntValidator(1, currentChapter->getCount()));
    ui->lstSections->setEnabled(true);
    ui->edtText->setEnabled(true);
    refreshSections();
}


void catalogEditor::on_lstSections_clicked(const QModelIndex &index)
{
    ui->btnSaveText->setEnabled(true);
    int id = index.row();
    if (id == -1) return;
    currentSection = currentChapter->getSectionById(id);
    refreshSections();

    ui->edtText->setEnabled(true);
    ui->edtText->setHtml(currentSection->getData());
}

void catalogEditor::contextMenuRequsted(const QPoint &p)
{
    QMenu book(this);
    book.addActions(bookActions);
    book.exec(mapToGlobal(p));

    QMenu chapter(this);
    chapter.addActions(chapterActions);
    chapter.exec(mapToGlobal(p));

    QMenu section(this);
    section.addActions(sectionActions);
    section.exec(mapToGlobal(p));
}


void catalogEditor::book_Edit()
{
    NameEnter ne;
    if(currentBook != NULL){
        ne.setName(currentBook->getName());
    } else {
        QMessageBox::information(this, "Предупреждение!", "Вы не выбрали ни одной книги!");
        return;
    }
    ne.exec();

    currentBook->setName(ne.getName());
    refreshBooks();
}

void catalogEditor::book_Up()
{
    catalog->up(ui->lstBooks->currentRow());
    refreshBooks();
}

void catalogEditor::book_Down()
{
    catalog->down(ui->lstBooks->currentRow());
    refreshBooks();
}

void catalogEditor::book_Insert_First()
{
    NameEnter ne;
    ne.exec();

    if (ne.getName() == "") {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    catalog->insertDataFirst(ne.getName());
    refreshBooks();
}

void catalogEditor::book_Insert()
{
    if(currentBook == NULL) return;

    NameEnter ne;
    ne.exec();

    if (ne.getName() == "") {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    catalog->insertData(currentBook->getName(), ne.getName());
    refreshBooks();
}

void catalogEditor::book_Insert_End()
{
    NameEnter ne;
    ne.exec();

    if (ne.getName() == "") {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    catalog->insertDataAtEnd(ne.getName());
    ui->lstSections->clear();
    refreshBooks();
}

void catalogEditor::book_Delete()
{
    if(currentBook == NULL) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Вопрос!", "Вы действительно хотите удалить книгу?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        //Удаляем указатель массива books

        catalog->deleteBook(currentBook);
        refreshBooks();

        ui->lstChapters->clear();
        ui->lstSections->clear();
        ui->edtText->clear();
        ui->lstChapters->setEnabled(false);
        ui->lstSections->setEnabled(false);
        ui->edtText->setEnabled(false);

        currentBook = NULL;
    } else {
        return;
    }
}

void catalogEditor::book_Export()
{
    if(currentBook == NULL) return;
    ExportDialog ed(s, catalog, currentBook);

    ed.exec();

    refreshBooks();
    ui->lstChapters->clear();
    ui->lstChapters->setEnabled(false);
}

void catalogEditor::book_add_Number()
{
    ui->lstChapters->clear();
        for(int i = 0; i < currentBook->getCount(); i++)
        {
            ui->lstChapters->addItem("[" + QString::number(i + 1) + "] " +
                                     currentBook->getChapterById(i)->getName());
             ui->lstChapters->item(i)->setIcon(QIcon(":/images/chapter.png"));
        }

        ui->edtFirstSection->setText(QString::number(1));
        ui->edtLastSection->setText(QString::number(currentBook->getCount()));
}

void catalogEditor::book_Duplicate()
{
    if(currentBook == NULL) return;

    int first = ui->edtFirstSection->text().toInt();
    int end = ui->edtLastSection->text().toInt();

    //Проверки
    if(first <= 0) {
        QMessageBox::warning(this, "Предупреждение!", "Перед копированием пронумеруйте главы, выбрав "
                                                      "необходимые для создания новой книги. По умолчанию будет скопирована "
                                                      "вся книга!");
        return;
    }
    if(end > currentBook->getCount()) {
        QMessageBox::warning(this, "Предупреждение!", "Количество глав не совпадает с реальным. "
                                                      "Пронумеруйте главы, выбрав необходимые для создания новой книги. "
                                                      "По умолчанию будет скопирована вся книга!");
        return;
    }

    BookItem* newBook = catalog->insert_Duplicate(currentBook->getName() + "_копия");

//    for(int i = 0; i < currentBook->getCount(); i++){
     for(int i = first; i <= end; i++){

        currentChapter = currentBook->getChapterById(i - 1);
        ListItem* newChapter = newBook->insert_Duplicate(currentChapter->getName());

       for(int j = 0; j < currentChapter->getCount(); j++){

            currentSection = currentChapter->getSectionById(j);
            TextItem* newSection = newChapter->insert_Duplicate(currentSection->getName());
            newSection->setData(currentSection->getData());
        }
    }

    refreshBooks();
}

void catalogEditor::chapter_Up()
{
    currentBook->up(ui->lstChapters->currentRow());
    refreshChapters();
}

void catalogEditor::chapter_Down()
{
    currentBook->down(ui->lstChapters->currentRow());
    refreshChapters();
}

void catalogEditor::chapter_Insert_First()
{
    NameEnter ne;
    ne.exec();

    if (ne.getName() == "") {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    currentBook->insertDataFirst(ne.getName());
    refreshChapters();
}

void catalogEditor::chapter_Insert()
{
    if(currentChapter == NULL) return;

    NameEnter ne;
    ne.exec();

    if (ne.getName() == "") {
        QMessageBox::information(this, "Информация!", "Файл не может быть пустым!");
        return;
    }

    currentBook->insertData(currentChapter->getName(), ne.getName());
    refreshChapters();
}

void catalogEditor::chapter_Insert_End()
{
    NameEnter ne;
    ne.exec();

    if (ne.getName() == "") {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    currentBook->insertDataAtEnd(ne.getName());
    refreshChapters();
}

void catalogEditor::chapter_Edit()
{
    NameEnter ne;
    if(currentChapter != NULL){
    ne.setName(currentChapter->getName());
    } else {
        QMessageBox::information(this, "Предупреждение!", "Вы не выбрали ни одной книги!");
        return;
    }
    ne.exec();

    currentChapter->setName(ne.getName());
    refreshChapters();
}

void catalogEditor::chapter_Export()
{
    if(currentChapter == NULL) return;
    ExportDialog ed(catalog, currentBook, currentChapter);
    ed.exec();

    ui->lstChapters->clear();
    refreshChapters();
}


void catalogEditor::chapter_Delete()
{
    if(currentChapter == NULL) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Вопрос!", "Удаляем главу?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
       // Удаляем указатель массива items

        currentBook->deleteChapter(currentChapter);

        refreshChapters();
        ui->lstSections->clear();
        ui->edtText->clear();
        currentChapter = NULL;
    } else {
        return;
    }
}

void catalogEditor::chapter_add_Number()
{
    ui->lstSections->clear();
    for(int i = 0; i < currentChapter->getCount(); i++)
    {
        ui->lstSections->addItem("[" + QString::number(i + 1) + "] " +
                                 currentChapter->getSectionById(i)->getName());
         ui->lstSections->item(i)->setIcon(QIcon(":/images/section.png"));
    }

    ui->edtFirstSection->setText(QString::number(1));
    ui->edtLastSection->setText(QString::number(currentChapter->getCount()));
}

void catalogEditor::chapter_Duplicate()
{
    if(currentChapter == NULL) return;

    int first = ui->edtFirstSection->text().toInt();
    int end = ui->edtLastSection->text().toInt();

    //Проверки
    if(first <= 0 || end - first < 0) {
        QMessageBox::warning(this, "Предупреждение!", "Перед копированием пронумеруйте разделы, выбрав"
                                                      "необходимые для создания новой главы. "
                                                      "По умолчанию будет скопирована вся глава!");
        return;
    }
    if(end > currentChapter->getCount()) {
        QMessageBox::warning(this, "Предупреждение!", "Количество разделов не совпадает с реальным. "
                                                      "Пронумеруйте главы, выбрав необходимые для создания новой главы. "
                                                      "По умолчанию будет скопирована вся глава!");
        return;
    }

    ListItem* newChapter = currentBook->insert_Duplicate(currentChapter->getName()  + "_копия");

    for(int i = first; i <= end; i++)
    {
        currentSection = currentChapter->getSectionById(i - 1);
        TextItem* newSection = newChapter->insert_Duplicate(currentSection->getName());
        newSection->setData(currentSection->getData());
    }

    refreshChapters();
}

void catalogEditor::section_Up()
{
    currentChapter->up(currentSection);
    refreshSections();
}

void catalogEditor::section_Down()
{
    currentChapter->down(currentSection);
    refreshSections();
}

void catalogEditor::section_Insert_First()
{
    NameEnter ne;
    ne.exec();

    if (ne.getName() == "") {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    currentChapter->insertDataFirst(ne.getName());
    refreshSections();
}

void catalogEditor::section_Insert()
{
    if(currentSection == NULL) return;

    NameEnter ne;
    ne.exec();

    if (ne.getName() == "") {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    currentChapter->insertData(currentSection->getName(), ne.getName());
    refreshSections();
}

void catalogEditor::section_Insert_End()
{
    NameEnter ne;
    ne.exec();

    if (ne.getName() == "") {
        QMessageBox::information(this, "Информация", "Файл не может быть пустым!");
        return;
    }

    currentChapter->insertDataAtEnd(ne.getName());
    refreshSections();
}

void catalogEditor::section_Edit()
{
    NameEnter ne;
    ne.setName(currentSection->getName());
    ne.exec();

    currentSection->setName(ne.getName());
    refreshSections();
}

void catalogEditor::section_Export()
{
    if(currentSection == NULL) return;
    ExportDialog ed(currentBook, currentChapter, currentSection);
    ed.exec();

    ui->lstSections->clear();
    refreshSections();
}

void catalogEditor::section_Delete()
{
    if(currentSection == NULL) return;
    //Удаляем указатель массива data
    currentChapter->deleteSection(currentSection);

    refreshSections();
    ui->edtText->clear();
    ui->edtText->setEnabled(false);
    currentSection = NULL;
}


//Обновление списков
void catalogEditor::refreshBooks()
{
    ui->lstBooks->clear();

    for(int i = 0; i < catalog->getCount(); i++)
    {
        ui->lstBooks->addItem(catalog->getBookById(i)->getName());
        ui->lstBooks->item(i)->setIcon(QIcon(":/images/book.png"));
    }

    ui->lstSections->clear();
    ui->edtText->clear();
}

void catalogEditor::refreshChapters(){
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

void catalogEditor::refreshSections(){
    ui->lstSections->clear();
    for(int i = 0; i < currentChapter->getCount(); i++)
    {
        ui->lstSections->addItem(currentChapter->getSectionById(i)->getName());
         ui->lstSections->item(i)->setIcon(QIcon(":/images/section.png"));
    }

    ui->edtText->clear();
    ui->edtText->setEnabled(false);
}


void catalogEditor::shutdown()
{
    close();
}

//void catalogEditor::refreshChanged()
//{
//    refreshBooks();
//}


//Сохранение текста
void catalogEditor::on_btnSaveText_clicked()
{
    QString q = ui->edtText->toHtml();
    currentSection->setData(q);
    refreshSections();
    ui->btnSaveText->setEnabled(false);
}

void catalogEditor::on_btnSaveCatalog_clicked()
{
    QFile f(catalog->getPath());
    f.open(QFile::WriteOnly | QFile::Truncate);
    QDataStream str(&f);

    for(int i = 0; i < catalog->Books().count(); i++){
        str << catalog->Books().at(i)->saveIt();
    }

    f.close();

    QMessageBox::information(this, "Сообщение!", "Каталог сохранён!");
}


void catalogEditor::addActions()
{
    //События списка книг
    {
        QAction *A = bookInsertFirst = new QAction(this);
        QPixmap p(":/images/insert-in-the-front.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+Q"));
        A->setText(tr("Вставить в начало"));
        connect(A, SIGNAL(triggered()),this, SLOT(book_Insert_First()));
        ui->lstBooks->addAction(A);
        bookActions << A;
    }{
        QAction *A = bookInsert = new QAction(this);
        QPixmap p(":/images/insert-inside.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+W"));
        A->setText(tr("Вставить"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(book_Insert()));
        ui->lstBooks->addAction(A);
        bookActions << A;
    }{
        QAction *A = bookInsertEnd = new QAction(this);
        QPixmap p(":/images/insert-at-the-end.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+E"));
        A->setText(tr("Вставить в конец"));
        connect(A, SIGNAL(triggered()),this, SLOT(book_Insert_End()));
        ui->lstBooks->addAction(A);
        bookActions << A;
    }{
        QAction *A = bookUp = new QAction(this);
        QPixmap p(":/images/up.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+R"));
        A->setText(tr("Вверх"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(book_Up()));
        ui->lstBooks->addAction(A);
        bookActions << A;
    }{
        QAction *A = bookDown = new QAction(this);
        QPixmap p(":/images/down.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+T"));
        A->setText(tr("Вниз"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(book_Down()));
        ui->lstBooks->addAction(A);
        bookActions << A;
    }{
        QAction *A = bookEdit = new QAction(this);
        QPixmap p(":/images/edit.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+Y"));
        A->setText(tr("Переименовать"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(book_Edit()));
        ui->lstBooks->addAction(A);
        bookActions << A;
    }{
        QAction *A = bookExport = new QAction(this);
        QPixmap p(":/images/export-to-display.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+L"));
        A->setText(tr("Экспорт в другой каталог"));
        connect(A, SIGNAL(triggered()),this, SLOT(book_Export()));
        ui->lstBooks->addAction(A);
        bookActions << A;
    }{
        QAction *A = bookAddNumber = new QAction(this);
        QPixmap p(":/images/numbers.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+N"));
        A->setText(tr("Пронумеровать главы"));
        connect(A, SIGNAL(triggered()),this, SLOT(book_add_Number()));
        ui->lstBooks->addAction(A);
        bookActions << A;
    }{
        QAction *A = bookDuplicate = new QAction(this);
        QPixmap p(":/images/duplicate.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+P"));
        A->setText(tr("Дублировать"));
        connect(A, SIGNAL(triggered()),this, SLOT(book_Duplicate()));
        ui->lstBooks->addAction(A);
        bookActions << A;
    }{
        QAction *A = bookDelete = new QAction(this);
        QPixmap p(":/images/delete.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+U"));
        A->setText(tr("Удалить"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(book_Delete()));
        ui->lstBooks->addAction(A);
        bookActions << A;
    }

    //События списка с главами
    {
        QAction *A = chapterInsertFirst = new QAction(this);
        QPixmap p(":/images/insert-in-the-front.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+A"));
        A->setText(tr("Вставить в начало"));
        connect(A, SIGNAL(triggered()),this, SLOT(chapter_Insert_First()));
        ui->lstChapters->addAction(A);
        chapterActions << A;
    }{
        QAction *A = chapterInsert = new QAction(this);
        QPixmap p(":/images/insert-inside.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+S"));
        A->setText(tr("Вставить"));
        connect(A, SIGNAL(triggered()),this, SLOT(chapter_Insert()));
        ui->lstChapters->addAction(A);
        chapterActions << A;
    }  {
        QAction *A = chapterInsertEnd = new QAction(this);
        QPixmap p(":/images/insert-at-the-end.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+D"));
        A->setText(tr("Вставить в конец"));
        connect(A, SIGNAL(triggered()),this, SLOT(chapter_Insert_End()));
        ui->lstChapters->addAction(A);
        chapterActions << A;
    }{
        QAction *A = chapterUp = new QAction(this);
        QPixmap p(":/images/up.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+F"));
        A->setText(tr("Вверх"));
        connect(A, SIGNAL(triggered()),this, SLOT(chapter_Up()));
        ui->lstChapters->addAction(A);
        chapterActions << A;
    }{
        QAction *A = chapterDown = new QAction(this);
        QPixmap p(":/images/down.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+G"));
        A->setText(tr("Вниз"));
        connect(A, SIGNAL(triggered()),this, SLOT(chapter_Down()));
        ui->lstChapters->addAction(A);
        chapterActions << A;
    }{
        QAction *A = chapterEdit = new QAction(this);
        QPixmap p(":/images/edit.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+H"));
        A->setText(tr("Переименовать"));
        connect(A, SIGNAL(triggered()),this, SLOT(chapter_Edit()));
        ui->lstChapters->addAction(A);
        chapterActions << A;
    }{
        QAction *A = chapterEdit = new QAction(this);
        QPixmap p(":/images/export-to-display.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+H"));
        A->setText(tr("Экспорт в другую книгу"));
        connect(A, SIGNAL(triggered()),this, SLOT(chapter_Export()));
        ui->lstChapters->addAction(A);
        chapterActions << A;
    }{
        QAction *A = chapterAddNumber = new QAction(this);
        QPixmap p(":/images/numbers.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+N"));
        A->setText(tr("Пронумеровать разделы"));
        connect(A, SIGNAL(triggered()),this, SLOT(chapter_add_Number()));
        ui->lstChapters->addAction(A);
        chapterActions << A;
    }{
        QAction *A = chapterDuplicate = new QAction(this);
        QPixmap p(":/images/duplicate.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+P"));
        A->setText(tr("Дублировать"));
        connect(A, SIGNAL(triggered()),this, SLOT(chapter_Duplicate()));
        ui->lstChapters->addAction(A);
        chapterActions << A;
    }{
        QAction *A = chapterDelete = new QAction(this);
        QPixmap p(":/images/delete.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+D"));
        A->setText(tr("Удалить"));
        connect(A, SIGNAL(triggered()),this, SLOT(chapter_Delete()));
        ui->lstChapters->addAction(A);
        chapterActions << A;
    }

    //События списка разделов
    {
        QAction *A = sectionInsertFirst = new QAction(this);
        QPixmap p(":/images/insert-in-the-front.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+Z"));
        A->setText(tr("Вставить в начало"));
        connect(A, SIGNAL(triggered()),this, SLOT(section_Insert_First()));
        ui->lstSections->addAction(A);
        sectionActions << A;
    }{
        QAction *A = sectionInsert = new QAction(this);
        QPixmap p(":/images/insert-inside.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+X"));
        A->setText(tr("Вставить"));
        connect(A, SIGNAL(triggered()),this, SLOT(section_Insert()));
        ui->lstSections->addAction(A);
        sectionActions << A;
    }{
        QAction *A = sectionInsertEnd = new QAction(this);
        QPixmap p(":/images/insert-at-the-end.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+C"));
        A->setText(tr("Вставить в конец"));
        connect(A, SIGNAL(triggered()),this, SLOT(section_Insert_End()));
        ui->lstSections->addAction(A);
        sectionActions << A;
    }{
        QAction *A = sectionUp = new QAction(this);
        QPixmap p(":/images/up.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+V"));
        A->setText(tr("Вверх"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(section_Up()));
        ui->lstSections->addAction(A);
        sectionActions << A;
    }{
        QAction *A = sectionDown = new QAction(this);
        QPixmap p(":/images/down.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+B"));
        A->setText(tr("Вниз"));
        //Переносим добавление в модель
        connect(A, SIGNAL(triggered()),this, SLOT(section_Down()));
        ui->lstSections->addAction(A);
        sectionActions << A;
    }{
        QAction *A = sectionEdit = new QAction(this);
        QPixmap p(":/images/edit.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+N"));
        A->setText(tr("Переименовать"));
        connect(A, SIGNAL(triggered()),this, SLOT(section_Edit()));
        ui->lstSections->addAction(A);
        sectionActions << A;
    }{
        QAction *A = sectionExport = new QAction(this);
        QPixmap p(":/images/export-to-display.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+E"));
        A->setText(tr("Экспорт в другую главу"));
        connect(A, SIGNAL(triggered()),this, SLOT(section_Export()));
        ui->lstSections->addAction(A);
        sectionActions << A;
    }{
        QAction *A = sectionDelete = new QAction(this);
        QPixmap p(":/images/delete.png");
        A->setIcon(QIcon(p));
        A->setShortcut(tr("Ctrl+M"));
        A->setText(tr("Удалить"));
        connect(A, SIGNAL(triggered()),this, SLOT(section_Delete()));
        ui->lstSections->addAction(A);
        sectionActions << A;
    }
}

