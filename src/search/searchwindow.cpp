#include "searchwindow.h"
#include "ui_searchwindow.h"

#include <QScrollBar>
#include <QFontDialog>
#include <QMenu>
#include <QTextStream>
#include <QFileDialog>
#include <QThread>
#include <QThreadPool>
#include <QDebug>

SearchWindow::SearchWindow(Storage *s, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    setWindowTitle("Поисковик");

    this->currentBook = NULL;
    this->currentChapter = NULL;
    this->currentSection = NULL;

    this->s = s;
    this->resource = true;
    this->catalogsList = s->Catalogs();//по умолчанию ищем по всем каталогам

    //Подсветка
    highlighter1 = new QRegexpHighlighter(this);
    highlighter1->setDocument(ui->edtText->document());
    highlighter1->setPattern(ui->edtSearch->text());

    ui->edtText->setEnabled(false);
    ui->label->setEnabled(false);
    ui->edtSource->setEnabled(false);
    ui->lstResults->setEnabled(false);
    ui->lstText->setEnabled(false);

    //Подключим ко всем toolButton экшены
    ui->btnFind->setDefaultAction(ui->actionFind);
    connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(find()));

    ui->btnFindChapters->setDefaultAction(ui->actionFindChapters);
    connect(ui->actionFindChapters, SIGNAL(triggered()), this, SLOT(findInChapters()));

    ui->btnBooksSelect->setDefaultAction(ui->action_books_select);
    connect(ui->action_books_select, SIGNAL(triggered()), this, SLOT(selectBooks()));

    ui->btnCatalogsSelect->setDefaultAction(ui->action_catalogs_select);
    connect(ui->action_catalogs_select, SIGNAL(triggered()), this, SLOT(selectCatalogs()));

    ui->btnFont->setDefaultAction(ui->actionFont);
    connect(ui->actionFont, SIGNAL(triggered()), this, SLOT(chooseFont()));

    //Для экспорта текстов с помощью событий
    ui->lstResults->setContextMenuPolicy(Qt::ActionsContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequsted(QPoint)));

    {
        QAction *A = exportTextToDisplay= new QAction(this);
        QPixmap p(":/images/export-to-display.png");
        A->setIcon(QIcon(p));
        A->setText(tr("Экспорт цитат текста на дисплей"));
        connect(A, SIGNAL(triggered()),this, SLOT(text_display_Export()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstResults->addAction(A);
        listActions << A;
    }{
        QAction *A = exportResultToDisplay= new QAction(this);
        QPixmap p(":/images/export-to-display.png");
        A->setIcon(QIcon(p));
        A->setText(tr("Экспорт всех цитат на дисплей"));
        connect(A, SIGNAL(triggered()),this, SLOT(result_display_Export()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstResults->addAction(A);
        listActions << A;
    }{
        QAction *A = exportTextToFile = new QAction(this);
        QPixmap p(":/images/export-in-file.png");
        A->setIcon(QIcon(p));
        A->setText(tr("Экспорт цитат текста в файл"));
        connect(A, SIGNAL(triggered()),this, SLOT(text_file_Export()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstResults->addAction(A);
        listActions << A;
    }{
        QAction *A = exportResultToFile = new QAction(this);
        QPixmap p(":/images/export-in-file.png");
        A->setIcon(QIcon(p));
        A->setText(tr("Экспорт всех цитат в файл"));
        connect(A, SIGNAL(triggered()),this, SLOT(result_file_Export()));
        A->setFont(QFont ("MS Shell Dlg 2", 11));
        ui->lstResults->addAction(A);
        listActions << A;
    }
}

SearchWindow::~SearchWindow()
{
    delete ui;
}

int SearchWindow::findInOneText(int *c, QString txt)
{
    int cnt = 0;
    QRegExp rx(ui->edtSearch->text());
    if(!checkRegExp(rx)) return 0;
    int pos = 0;
    while((pos = rx.indexIn(txt, pos)) != -1){
        pos += rx.matchedLength();
        *c += 1;
        cnt++;
    }

    return cnt;
}

void SearchWindow::addSearchItem(int cnt)
{
    SearchItem *si = new SearchItem();
    si->_catalog = currentCatalog;
    si->_book = currentBook;
    si->_chapter = currentChapter;
    si->_section = currentSection;
    si->textCount = cnt;
    s->addSearchItem(si);
}


void SearchWindow::findInCatalogs()
{
    emit sendPattern(ui->edtSearch->text());
    ui->edtText->setEnabled(true);
    ui->edtSource->setEnabled(true);
    ui->label->setEnabled(true);
    ui->lstResults->setEnabled(true);
    ui->lstResults->clear();
    ui->edtSource->clear();
    ui->edtText->clear();
    s->searchItemsClear();

    QThreadPool* pool = QThreadPool::globalInstance();

        for(int k = 0; k < catalogsList.count(); k++){

            currentCatalog = catalogsList[k];

            CatalogsWorker* cw = new CatalogsWorker(s, currentCatalog,
                                        ui->edtSearch->text(),
                                        nullptr);

            cw->setAutoDelete(true);
            pool->start(cw);
        }

    pool->waitForDone();
    s->sortResult();

    //Отображение списка результатов в нижнем виджете
    int n = 0;
    for(int i = 0; i < s->getSearchItemsCount(); i++,n++){
        ui->lstResults->addItem(QString::number(n+1) + ": " + //Порядковый номер
                                "[" + QString::number(s->getSearchItem(i)->textCount) + "] " + //Число совпадений в тексте
                                s->getSearchItem(i)->_catalog->getName() + ", " +
                                s->getSearchItem(i)->_book->getName() + ", " +
                                s->getSearchItem(i)->_chapter->getName() + ", " +
                                s->getSearchItem(i)->_section->getName());
        ui->lstResults->item(i)->setIcon(QIcon(":/images/search_.png"));
    }

    //Отображение информации о результатах
    QString result;
    result += "<b><span style=\"color:#800000\">";
    result += "Режим исследования текстов";
    result += "</span><br><br>";
    result += "<span style=\"color:#483D8B\">";
    result += "В квадратных скобках - число, указывающее на то, "
              "сколько раз в тексте встретились слово или фраза.";
    result += "<br>";
    result += "<br><span style=\"color:#FF0000\">";
    result += QString::number(s->getC()) + "</span> повторений фразы (слова) " + "\"" +
            ui->edtSearch->text() + "\" в <span style=\"color:#FF0000\">"  + QString::number(n) +
            "</span> текстах следующих каталогов:";
    result += "</span></b>";
    ui->edtText->append(result);
    ui->edtText->append(" ");
    result = "";

    for(int i = 0; i < catalogsList.count(); i++){
        ui->edtText->append(QString::number(i+1) + ": " + catalogsList[i]->getName());
    }

    result += "<br><br><b><span style=\"color:#483D8B\">";
    result += "Поиск завершен!";
    result += "</span></b><br>";
    ui->edtText->append(result);

    ui->lstResults->addItem(" ");
    ui->lstResults->addItem("Итого: " + QString::number(s->getC()) + " повторений фразы (слова) " + "\"" +
                            ui->edtSearch->text() + "\"" + " в " + QString::number(n) + " текстах");
    ui->lstResults->addItem("Поиск завершен!");
}

void SearchWindow::findInBooks()
{
    emit sendPattern(ui->edtSearch->text());
    ui->edtText->clear();
    ui->edtText->setEnabled(true);
    ui->edtSource->setEnabled(true);
    ui->label->setEnabled(true);
    ui->lstResults->setEnabled(true);
    ui->lstResults->clear();
    ui->edtSource->clear();
    ui->edtText->clear();
    s->searchItemsClear();

     QThreadPool* pool = QThreadPool::globalInstance();

    for(int i = 0; i < booksList.count(); i++){
        currentBook = booksList[i];
        BooksWorker* bw = new BooksWorker(s, currentBook, ui->edtSearch->text(), nullptr);
        bw->setAutoDelete(true);
        pool->start(bw);
    }

    pool->waitForDone();

    s->sortResult();

    //Вывод результатов
    int n = 0;
    for(int i = 0; i < s->getSearchItemsCount(); i++,n++){
        ui->lstResults->addItem(QString::number(n) + ": " + //Порядковый номер
                                "[" + QString::number(s->getSearchItem(i)->textCount) + "] " + //Число совпадений в тексте
                                s->getSearchItem(i)->_catalog->getName() + ", " +
                                s->getSearchItem(i)->_book->getName() + ", " +
                                s->getSearchItem(i)->_chapter->getName() + ", " +
                                s->getSearchItem(i)->_section->getName());
        ui->lstResults->item(i)->setIcon(QIcon(":/images/search_.png"));
    }

    //Отображение информации о результатах
    QString result;
    result += "<b><span style=\"color:#800000\">";
    result += "Режим исследования текстов";
    result += "</span><br><br><span style=\"color:#483D8B\">";
    result += "В квадратных скобках - число, указывающее на то, "
              "сколько раз в тексте встретились слово или фраза.";
    result += "<br><br><span style=\"color:#FF0000\">";
    result += QString::number(s->getC()) + "</span> повторений фразы (слова) " + "\"" +
            ui->edtSearch->text() + "\" в <span style=\"color:#FF0000\">"  + QString::number(n) +
            "</span> текстах следующих книг:";
    result += "</span></b>";
    ui->edtText->append(result);
    ui->edtText->append(" ");
    result = "";

    for(int i = 0; i < booksList.count(); i++){
        ui->edtText->append(QString::number(i+1) + ": " + booksList[i]->getName());
    }

    result += "<br><br><b><span style=\"color:#483D8B\">";
    result += "Поиск завершен!";
    result += "</span></b><br>";
    ui->edtText->append(result);

    ui->lstResults->addItem(" ");
    ui->lstResults->addItem("Итого: " + QString::number(s->getC()) + " повторений фразы (слова) " + "\"" +
                            ui->edtSearch->text() + "\"" + " в " + QString::number(n) + " текстах");
    ui->lstResults->addItem("Поиск завершен!");
}


void SearchWindow::contextMenuRequsted(const QPoint &p)
{
    QMenu M(this);
    M.addActions(listActions);
    M.exec(mapToGlobal(p));
}




//Функция, закрывающая окно поиска, если закрыть главное окно
void SearchWindow::shutdown()
{
    close();
}

//Реализация поиска
void SearchWindow::find()
{
    ui->lstText->clear();
    ui->btnFont->setEnabled(true);
    s->setinZeroC();

    if(resource){
        findInCatalogs();
    } else {
        findInBooks();
    }
}

void SearchWindow::findInChapters()
{
    emit sendPattern(ui->edtSearch->text());

    ui->btnFont->setEnabled(false);
    ui->edtText->setEnabled(true);
    ui->edtSource->setEnabled(true);
    ui->edtSource->setEnabled(false);
    ui->edtSource->clear();
    ui->label->setEnabled(false);
    ui->lstResults->setEnabled(false);
    ui->lstText->setEnabled(false);

    ui->lstResults->clear();
    ui->lstText->clear();
    s->searchItemsClear();
    ui->edtText->clear();

    int c = 0;
    for(int k = 0; k < catalogsList.count(); k++){

        currentCatalog = catalogsList.at(k);

        int cnt = 0;

        for(int l = 0; l < currentCatalog->getCount(); l++){

            currentBook = currentCatalog->getBookById(l);

            QRegExp rx(ui->edtSearch->text());
            if(!checkRegExp(rx))return;
            int pos = 0;
            while((pos = rx.indexIn(currentBook->getName(), pos)) != -1){
                pos += rx.matchedLength();
                c++;
                cnt++;
            }

            if(cnt != 0){
                SearchItem *si = new SearchItem();
                si->_catalog = currentCatalog;
                si->_book = currentBook;
                si->textCount = cnt;
                s->addSearchItem(si);
            }

            cnt = 0;

            for(int i = 0; i < currentBook->getCount(); i++){
                currentChapter = currentBook->getChapterById(i);

                QRegExp rx(ui->edtSearch->text());
                if(!checkRegExp(rx))return;
                int pos = 0;
                while((pos = rx.indexIn(currentChapter->getName(), pos)) != -1){
                    pos += rx.matchedLength();
                    c++;
                    cnt++;
                }

                if(cnt != 0){
                    SearchItem *si = new SearchItem();
                    si->_catalog = currentCatalog;
                    si->_book = currentBook;
                    si->_chapter = currentChapter;
                    si->textCount = cnt;
                    s->addSearchItem(si);
                }

                cnt = 0;

                for(int j = 0; j < currentChapter->getCount(); j++){
                    currentSection = currentChapter->getSectionById(j);

                    QRegExp rx(ui->edtSearch->text());
                    if(!checkRegExp(rx))return;
                    int pos = 0;
                    while((pos = rx.indexIn(currentSection->getName(), pos)) != -1){
                        pos += rx.matchedLength();
                        c++;
                        cnt++;
                    }

                    if(cnt != 0){
                        SearchItem *si = new SearchItem();
                        si->_catalog = currentCatalog;
                        si->_book = currentBook;
                        si->_chapter = currentChapter;
                        si->_section = currentSection;
                        si->textCount = cnt;
                        s->addSearchItem(si);
                    }

                    cnt = 0;
                }
            }
        }
    }

    //Отображение информации о результатах в окне поиска
    QString result;

    result += "<b><span style=\"color:#800000\">";
    result += "Режим исследования заголовков текстов";
    result += "<br>";
    result += "</span><br>";
    result += "<span style=\"color:#483D8B\">";
    result += "Итого: " + QString::number(c) + " повторений фразы (слова) " + "\"" +
            ui->edtSearch->text() + "\" в " +  "заголовках следующих каталогов:";
    result += "</span></b>";
    result += "<br>";
    ui->edtText->append(result);
    result = "";

    for(int i = 0; i < catalogsList.count(); i++){
        ui->edtText->append(QString::number(i+1) + ": " +
                            catalogsList[i]->getName());
    }

    result += "<br><b><span style=\"color:#483D8B\">";
    result += "Результат поиска:";
    result += "</span></b><br>";
    ui->edtText->append(result);
    result = "";

    int n = 1;
    for(int i = 0; i < s->getSearchItemsCount(); i++,n++){
        ui->edtText->append(QString::number(n) + ": " +
                            s->getSearchItem(i)->_catalog->getName() + ", " +
                            s->getSearchItem(i)->_book->getName() + ", " +
                            s->getSearchItem(i)->_chapter->getName() + ", " +
                            s->getSearchItem(i)->_section->getName());
    }

    result += "<br><br><b><span style=\"color:#483D8B\">";
    result += "Поиск завершен!";
    result += "</span></b><br>";
    ui->edtText->append(result);
}

void SearchWindow::on_lstResults_clicked(const QModelIndex &index)
{            
    ui->lstText->setEnabled(true);
    ui->lstText->clear();

    textItems.clear();
    int id = index.row();

    if(id >= s->getSearchItemsCount()){
        QMessageBox::information(this, "Информация!", "Спасибо!");
        return;
    }

    currentBook = s->getBookByName(s->getSearchItem(id)->_book->getName());
    currentChapter = currentBook->getChapterByName(s->getSearchItem(id)->_chapter->getName());
    currentSection = currentChapter->getSectionByName(s->getSearchItem(id)->_section->getName());
    currentText = s->getSearchItem(id)->_section->getData();
    ui->edtText->setHtml(currentText);

    //Вводим переменную, считающую сколько раз встречается слово в строке
    //Анализируем текст по клику на предмет наличия искомой фразы или слова
    int cnt = 0;
    for(int i = 0; i < ui->edtText->document()->blockCount(); i++){
        QRegExp rx(ui->edtSearch->text());
        if(!checkRegExp(rx))return;
        int pos = 0;
        QString str = ui->edtText->document()->findBlockByLineNumber(i).text();
        while((pos = rx.indexIn(str, pos)) != -1){
            pos += rx.matchedLength();
            cnt++;

            //В нижнем правом окошке выдаем информацию в каких строках
            //и сколько раз встретилось искомое слово
            ui->lstText->addItem(QString::number(cnt)+ " [" + QString::number(i+1) + " строка" + "] ");

            textItem t;
            t.id = i+1;
            t.line = str;
            textItems.append(t);
        }
    }

    //Отправляем в главное окно результаты поиска по клику для синхронизации
    emit sendAll(s->getSearchItem(id)->_book,
                 s->getSearchItem(id)->_chapter,
                 s->getSearchItem(id)->_section,
                 s->getSearchItem(id)->_catalog->getPath());

    //Выводим всю информацию об источнике искомой фразы
    ui->edtSource->setText(s->getSearchItem(id)->_book->getName() + ", " +
                           s->getSearchItem(id)->_chapter->getName() + ", " +
                           s->getSearchItem(id)->_section->getName());

    currentTitle = s->getSearchItem(id)->_chapter->getName();
}

//Реализация выбора ресурса для поиска
void SearchWindow::selectBooks()
{
    books_selector = new BooksSelector(s);

    connect(books_selector, SIGNAL(select(QList<BookItem*>)),
            this, SLOT(selectedBooks(QList<BookItem*>)));

    books_selector->exec();
}

void SearchWindow::selectedBooks(QList<BookItem *> selectedBooks)
{
    resource = false;//ищем по книгам
    this->booksList = selectedBooks;
}

void SearchWindow::selectCatalogs()
{
    catalogs_selector = new CatalogsSelector(s);

    connect(catalogs_selector, SIGNAL(select(QList<Catalog*>)),
            this, SLOT(selectedCatalogs(QList<Catalog*>)));

    catalogs_selector->exec();
}

void SearchWindow::selectedCatalogs(QList<Catalog *> catalogs)
{
    resource = true;//ищем по каталогам
    this->catalogsList = catalogs;
}


void SearchWindow::on_edtSearch_returnPressed()
{   
    ui->lstText->clear();
    ui->btnFont->setEnabled(true);
    s->setinZeroC();

    if(resource){
        findInCatalogs();
    } else {
        findInBooks();
    }
}

void SearchWindow::on_lstText_clicked(const QModelIndex &index)
{
    ui->edtText->clear();
    //Помещаю текст в окно, ведь по нему придётся искать
    ui->edtText->setHtml(currentText);
    int id = index.row();

    //Найденные строки помещаю в QStringlist p
    //По другому реализовать пробелы между строками не получилось
    QStringList p;
    for(int i = textItems[id].id - 1; i < ui->edtText->document()->blockCount(); i++){
        p.append(ui->edtText->document()->findBlockByLineNumber(i).text());
    }

    //Очищаю, теперь можно вставлять искомую строку и все остальные
    //Добавляю строчки в темповую переменную, чтобы бегунок был вверху
    //Если сразу писать в окно, то бегунок будет внизу, что неудобно, придётся каждый раз его поднимать вручную
    QString tmp;
    ui->edtText->clear();
    for(int i = 0; i < p.count(); i++){

        tmp += "<span style='color:#B22222'>" +
                QString::number(textItems[id].id + i) + ". "  +
                "</span>" + p[i];

        tmp += "<br>";
        tmp += "<br>";
    }

    ui->edtText->setHtml(tmp);
    p.clear();
}


void SearchWindow::chooseFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                &ok, QFont("MS Shell Dlg 2", 12), this);
    if (ok) {
        ui->edtText->setFont(font);

        QRegExp e("font-size");
        QString txt = currentText.replace(e," ");

        ui->edtText->clear();
        ui->edtText->setText(txt);

    } else {
        QMessageBox::information(this,"Сообщение","Шрифт не выбран");
    }
}

void SearchWindow::text_display_Export()
{    
    ui->lstText->clear();
    ui->edtText->clear();
    for(int i = 0; i < textItems.count(); i++){
        QString result;
        result += "<b><span style=\"color:#800000\">";
        result += QString::number(i+1);
        result += ": </span></b><br> \"";
        result += textItems[i].line + "\"";
        result += "<br>";
        result += "<b><span style=\"color:#483D8B\">(";
        result += currentBook->getName() + ", ";
        result +=  currentChapter->getName() + ", ";
        result += currentSection->getName();
        result +=   ")</span></b>";
        result += "<br>";
        result += "<br>";
        result += "<br>";

        ui->edtText->append(result);
    }
}

void SearchWindow::text_file_Export()
{
    //        QString filename = QFileDialog::getSaveFileName(this,"Сохранить как");
    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить как"), QString(), tr("DOC (*.doc)"));
    if(filename.isEmpty())return;

    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;

    QTextStream out(&file);

    for(int i = 0; i < textItems.count(); i++){
        out << "\"" + textItems[i].line + "\"" << "\n" <<
               "(" << currentBook->getName() << ", " <<
               currentChapter->getName() << ", " <<
               currentSection->getName() << ")" << "\n\n";
    }

    file.close();
}

void SearchWindow::result_display_Export()
{    
    ui->edtText->clear();
    ui->lstText->clear();

    int cnt = 0;
    for(int i = 0; i < s->getSearchItemsCount(); i++){
        QString result;
        currentText = s->getSearchItem(i)->_section->getData();
        QRegExp rx_("</p>");
        QStringList strList = currentText.split(rx_);

        for(int j = 0; j < strList.count(); j++){
            QRegExp rx(ui->edtSearch->text());
            if(!checkRegExp(rx))return;
            int pos = 0;
            QString str = "\"" +strList[j] + "\"";
            while((pos = rx.indexIn(str, pos)) != -1){
                pos += rx.matchedLength();
                cnt++;

                result += "<b><span style=\"color:#800000\">";
                result += QString::number(cnt);
                result += ": </span></b>";
                result += str;
                result += "<b><span style=\"color:#483D8B\"><br>(";
                result += s->getSearchItem(i)->_book->getName() + ", ";
                result +=  s->getSearchItem(i)->_chapter->getName() + ", ";
                result += s->getSearchItem(i)->_section->getName();
                result +=   ")</span></b>";
                result += "<br>";
                result += "<br>";
                result += "<br>";

                ui->edtText->append(result);
            }
        }

        strList.clear();
    }
}


void SearchWindow::result_file_Export()
{
    QStringList strList;

    //    QString filename = QFileDialog::getSaveFileName(this,"Сохранить как");
    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить как"), QString(), tr("DOC (*.doc)"));
    if(filename.isEmpty())return;

    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) return;
    QTextStream out(&file);

    int cnt = 0;
    for(int i = 0; i < s->getSearchItemsCount(); i++){

        currentText = s->getSearchItem(i)->_section->getData();
        QRegExp rx_("</p>");
        strList = currentText.split(rx_);

        for(int j = 0; j < strList.count(); j++){
            QRegExp rx(ui->edtSearch->text());
            if(!checkRegExp(rx))return;
            int pos = 0;

            QRegExp e("\<(/?[^>]+)>");
            QString str = strList[j].replace(e," ");

            while((pos = rx.indexIn(str, pos)) != -1){
                pos += rx.matchedLength();
                cnt++;

                out << QString::number(cnt) << ": " << "\"" << str<< "\"" << "\n" <<
                       "(" << currentBook->getName() << ", " <<
                       currentChapter->getName() << ", " <<
                       currentSection->getName() << ")" << "\n\n";
            }
        }

        strList.clear();
    }
    file.close();
}


//Вспомогательные функции
bool SearchWindow::checkRegExp(QRegExp rx)
{
    if(rx.isValid() && !rx.isEmpty() && !rx.exactMatch("")){
        return true;
    } else {
        QMessageBox::information(this,
                                 "Информсообщение",
                                 trUtf8("Некорректный шаблон регулярного выражения!"));
        return false;
    }
}


void SearchWindow::on_edtSearch_editingFinished()
{
    highlighter1->setPattern(ui->edtSearch->text());
    highlighter1->rehighlight();
}
