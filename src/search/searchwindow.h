#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QWidget>
#include <QRegExp>
#include <QMessageBox>

#include "data/datatypes.h"
#include "helpers/qregexphighlighter.h"
#include "search/findchooser.h"

namespace Ui {
class SearchWindow;
}

//Структура, формирующаяся по результатам поиска в одном цикле
struct searchItem {
    QString booksCategory; //Категория
    QString bookName;       //Название книги
    QString bookChapter;   //Глава книги
    QString bookSection;   //Раздел в главе книги
    QString searchPhrase;   //Искомая фраза или слово
    QString booksPath;      //Путь к каталогу (категории) книг
    int num;                       //Сколько раз фраза встретилась в конкретном тексте
    int n;                           //Количество текстов, в которых встретилась фраза
};

//Структура, описывающая номер строки, в которой найдено искомое слово
struct textItem {
    int id;
};

class SearchWindow : public QWidget
{
    Q_OBJECT

    Storage *s;
    QStringList catalogsNamesList;
    QStringList pathList;

    QList<BookItem*> booksList;//для поиска по книгам или в отдельной книге

    QList<searchItem> searchItems; //Массив структур, формирующийся по результатам поиска
    QList<textItem> textItems;

    QList<BookItem*> currentBooks; //Массив книг в выбранном каталоге
    BookItem* currentBook;
    ListItem* currentChapter;
    TextItem* currentSection;
    QString currentTitle;
    QString currentTxt;

    //Переменные для подсветки
    QRegexpHighlighter *highlighter1;
    QRegexpHighlighter *highlighter2;

    FindChooser *widget_findchooser;
    bool isBook;
    Ui::SearchWindow *ui;

private:
    bool checkRegExp(QRegExp rx);

public:
    explicit SearchWindow(Storage *s, QWidget *parent = 0);
    ~SearchWindow();

    void loadFromFile(QString);
    void textFindInCatalogs();


private slots:
    void shutdown();

    void findInCatalogs();//запускает textFindInCatalogs()
    void on_edtSearch_returnPressed();
    void findInChapters();//экшен, привязанный к toolButton
    void findInBooks();//Поиск по книгам, полученным из класса FindChooser

    void on_edtSearch_textChanged(const QString &arg1);
    void on_lstResults_clicked(const QModelIndex &index);
    void chooseResource();//вызов chooseFinder
    void selectedBooks(QList<BookItem*> selectedBooks);//список книг, результат из chooseFinder
    void on_lstText_clicked(const QModelIndex &index);
    void chooseFont();

signals:
    void sendPattern(QString value);
    void sendAll(BookItem*,ListItem*,TextItem*,QString);   
};

#endif // SEARCHWINDOW_H
