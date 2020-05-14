#include "searchwindow.h"
#include "ui_searchwindow.h"

#include <QScrollBar>
#include <QFontDialog>

SearchWindow::SearchWindow(QList<QString> list, QList<loadItem> LoadItems, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWindow)
{
    ui->setupUi(this);

    currentBook = NULL;
    currentList = NULL;
    currentText = NULL;

    setWindowTitle("Поисковик");

    this->list = list;
    this->LoadItems = LoadItems;

    highlighter1 = new QRegexpHighlighter(this);
    highlighter1->setDocument(ui->edtText->document());
    highlighter1->setPattern(ui->edtSearch->text());

    ui->edtText->setEnabled(false);
    ui->label->setEnabled(false);
    ui->edtSource->setEnabled(false);
    ui->lstResults->setEnabled(false);
    ui->lstText->setEnabled(false);


}

SearchWindow::~SearchWindow()
{
    delete ui;
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
    list.append(":/doc/basic.book");
    list.append(":/doc/tribal_messianship.book");
    list.append(":/doc/tfs.book");
    list.append(":/doc/tfs_quotes.book");
    list.append(":/doc/sw.book");
    list.append(":/doc/other.book");
    list.append(":/doc/tf_eng1.book");
    list.append(":/doc/tf_eng2.book");
    list.append(":/doc/bible.book");
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
    ui->edtText->append("В квадратных скобках - число, указывающее на то, сколько раз в тексте встретились слово или фраза.");

    int c = 0;
    int n = 0;

    for(int k = 0; k < list.count(); k++){

        for(int i = 0; i < LoadItems.size(); i++){
            if(list[k] == LoadItems[i].name){
                books = LoadItems[i].books;
            }
        }

        QString listItem = defineCurrentList(list[k]);

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
                        s.text1 = listItem;
                        s.text2 = currentBook->getName();
                        s.text3 = currentList->getName();
                        s.text4 = currentText->getName();
                        s.text5 = ui->edtSearch->text();
                        s.text6 = list[k];
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

    for(int i = 0; i < searchItems.size(); i++){
        n++;
        searchItems[i].n = n;
        ui->lstResults->addItem(QString::number(n) + ": " +
                                "[" + QString::number(searchItems[i].num) + "] " +
                                searchItems[i].text1 + ", " +
                                searchItems[i].text2 + ", " + searchItems[i].text3
                                + ", " + searchItems[i].text4);
        ui->lstResults->item(i)->setIcon(QIcon(":/images/search_.png"));
    }

    ui->lstResults->addItem(" ");
    ui->lstResults->addItem("Итого: " + QString::number(c) + " повторений"
                            + " в " + QString::number(n) + " текстах");
    ui->lstResults->addItem("Поиск завершен!");

    ui->edtText->append(" ");
    ui->edtText->append(" ");
    ui->edtText->append(QString::number(c) + " повторений"
                        + " в " + QString::number(n) + " текстах");
    ui->edtText->append("Поиск завершен!");
}

void SearchWindow::shutdown()
{
    close();
}

//Реализация поиска
void SearchWindow::on_btnFind_clicked()
{
   ui->lstText->clear();
   textFind();
}

void SearchWindow::on_btnFindZagolovki_clicked()
{
    emit sendPattern(ui->edtSearch->text());
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

    for(int k = 0; k < list.count(); k++){

        for(int i = 0; i < LoadItems.size(); i++){
            if(list[k] == LoadItems[i].name){
                books = LoadItems[i].books;
            }
        }

        QString listItem = defineCurrentList(list[k]);

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
                s.text1 = listItem;
                s.text2 = currentBook->getName();
                s.text3 = "";
                s.text4 = "";
                s.text5 = ui->edtSearch->text();
                s.text6 = list[k];
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
                    s.text1 = listItem;
                    s.text2 = currentBook->getName();
                    s.text3 = currentList->getName();;
                    s.text4 = "";
                    s.text5 = ui->edtSearch->text();
                    s.text6 = list[k];
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
                        s.text1 = listItem;
                        s.text2 = currentBook->getName();
                        s.text3 = currentList->getName();
                        s.text4 = currentText->getName();
                        s.text5 = ui->edtSearch->text();
                        s.text6 = list[k];
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
                            searchItems[i].text1 + ", " +
                            searchItems[i].text2 + ", " + searchItems[i].text3
                            + ", " + searchItems[i].text4);

    }

    ui->edtText->append(" ");
    ui->edtText->append(" ");
    ui->edtText->append("Итого: " + QString::number(c) + " повторений"
                        + " в " + QString::number(n) + " текстах");
    ui->edtText->append("Поиск завершен!");

    ui->edtText->append(" ");
    ui->edtText->append(" ");
    ui->edtText->append(QString::number(c) + " повторений"
                        + " в " + QString::number(n) + " текстах");
    ui->edtText->append("Поиск завершен!");
}

