#include "searchwindow.h"
#include "ui_searchwindow.h"

#include <QScrollBar>
#include <QFontDialog>
#include <QMenu>
#include <QTextStream>
#include <QFileDialog>

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

    //Подсветка
    highlighter1 = new QRegexpHighlighter(this);
    highlighter1->setDocument(ui->edtText->document());
    highlighter1->setPattern(ui->edtSearch->text());

    ui->edtText->setEnabled(false);
    ui->label->setEnabled(false);
    ui->edtSource->setEnabled(false);
    ui->lstResults->setEnabled(false);
    ui->lstText->setEnabled(false);
    ui->actionFindInSelectedBooks->setEnabled(false);

    //Подключим ко всем toolButton экшены
    ui->btnFind->setDefaultAction(ui->actionFind);
    connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(findInCatalogs()));

    ui->btnFindInSelectedBooks->setDefaultAction(ui->actionFindInSelectedBooks);
    connect(ui->actionFindInSelectedBooks, SIGNAL(triggered()), this, SLOT(findInBooks()));

    ui->btnFindChapters->setDefaultAction(ui->actionFindChapters);
    connect(ui->actionFindChapters, SIGNAL(triggered()), this, SLOT(findInChapters()));

    ui->btnChoose->setDefaultAction(ui->actionChoose);
    connect(ui->actionChoose, SIGNAL(triggered()), this, SLOT(chooseResource()));

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
        ui->lstResults->addAction(A);
        listActions << A;
    }{
        QAction *A = exportTextToFile = new QAction(this);
        QPixmap p(":/images/export-in-file.png");
        A->setIcon(QIcon(p));
        A->setText(tr("Экспорт цитат текста в файл"));
        connect(A, SIGNAL(triggered()),this, SLOT(text_file_Export()));
        ui->lstResults->addAction(A);
        listActions << A;
    }{
        QAction *A = exportResultToDisplay= new QAction(this);
        QPixmap p(":/images/export-to-display.png");
        A->setIcon(QIcon(p));
        A->setText(tr("Экспорт цитат текстов на дисплей"));
        connect(A, SIGNAL(triggered()),this, SLOT(result_display_Export()));
        ui->lstResults->addAction(A);
        listActions << A;
    }{
        QAction *A = exportResultToFile = new QAction(this);
        QPixmap p(":/images/export-in-file.png");
        A->setIcon(QIcon(p));
        A->setText(tr("Экспорт цитат текстов в файл"));
        connect(A, SIGNAL(triggered()),this, SLOT(result_file_Export()));
        ui->lstResults->addAction(A);
        listActions << A;
    }
}

SearchWindow::~SearchWindow()
{
    delete ui;
}


