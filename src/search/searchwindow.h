#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include <QWidget>
#include <QRegExp>
#include <QMessageBox>

#include "data/searchitem.h"
#include "data/searchstorage.h"
#include "helpers/qregexphighlighter.h"
#include "helpers/searchitemsmaker.h"
#include "search/booksselector.h"
#include "search/catalogsselector.h"

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
    SearchStorage *ss;
    SearchItemsMaker *sim;//Переменная для формирования результата поиска и текста с цитатами


    Catalog *currentCatalog;
    BookItem* currentBook;
    ListItem* currentChapter;
    TextItem* currentSection;
    QString currentTitle;
    QString currentText;

    bool resource;

    //QList<searchItem*> searchItems; //Массив структур, формирующийся по результатам поиска
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

public:
    explicit SearchWindow(Storage *s, QWidget *parent = 0);
    ~SearchWindow();

    //Поиск по каталогам или книгам
    void findInCatalogs();
    void findInBooks();

    void prepareWidgets();//подготовка виджетов
    int showResult();//Отображение результатов поиска в нижнем виджете
    void report(int c, int n);//отчёт о результатах

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

    //подсветка
    void on_edtSearch_editingFinished();

signals:
    void sendPattern(QString value);
    void sendAll(BookItem*,ListItem*,TextItem*,QString);   
};

#endif // SEARCHWINDOW_H
