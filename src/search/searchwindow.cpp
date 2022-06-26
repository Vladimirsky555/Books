#include "searchwindow.h"
#include "ui_searchwindow.h"

#include <QScrollBar>
#include <QFontDialog>
#include <QMenu>
#include <QTextStream>
#include <QFileDialog>

#include <QDebug>

SearchWindow::SearchWindow(Storage *s, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Поисковик"));

    this->currentBook = NULL;
    this->currentChapter = NULL;
    this->currentSection = NULL;
    sim = NULL;

    this->s = s;
    ss = new SearchStorage();

    this->resource = true;

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


void SearchWindow::findInCatalogs()
{
    emit sendPattern(ui->edtSearch->text());
    prepareWidgets();
    int c = sim->searchItemsMakerfromCatalogs();
    ss->sortResult();
    int n = showResult();
    report(c, n);
}

void SearchWindow::findInBooks()
{
    emit sendPattern(ui->edtSearch->text());
    prepareWidgets();
    int c = sim->searchItemsMakerfromBooks(s->booksList);
    ss->sortResult();
    int n = showResult();
    report(c, n);
}

void SearchWindow::prepareWidgets()
{
    ui->edtText->clear();
    ui->edtText->setEnabled(true);
    ui->edtSource->setEnabled(true);
    ui->label->setEnabled(true);
    ui->lstResults->setEnabled(true);
    ui->lstResults->clear();
    ui->edtSource->clear();
    ui->edtText->clear();
}

int SearchWindow::showResult()
{
    //Отображение списка результатов в нижнем виджете
    int n = 0;
    for(int i = 0; i < ss->searchItems.size(); i++,n++){
        ui->lstResults->addItem(QString::number(n+1) + ": " + //Порядковый номер
                                "[" + QString::number(ss->searchItems[i]->textCount) + "] " + //Число совпадений в тексте
                                ss->searchItems[i]->_catalog->getName() + ", " +
                                ss->searchItems[i]->_book->getName() + ", " +
                                ss->searchItems[i]->_chapter->getName() + ", " +
                                ss->searchItems[i]->_section->getName());
        ui->lstResults->item(i)->setIcon(QIcon(":/images/search_2.png"));
    }

    return n;
}



void SearchWindow::report(int c, int n)
{
    QString result;
    result += "<b><span style=\"color:#800000\">";
    result += tr("Режим исследования текстов");
    result += "</span><br><br><span style=\"color:#483D8B\">";
    result += tr("В квадратных скобках - число, указывающее на то, "
              "сколько раз в тексте встретились слово или фраза.");
    result += "<br><br><span style=\"color:#FF0000\">";
    result += QString::number(c) + tr("</span> повторений фразы (слова) ") + "\"" +
            ui->edtSearch->text() + "\" в <span style=\"color:#FF0000\">"  +
            QString::number(n) +
            tr("</span> текстах следующих книг:");
    result += "</span></b>";
    ui->edtText->append(result);
    ui->edtText->append(" ");
    result = "";

    for(int i = 0; i < s->booksList.count(); i++)
    {
        ui->edtText->append(QString::number(i+1) + ": " +
                            s->booksList[i]->getName());
    }

    result += "<br><br><b><span style=\"color:#483D8B\">";
    result += tr("Поиск завершен!");
    result += "</span></b><br>";
    ui->edtText->append(result);

    ui->lstResults->addItem(" ");
    ui->lstResults->addItem(tr("Итого: ") + QString::number(c) + tr(" повторений фразы (слова) ") + "\"" +
                            ui->edtSearch->text() + "\"" + tr(" в ") + QString::number(n) + tr(" текстах"));
    ui->lstResults->addItem(tr("Поиск завершен!"));
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
    sim = new SearchItemsMaker(s, ss, ui->edtSearch->text());

    ui->lstText->clear();
    ui->btnFont->setEnabled(true);

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
    ss->searchItems.clear();
    ui->edtText->clear();

    sim = new SearchItemsMaker(s, ss, ui->edtSearch->text());
    int c = sim->searchItemsMakerInChapters(currentChapter, currentSection);

    //Отображение информации о результатах в окне поиска
    QString result;

    result += "<b><span style=\"color:#800000\">";
    result += tr("Режим исследования заголовков текстов");
    result += "<br>";
    result += "</span><br>";
    result += "<span style=\"color:#483D8B\">";
    result += "Итого: " + QString::number(c) + tr(" повторений фразы (слова) ") + "\"" +
            ui->edtSearch->text() + tr("\" в ") +  tr("заголовках следующих каталогов:");
    result += "</span></b>";
    result += "<br>";
    ui->edtText->append(result);
    result = "";

    for(int i = 0; i < s->catalogsList.count(); i++){
        ui->edtText->append(QString::number(i+1) + ": " + s->catalogsList[i]->getName());
    }

    result += "<br><b><span style=\"color:#483D8B\">";
    result += tr("Результат поиска:");
    result += "</span></b><br>";
    ui->edtText->append(result);
    result = "";

    int n = 1;
    for(int i = 0; i < ss->searchItems.size(); i++,n++){
        ui->edtText->append(QString::number(n+1) + ": " +
                            ss->searchItems[i]->_catalog->getName() + ", " +
                            ss->searchItems[i]->_book->getName() + ", " +
                            ss->searchItems[i]->_chapter->getName() + ", " +
                            ss->searchItems[i]->_section->getName());
    }

    result += "<br><br><b><span style=\"color:#483D8B\">";
    result += tr("Поиск завершен!");
    result += "</span></b><br>";
    ui->edtText->append(result);
}

void SearchWindow::on_lstResults_clicked(const QModelIndex &index)
{            
    ui->lstText->setEnabled(true);
    ui->lstText->clear();

    textItems.clear();
    int id = index.row();

    if(id >= ss->searchItems.size()){
        QMessageBox::information(this, tr("Информация!"), tr("Спасибо!"));
        return;
    }

    currentBook = s->getBookByName(ss->searchItems[id]->_book->getName());
    currentChapter = currentBook->getChapterByName(ss->searchItems[id]->_chapter->getName());
    currentSection = currentChapter->getSectionByName(ss->searchItems[id]->_section->getName());
    currentText = ss->searchItems[id]->_section->getData();
    ui->edtText->setHtml(currentText);

    //Вводим переменную, считающую сколько раз встречается слово в строке
    //Анализируем текст по клику на предмет наличия искомой фразы или слова
    int cnt = 0;
    for(int i = 0; i < ui->edtText->document()->blockCount(); i++){
        QRegExp rx(ui->edtSearch->text());
        if(!sim->checkRegExp(rx))return;
        int pos = 0;
        QString str = ui->edtText->document()->findBlockByLineNumber(i).text();

        int check = 0;
        while((pos = rx.indexIn(str, pos)) != -1){
            pos += rx.matchedLength();
            if(check > 1)continue;
            cnt++;

            //В нижнем правом окошке выдаем информацию в каких строках
            //и сколько раз встретилось искомое слово
            ui->lstText->addItem(QString::number(cnt)+ " "
                    "[" + QString::number(i+1) + tr(" строка") + "] ");

            textItem t;
            t.id = i+1;
            t.line = str;
            textItems.append(t);
        }
    }

    //Отправляем в главное окно результаты поиска по клику для синхронизации
    emit sendAll(ss->searchItems[id]->_book,
                 ss->searchItems[id]->_chapter,
                 ss->searchItems[id]->_section,
                 ss->searchItems[id]->_catalog->getPath());

    //Выводим всю информацию об источнике искомой фразы
    ui->edtSource->setText(ss->searchItems[id]->_book->getName() + ", " +
                           ss->searchItems[id]->_chapter->getName() + ", " +
                           ss->searchItems[id]->_section->getName());

    currentTitle = ss->searchItems[id]->_chapter->getName();
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
    s->booksList = selectedBooks;
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
    s->catalogsList = catalogs;
}


void SearchWindow::on_edtSearch_returnPressed()
{   
    sim = new SearchItemsMaker(s, ss, ui->edtSearch->text());

    ui->lstText->clear();
    ui->btnFont->setEnabled(true);

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
        QMessageBox::information(this, tr("Сообщение"), tr("Шрифт не выбран"));
    }
}

