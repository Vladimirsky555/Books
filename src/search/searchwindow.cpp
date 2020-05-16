#include "searchwindow.h"
#include "ui_searchwindow.h"

#include <QScrollBar>
#include <QFontDialog>

SearchWindow::SearchWindow(QStringList pathList, QList<loadItem> LoadItems, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);
    setWindowTitle("Поисковик");

    this->currentBook = NULL;
    this->currentList = NULL;
    this->currentText = NULL;
    this->pathList = pathList;
    this->LoadItems = LoadItems;

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
    connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(FindTexts()));

    ui->btnFindChapters->setDefaultAction(ui->actionFindChapters);
    connect(ui->actionFindChapters, SIGNAL(triggered()), this, SLOT(FindChapters()));

    ui->btnChoose->setDefaultAction(ui->actionChoose);
    connect(ui->actionChoose, SIGNAL(triggered()), this, SLOT(chooseResource()));

    ui->btnFont->setDefaultAction(ui->actionFont);
    connect(ui->actionFont, SIGNAL(triggered()), this, SLOT(chooseFont()));
}

SearchWindow::~SearchWindow()
{
    delete ui;
}


void SearchWindow::textFind()
{
    emit sendPattern(ui->edtSearch->text());

    ui->edtText->setEnabled(true);
    ui->edtSource->setEnabled(true);
    ui->label->setEnabled(true);
    ui->lstResults->setEnabled(true);
    ui->lstResults->clear();
    ui->edtSource->clear();
    ui->edtText->clear();

    books.clear();
    searchItems.clear();

    ui->edtText->append("Режим исследования текстов!");
    ui->edtText->append("");
    ui->edtText->append("В квадратных скобках - число, указывающее на то, "
                        "сколько раз в тексте встретились слово или фраза.");

    int c = 0;
    int n = 0;

    for(int k = 0; k < pathList.count(); k++){

        for(int i = 0; i < LoadItems.size(); i++){
            if(pathList[k] == LoadItems[i].path){
                books = LoadItems[i].books;
            }
        }

        QString listItem = defineCurrentList(pathList[k]);

        int cnt = 0;

        for(int l = 0; l < books.count(); l++){
            currentBook = books[l];

            for(int i = 0; i < currentBook->getItemsCount(); i++){
                currentList = currentBook->getItemById(i);

                for(int j = 0; j < currentList->getItemsCount(); j++){
                    currentText = currentList->getItemById(j);

                    QRegExp rx(ui->edtSearch->text());
                    if(!checkRegExp(rx))return;
                    int pos = 0;
                    while((pos = rx.indexIn(currentText->getData(), pos)) != -1){
                        pos += rx.matchedLength();
                        c++;
                        cnt++;
                    }


                    if(cnt != 0){
                        searchItem s;
                        s.booksCategory = listItem;
                        s.bookName = currentBook->getName();
                        s.bookChapter = currentList->getName();
                        s.bookSection = currentText->getName();
                        s.searchPhrase = ui->edtSearch->text();
                        s.booksPath = pathList[k];
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
                                searchItems[i].booksCategory + ", " + searchItems[i].bookName + ", " +
                                searchItems[i].bookChapter + ", " + searchItems[i].bookSection);
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

//Функция, закрывающая окно поиска, если закрыть главное окно
void SearchWindow::shutdown()
{
    close();
}

//Реализация поиска
void SearchWindow::FindTexts()
{
    ui->lstText->clear();
    ui->btnFont->setEnabled(true);
    textFind();
}

void SearchWindow::FindChapters()
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
    books.clear();
    ui->edtText->clear();

    ui->edtText->append("Режим исследования заголовков текстов!");
    ui->edtText->append(" ");
    ui->edtText->append(" ");

    int c = 0;
    int n = 0;

    for(int k = 0; k < pathList.count(); k++){

        for(int i = 0; i < LoadItems.size(); i++){
            if(pathList[k] == LoadItems[i].path){
                books = LoadItems[i].books;
            }
        }

        QString listItem = defineCurrentList(pathList[k]);

        int cnt = 0;

        for(int l = 0; l < books.count(); l++){
            currentBook = books[l];

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
                s.booksCategory = listItem;
                s.bookName = currentBook->getName();
                s.bookChapter = "";
                s.bookSection = "";
                s.searchPhrase = ui->edtSearch->text();
                s.booksPath = pathList[k];
                s.num = cnt;
                searchItems.append(s);
            }

            cnt = 0;

            for(int i = 0; i < currentBook->getItemsCount(); i++){
                currentList = currentBook->getItemById(i);

                QRegExp rx(ui->edtSearch->text());
                if(!checkRegExp(rx))return;
                int pos = 0;
                while((pos = rx.indexIn(currentList->getName(), pos)) != -1){
                    pos += rx.matchedLength();
                    c++;
                    cnt++;
                }

                if(cnt != 0){
                    searchItem s;
                    s.booksCategory = listItem;
                    s.bookName = currentBook->getName();
                    s.bookChapter = currentList->getName();;
                    s.bookSection = "";
                    s.searchPhrase = ui->edtSearch->text();
                    s.booksPath = pathList[k];
                    s.num = cnt;
                    searchItems.append(s);
                }

                cnt = 0;

                for(int j = 0; j < currentList->getItemsCount(); j++){
                    currentText = currentList->getItemById(j);

                    QRegExp rx(ui->edtSearch->text());
                    if(!checkRegExp(rx))return;
                    int pos = 0;
                    while((pos = rx.indexIn(currentText->getName(), pos)) != -1){
                        pos += rx.matchedLength();
                        c++;
                        cnt++;
                    }

                    if(cnt != 0){
                        searchItem s;
                        s.booksCategory = listItem;
                        s.bookName = currentBook->getName();
                        s.bookChapter = currentList->getName();
                        s.bookSection = currentText->getName();
                        s.searchPhrase = ui->edtSearch->text();
                        s.booksPath = pathList[k];
                        s.num = cnt;
                        searchItems.append(s);
                    }

                    cnt = 0;
                }
            }
        }
    }

    for(int i = 0; i < searchItems.size(); i++){
        n++;
        searchItems[i].n = n;
        ui->edtText->append(QString::number(n) + ": " +
                            searchItems[i].booksCategory + ", " +
                            searchItems[i].bookName + ", " + searchItems[i].bookChapter
                            + ", " + searchItems[i].bookSection);

    }

    //Отображение информации о результатах в окне поиска
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

    if(index.row() >= searchItems.size()){
        QMessageBox::information(this, "Информация!", "Спасибо!");
        return;
    }

        for(int i = 0; i < LoadItems.size(); i++){
            if(LoadItems.at(i).path == searchItems.at(id).booksPath){
                books = LoadItems.at(i).books;
            }
        }

        currentBook = getItemByName(searchItems[id].bookName);
        currentList = currentBook->getItemByName(searchItems[id].bookChapter);
        currentText = currentList->getItemByName(searchItems[id].bookSection);

        currentTxt = currentText->getData();
        ui->edtText->setHtml(currentTxt);

        //Вводим переменную, считающую сколько раз встречается слово в строке
        //Анализируем текст по клику на предмет наличия искомой фразы или слова
        int cnt = 0;
        for(int i = 0; i < ui->edtText->document()->blockCount(); i++){
            QRegExp rx(ui->edtSearch->text());
            if(!checkRegExp(rx))return;
            int pos = 0;
            while((pos = rx.indexIn(ui->edtText->document()->findBlockByLineNumber(i).text(), pos)) != -1){
                pos += rx.matchedLength();
                cnt++;

                 //В нижнем правом окошке выдаем информацию в каких строках
                //и сколько раз встретилось искомое слово
                ui->lstText->addItem(QString::number(cnt)+ " [" + QString::number(i+1) + " строка" + "] ");
                textItem t;
                t.id = i+1;
                textItems.append(t);
                }
        }

    //Отправляем в главное окно результаты поиска по клику для синхронизации
    emit sendAll(currentBook,currentList,currentText,searchItems[id].booksPath);

    //Выводим всю информацию об источнике искомой фразы
    ui->edtSource->setText(searchItems[id].booksCategory + ", " +
                                          searchItems[id].bookName + ", " +
                                         searchItems[id].bookChapter + ", " +
                                         searchItems[id].bookSection);

    currentTitle = searchItems[id].bookChapter;
}

//Реализация выбора ресурса для поиска
void SearchWindow::chooseResource()
{
    pathList.clear();
    fillList();

    widget_findchooser = new FindChooser(pathList);

    connect(widget_findchooser, SIGNAL(changeList(QList<QString>)),
                 this, SLOT(changeList(QList<QString>)));

    widget_findchooser->exec();
}

void SearchWindow::changeList(QList<QString> list)
{
    books.clear();
    this->pathList.clear();
    this->pathList = list;
}

void SearchWindow::on_edtSearch_returnPressed()
{
    ui->lstText->clear();
    textFind();
}

void SearchWindow::on_lstText_clicked(const QModelIndex &index)
{
    ui->edtText->clear();
    //Помещаю текст в окно, ведь по нему придётся искать
    ui->edtText->setHtml(currentTxt);
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
        QString txt = currentTxt.replace(e," ");

        ui->edtText->clear();
        ui->edtText->setText(txt);

    } else {
        QMessageBox::information(this,"Сообщение","Шрифт не выбран");
    }
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

void SearchWindow::fillList()
{
    pathList.append(":/doc/basic.book");
    pathList.append(":/doc/tribal_messianship.book");
    pathList.append(":/doc/tfs.book");
    pathList.append(":/doc/tfs_quotes.book");
    pathList.append(":/doc/sw.book");
    pathList.append(":/doc/other.book");
    pathList.append(":/doc/tf_eng1.book");
    pathList.append(":/doc/tf_eng2.book");
    pathList.append(":/doc/bible.book");
}


QString SearchWindow::defineCurrentList(QString el)
{
    QString listItem;

    if(el == ":/doc/basic.book"){
        listItem = "Базовые книги";
    }else if(el == ":/doc/tribal_messianship.book"){
        listItem = "Родовое мессианство";
    }else if(el == ":/doc/tfs.book"){
        listItem = "Сборники речей";
    }else if(el == ":/doc/tfs_quotes.book"){
        listItem = "Сборники цитат";
    }else if(el == ":/doc/sw.book"){
        listItem = "О духовном мире";
    }else if(el == ":/doc/other.book"){
        listItem = "Прочие...";
    }else if(el == ":/doc/tf_eng1.book"){
        listItem = "True Father's Speech(1936-1986)";
    }else if(el == ":/doc/tf_eng2.book"){
        listItem = "True Father's Speech(1987-2006)";
    }else if(el == ":/doc/bible.book"){
        listItem = "Библия";
    }

    return listItem;
}

BookItem *SearchWindow::getItemByName(QString value)
{
    for(int i = 0; i < books.size(); i++){
        if(books[i]->getName() == value){
            return books[i];
        }
    }

    return NULL;
}