void SearchWindow::textFindInCatalogs()
{
    emit sendPattern(ui->edtSearch->text());

    ui->edtText->setEnabled(true);
    ui->edtSource->setEnabled(true);
    ui->label->setEnabled(true);
    ui->lstResults->setEnabled(true);
    ui->lstResults->clear();
    ui->edtSource->clear();
    ui->edtText->clear();

    searchItems.clear();

    ui->edtText->append("Режим исследования текстов!");
    ui->edtText->append("");
    ui->edtText->append("В квадратных скобках - число, указывающее на то, "
                        "сколько раз в тексте встретились слово или фраза.");

    int c = 0;
    int n = 0;

    for(int k = 0; k < s->getCount(); k++){

        currentCatalog = s->getCatalogById(k);
        int cnt = 0;

        for(int l = 0; l < currentCatalog->getCount(); l++){
            currentBook = currentCatalog->getBookById(l);

            for(int i = 0; i < currentBook->getCount(); i++){
                currentChapter = currentBook->getChapterById(i);

                for(int j = 0; j < currentChapter->getCount(); j++){
                    currentSection = currentChapter->getSectionById(j);

                    QRegExp rx(ui->edtSearch->text());
                    if(!checkRegExp(rx))return;
                    int pos = 0;
                    while((pos = rx.indexIn(currentSection->getData(), pos)) != -1){
                        pos += rx.matchedLength();
                        c++;
                        cnt++;
                    }

                    if(cnt != 0){
                        searchItem s;
                        s._catalog = currentCatalog;
                        s._book = currentBook;
                        s._chapter = currentChapter;
                        s._section = currentSection;
                        s.searchPhrase = ui->edtSearch->text();
                        s.num = cnt;
                        searchItems.append(s);
                    }
                    cnt = 0;
                }
            }
        }
    }

    //Cортировка
    for(int i = 0; i < searchItems.count(); i++)
    {
        for(int j = 0; j < searchItems.count() - 1; j++)
        {
            if(searchItems[j].num < searchItems[j + 1].num)
            {
                searchItem tmp = searchItems[j];
                searchItems[j] = searchItems[j + 1];
                searchItems[j + 1] = tmp;
            }
        }
    }

    //Отображение списка результатов в нижнем виджете
    for(int i = 0; i < searchItems.size(); i++){
        n++;
        searchItems[i].n = n;
        ui->lstResults->addItem(QString::number(n) + ": " + //Порядковый номер
                                "[" + QString::number(searchItems[i].num) + "] " + //Число совпадений в тексте
                                searchItems[i]._catalog->getName() + ", " + searchItems[i]._book->getName() + ", " +
                                searchItems[i]._chapter->getName() + ", " + searchItems[i]._section->getName());
        ui->lstResults->item(i)->setIcon(QIcon(":/images/search_.png"));
    }

    //Отображение информации о результатах в конце списка нижнего виджета
    ui->lstResults->addItem(" ");
    ui->lstResults->addItem("Итого: " + QString::number(c) + " повторений фразы (слова) " + "\"" +
                            ui->edtSearch->text() + "\"" + " в " + QString::number(n) + " текстах");
    ui->lstResults->addItem("Поиск завершен!");

    //Отображение информации о результатах в окне поиска
    ui->edtText->append(" ");
    ui->edtText->append(" ");
    ui->edtText->append(QString::number(c) + " повторений фразы (слова) " + "\"" +
                                        ui->edtSearch->text() + "\" в "  + QString::number(n) + " текстах");
    ui->edtText->append("Поиск завершен!");
}

