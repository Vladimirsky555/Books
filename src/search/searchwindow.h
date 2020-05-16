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

    QList<loadItem> LoadItems;//Массив каталогов книг
    QList<BookItem*> books; //Массив книг в выбранном каталоге
    QList<QString> list;
    QList<searchItem> searchItems; //Массив структур, формирующийся по результатам поиска
    QList<textItem> textItems;

    BookItem* currentBook;
    BookItem* tmp;
    ListItem* currentList;
    ListItem* tmpList;
    TextItem* currentText;
    QString currentTitle;
    QString currentTxt;

    //Переменные для подсветки
    QRegexpHighlighter *highlighter1;
    QRegexpHighlighter *highlighter2;

    FindChooser *widget_findchooser;

    Ui::SearchWindow *ui;

private:
    bool checkRegExp(QRegExp rx);

public:
    explicit SearchWindow(QList<QString> list, QList<loadItem> LoadItems, QWidget *parent = 0);
    ~SearchWindow();

    void loadFromFile(QString);
    void fillList();
    QString defineCurrentList(QString el);
    BookItem* getItemByName(QString);
    void textFind();

private slots:
    void shutdown();

    void on_btnFind_clicked();
    void on_edtSearch_returnPressed();
    void on_btnFindZagolovki_clicked();

    void on_edtSearch_textChanged(const QString &arg1);
    void on_lstResults_clicked(const QModelIndex &);
    void changeList(QList<QString>);
    void on_btnChoose_clicked();
    void on_lstText_clicked(const QModelIndex &index);
    void on_btnFont_clicked();

signals:
    void sendPattern(QString value);
    void sendAll(BookItem*,ListItem*,TextItem*,QString);   
};

#endif // SEARCHWINDOW_H