void SearchWindow::text_display_Export()
{
    QString result;
    ui->lstText->clear();
    ui->edtText->clear();
    for(int i = 0; i < textItems.count(); i++){

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
    }

    ui->edtText->setHtml(result);
}

void SearchWindow::text_file_Export()
{
    //        QString filename = QFileDialog::getSaveFileName(this,"Сохранить как");
    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить как"), QString(), tr("DOC (*.doc)"));
    if(filename.isEmpty())return;

    QFile file(filename);

    //Open the file
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
    ui->edtText->setHtml(sim->quoteMaker());
}


void SearchWindow::result_file_Export()
{
    QStringList strList;

    //    QString filename = QFileDialog::getSaveFileName(this,"Сохранить как");
    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить как"), QString(), "DOC (*.doc)");
    if(filename.isEmpty())return;

    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) return;
    QTextStream out(&file);

    int cnt = 0;
    for(int i = 0; i < ss->searchItems.count(); i++){

        currentText = ss->searchItems[i]->_section->getData();
        QRegExp rx_("</p>");
        strList = currentText.split(rx_);

        for(int j = 0; j < strList.count(); j++){
            QRegExp rx(ui->edtSearch->text());
            if(!sim->checkRegExp(rx))return;
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



void SearchWindow::on_edtSearch_editingFinished()
{
    highlighter1->setPattern(ui->edtSearch->text());
    highlighter1->rehighlight();
}