void SearchWindow::contextMenuRequsted(const QPoint &p)
{
    QMenu M(this);
    M.addActions(listActions);
    M.exec(mapToGlobal(p));
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

    searchItems.clear();

    ui->edtText->append("Режим исследования текстов!");
    ui->edtText->append("");
    ui->edtText->append("В квадратных скобках - число, указывающее на то, "
                        "сколько раз в тексте встретились слово или фраза.");

    int c = 0;
    int n = 0;
    int cnt = 0;

    for(int l = 0; l < booksList.count(); l++){
        currentBook = booksList[l];
        currentCatalog = s->getCatalogByBook(currentBook);

        for(int i = 0; i < currentBook->getCount(); i++){
            currentChapter = currentBook->getChapterById(i);

            for(int j = 0; j < currentChapter->getCount(); j++){
                currentSection = currentChapter->getSectionById(j);

                QRegExp rx(ui->edtSearch->text());
                if(!checkRegExp(rx))return;
                int pos = 0;
                while((pos = rx.indexIn(currentSection->getData(), pos)) != -1){
                    pos += rx.matchedLength();
                    c++;
                    cnt++;
                }

                    if(cnt != 0){
                        searchItem s;
                        s._catalog = currentCatalog;
                        s._book = currentBook;
                        s._chapter = currentChapter;
                        s._section = currentSection;
                        s.searchPhrase = ui->edtSearch->text();
                        s.num = cnt;
                        searchItems.append(s);
                    }
                    cnt = 0;
                }
            }
        }

    //Cортировка
    for(int i = 0; i < searchItems.count(); i++)
    {
        for(int j = 0; j < searchItems.count() - 1; j++)
        {
            if(searchItems[j].num < searchItems[j + 1].num)
            {
                searchItem tmp = searchItems[j];
                searchItems[j] = searchItems[j + 1];
                searchItems[j + 1] = tmp;
            }
        }
    }

    //Отображение списка результатов в нижнем виджете
    for(int i = 0; i < searchItems.size(); i++){
        n++;
        searchItems[i].n = n;
        ui->lstResults->addItem(QString::number(n) + ": " + //Порядковый номер
                                "[" + QString::number(searchItems[i].num) + "] " + //Число совпадений в тексте
                                    searchItems[i]._catalog->getName() + ", " +
                                    searchItems[i]._book->getName() + ", " +
                                searchItems[i]._chapter->getName() + ", " + searchItems[i]._section->getName());
        ui->lstResults->item(i)->setIcon(QIcon(":/images/search_.png"));
    }

    //Отображение информации о результатах в конце списка нижнего виджета
    ui->lstResults->addItem(" ");
    ui->lstResults->addItem("Итого: " + QString::number(c) + " повторений фразы (слова) " + "\"" +
                            ui->edtSearch->text() + "\"" + " в " + QString::number(n) + " текстах");
    ui->lstResults->addItem("Поиск завершен!");

    //Отображение информации о результатах в окне поиска
    ui->edtText->append(" ");
    ui->edtText->append(" ");
    ui->edtText->append(QString::number(c) + " повторений фразы (слова) " + "\"" +
                                        ui->edtSearch->text() + "\" в "  + QString::number(n) + " текстах списка книг:");
    ui->edtText->append(" ");

    for(int i = 0; i < booksList.count(); i++){
        ui->edtText->append(QString::number(i+1) + ": " + booksList[i]->getName());
    }

    ui->edtText->append(" ");
    ui->edtText->append("Поиск завершен!");
}

//Функция, закрывающая окно поиска, если закрыть главное окно
void SearchWindow::shutdown()
{
    close();
}

//Реализация поиска
void SearchWindow::findInCatalogs()
{
    ui->lstText->clear();
    ui->btnFont->setEnabled(true);
    textFindInCatalogs();
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
    searchItems.clear();
//    currentBooks.clear();
    ui->edtText->clear();

    ui->edtText->append("Режим исследования заголовков текстов!");
    ui->edtText->append(" ");
    ui->edtText->append(" ");

    int c = 0;
    int n = 0;

    for(int k = 0; k < s->getCount(); k++){

        currentCatalog = s->getCatalogById(k);
//        currentBooks = s->getCatalogById(k)->Books();
//        QString listItem = catalogsNamesList[k];
        int cnt = 0;

        for(int l = 0; l < currentCatalog->getCount(); l++){
//            currentBook = currentBooks[l];
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
                searchItem s;
                s._catalog = currentCatalog;
                s._book = currentBook;
//                s.booksCatalog = listItem;
                s.searchPhrase = ui->edtSearch->text();
//                s.booksPath = pathList[k];
                s.num = cnt;
                searchItems.append(s);
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
                    searchItem s;
                    s._catalog = currentCatalog;
                    s._book = currentBook;
                    s._chapter = currentChapter;
//                    s.booksCatalog = listItem;
                    s.searchPhrase = ui->edtSearch->text();
//                    s.booksPath = pathList[k];
                    s.num = cnt;
                    searchItems.append(s);
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
                        searchItem s;
                        s._catalog = currentCatalog;
                        s._book = currentBook;
                        s._chapter = currentChapter;
                        s._section = currentSection;
//                        s.booksCatalog = listItem;
                        s.searchPhrase = ui->edtSearch->text();
//                        s.booksPath = pathList[k];
                        s.num = cnt;
                        searchItems.append(s);
                    }

                    cnt = 0;
                }
            }
        }
    }


    //Отображение информации о результатах в окне поиска
    for(int i = 0; i < searchItems.size(); i++){
        n++;
        searchItems[i].n = n;
        ui->edtText->append(QString::number(n) + ": " +
                            searchItems[i]._catalog->getName() + ", " +
                            searchItems[i]._book->getName() + ", " +
                            searchItems[i]._chapter->getName() + ", " +
                            searchItems[i]._section->getName());

    }

    ui->edtText->append(" ");
    ui->edtText->append(" ");
    ui->edtText->append("Итого: " + QString::number(c) + " повторений фразы (слова) " + "\"" +
                                        ui->edtSearch->text() + "\" в " +  "заголовках всех текстов!");
     ui->edtText->append(" ");
     ui->edtText->append("Поиск завершен!");
}

