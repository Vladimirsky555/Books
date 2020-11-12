#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QWidget>
#include <QRegExp>
#include <QMessageBox>

#include "search/catalogsselector.h"
#include "search/booksselector.h"
#include "search/catalogsworker.h"
#include "search/booksworker.h"
#include "helpers/qregexphighlighter.h"

namespace Ui {
class SearchWindow;
}

class QAction;

//Структура, описывающая номер строки, в которой найдено искомое слово
struct textItem
{
    int id;
    QString line;
};

class SearchWindow : public QWidget
{
    Q_OBJECT

    Storage *s;
    Catalog *currentCatalog;
    BookItem* currentBook;
    ListItem* currentChapter;
    TextItem* currentSection;
    QString currentTitle;
    QString currentText;

    bool resource;
    int allCount;

    QList<Catalog*> catalogsList;//для поиска по каталогам или одному каталогу
    QList<BookItem*> booksList;//для поиска по книгам или в отдельной книге
    QList<textItem> textItems;

    QList<QAction*> listActions;
    QAction *exportTextToFile;
    QAction *exportTextToDisplay;
    QAction *exportResultToFile;
    QAction *exportResultToDisplay;

    //Переменные для подсветки
    QRegexpHighlighter *highlighter1;
    QRegexpHighlighter *highlighter2;

    CatalogsSelector *catalogs_selector;
    BooksSelector *books_selector;

    Ui::SearchWindow *ui;

private:
    bool checkRegExp(QRegExp rx);

public:
    explicit SearchWindow(Storage *s, QWidget *parent = 0);
    ~SearchWindow();

    //Поиск по каталогам или книгам
    void findInCatalogs();
    void findInBooks();

    //Вспомогательные для поиска функции
    int findInOneText(int *c, QString txt);
    void addSearchItem(int cnt);

private slots:
    void contextMenuRequsted(const QPoint &p);
    void shutdown();

    //Поиск
    void find();
    void findInChapters();//экшен, привязанный к toolButton
    void on_edtSearch_returnPressed();//поиск по всем каталогам

    //Выбор каталогов, книг для поиска
    void selectBooks();//вызов BooksSelector
    void selectedBooks(QList<BookItem*> selectedBooks);//список книг, результат из chooseFinder
    void selectCatalogs();//вызов CatalogSelector
    void selectedCatalogs(QList<Catalog*> catalogs);
    void chooseFont();

    //Клики по спискам
    void on_lstResults_clicked(const QModelIndex &index);
    void on_lstText_clicked(const QModelIndex &index);

    void text_display_Export();//экспорт цитат только из текста, по которому кликнули мышкой
    void text_file_Export();
    void result_display_Export();//экспорт цитат из всего списка, где она встречается
    void result_file_Export();

    void on_edtSearch_editingFinished();

signals:
    void sendPattern(QString value);
    void sendAll(BookItem*,ListItem*,TextItem*,QString);   
};

#endif // SEARCHWINDOW_H
