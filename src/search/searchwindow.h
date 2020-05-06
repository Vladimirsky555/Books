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

struct searchItem {
    QString text1;
    QString text2;
    QString text3;
    QString text4;
    QString text5;
    QString text6;
    int num;
    int n;
};

//Структура, описывающая номер строки, в которой найдено искомое слово
struct textItem {
    int id;
};

class SearchWindow : public QWidget
{
    Q_OBJECT

    QList<loadItem> LoadItems;
    QList<BookItem*> books;
    QList<QString> list;
    QList<searchItem> searchItems;
    QList<textItem> textItems;

    BookItem* currentBook;
    BookItem* tmp;
    ListItem* currentList;
    ListItem* tmpList;
    TextItem* currentText;
    QString currentTitle;
    QString currentTxt;


    QRegexpHighlighter *highlighter1;
    QRegexpHighlighter *highlighter2;

    FindChooser *widget_findchooser;

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

private:
    Ui::SearchWindow *ui;
    bool checkRegExp(QRegExp rx);
};

#endif // SEARCHWINDOW_H