void SearchWindow::on_lstResults_clicked(const QModelIndex &)
{
    ui->lstText->setEnabled(true);
    ui->lstText->clear();

    textItems.clear();
    int index = ui->lstResults->currentRow();

    if(index >= searchItems.size()){
        QMessageBox box;
        box.setText("Спасибо!");
        box.exec();
        return;
    }

        for(int i = 0; i < LoadItems.size(); i++)
        {
            if(LoadItems[i].name == searchItems[index].text6)
            {
                books = LoadItems[i].books;
            }
        }

        currentBook = getItemByName(searchItems[index].text2);
        currentList = currentBook->getItemByName(searchItems[index].text3);
        currentText = currentList->getItemByName(searchItems[index].text4);

        currentTxt = currentText->getData();
        ui->edtText->setHtml(currentTxt);

        //Вводим переменную, считающую сколько раз встречается слово в строке
        int cnt = 0;
        for(int i = 0; i < ui->edtText->document()->blockCount(); i++){
            QRegExp rx(ui->edtSearch->text());
            if(!checkRegExp(rx))return;
            int pos = 0;
            while((pos = rx.indexIn(ui->edtText->document()->findBlockByLineNumber(i).text(), pos)) != -1){
                pos += rx.matchedLength();
                cnt++;

                ui->lstText->addItem(QString::number(cnt)+ " [" + QString::number(i+1) + " строка" + "] ");
                textItem t;
                t.id = i+1;
                textItems.append(t);
                }

        }

    emit sendAll(currentBook,currentList,currentText,searchItems[index].text6);

    ui->edtSource->setText(searchItems[index].text1 + ", " +
                           searchItems[index].text2 + ", " +
                           searchItems[index].text3 + ", " +
                           searchItems[index].text4);

    currentTitle = searchItems[index].text3;
}

//Реализация выбора для поисковика
void SearchWindow::on_btnChoose_clicked()
{
    list.clear();
    fillList();

    widget_findchooser = new FindChooser(list);

    connect(widget_findchooser, SIGNAL(changeList(QList<QString>)),
            this, SLOT(changeList(QList<QString>)));

    widget_findchooser->exec();
}

void SearchWindow::changeList(QList<QString> list)
{
    books.clear();
    this->list.clear();
    this->list = list;
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

    //Найденные строки помещаю в QStringlist p
    //По другому реализовать пробелы между строками не получилось
    QStringList p;
    for(int i = textItems[index.row()].id - 1; i < ui->edtText->document()->blockCount(); i++){
        p.append(ui->edtText->document()->findBlockByLineNumber(i).text());
    }

    //Очищаю, теперь можно вставлять искомую строку и все остальные
    //Добавляю строчки в темповую переменную, чтобы бегунок был вверху
    //Если сразу писать в окно, то бегунок будет внизу, что неудобно, придётся каждый раз его поднимать вручную
    QString tmp;
    ui->edtText->clear();
    for(int i = 0; i < p.count(); i++){
        if(p.at(i) != ""){
            tmp += "<span style='color:#B22222'>" +
                    QString::number(textItems[index.row()].id + i) + ". "  +
                    "</span>" + p[i];
        }
        tmp += "<br>";
        tmp += "<br>";
    }

    ui->edtText->setHtml(tmp);
    p.clear();
}

void SearchWindow::on_btnFont_clicked()
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