void SearchWindow::on_lstResults_clicked(const QModelIndex &index)
{
    ui->lstText->setEnabled(true);
    ui->lstText->clear();

    textItems.clear();
    int id = index.row();

    if(id >= searchItems.size()){
        QMessageBox::information(this, "Информация!", "Спасибо!");
        return;
    }

        currentText = searchItems[id]._section->getData();
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
    emit sendAll(searchItems[id]._book, searchItems[id]._chapter, searchItems[id]._section, searchItems[id]._catalog->getPath());

    //Выводим всю информацию об источнике искомой фразы
    ui->edtSource->setText(searchItems[id]._book->getName() + ", " +
                                         searchItems[id]._chapter->getName() + ", " +
                                         searchItems[id]._section->getName());

    currentTitle = searchItems[id]._chapter->getName();
}

//Реализация выбора ресурса для поиска
void SearchWindow::chooseResource()
{
    widget_findchooser = new FindChooser(s);

    connect(widget_findchooser, SIGNAL(choose(QList<BookItem*>)),
            this, SLOT(selectedBooks(QList<BookItem*>)));

    widget_findchooser->exec();
}

void SearchWindow::selectedBooks(QList<BookItem *> selectedBooks)
{
    ui->actionFindInSelectedBooks->setEnabled(true);
    this->booksList = selectedBooks;
}

void SearchWindow::on_edtSearch_returnPressed()
{
    ui->lstText->clear();
    textFindInCatalogs();
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
    QString tmp;
    ui->edtText->clear();
    for(int i = 0; i < textItems.count(); i++){
    tmp += "\"" + textItems[i].line + "\"";
    tmp += "<br>";
    tmp += "(" + currentBook->getName() + ", " +
           currentChapter->getName() + ", " +
           currentSection->getName() + ")";
    tmp += "<br>";
    tmp += "<br>";
    }

    ui->edtText->setHtml(tmp);
}

void SearchWindow::text_file_Export()
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
    QString result;
    QStringList strList;

    int cnt = 0;
    for(int i = 0; i < searchItems.count(); i++){

        currentText = searchItems[i]._section->getData();
        QRegExp rx_("</p>");
        strList = currentText.split(rx_);

        for(int j = 0; j < strList.count(); j++){
            QRegExp rx(ui->edtSearch->text());
            if(!checkRegExp(rx))return;
            int pos = 0;
            QString str = strList[j];
            while((pos = rx.indexIn(str, pos)) != -1){
                pos += rx.matchedLength();
                cnt++;

                result += QString::number(cnt) + ": " + "\"" + str + "\"";
                result += "<br>";
                result += "(" + searchItems[i]._book->getName() + ", " +
                                      searchItems[i]._chapter->getName() + ", " +
                                      searchItems[i]._section->getName() + ")";
                result += "<br>";
                result += "<br>";
            }
        }

        strList.clear();
    }

    ui->edtText->setHtml(result);
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
    for(int i = 0; i < searchItems.count(); i++){

        currentText = searchItems[i]._section->getData();
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

void SearchWindow::on_edtSearch_textChanged(const QString &arg1)
{
    highlighter1->setPattern(arg1);
    highlighter1->rehighlight();
}

