#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QListWidgetItem>

#include "data/datatypes.h"
#include "diary/iteminfoform.h"
#include "search/searchwindow.h"
#include "helpers/qregexphighlighter.h"
#include "info/list.h"
#include "info/info.h"
#include "catalogEditor/catalogeditor.h"
#include "catalogEditor/catalogseditor.h"
#include "dialog/logindialog.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Storage *s;
    QList<BookItem*> currentBooks;//Массив книг каталога
    QStringList pathList;//Массив путей к каталогам
    QStringList catalogNamesList;

    Catalog *currentCatalog;
    BookItem* currentBook;
    ListItem* currentChapter;
    TextItem* currentSection;

    //Переменные виджетов
    ItemInfoForm *widget_o;
    SearchWindow *search_window;
    List *widget_list;
    CatalogsEditor *CatalogsEditor;
    LoginDialog login;

    QRegexpHighlighter *highlighter;
    QString title;

    bool admin;

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    void refreshCatalogs();
    void refreshBooks();
    void refreshChapters();
    void refreshSections();

    void setEnabledAll();
    ListItem* getItemByName(QString);

    //Чтение-запись в файл
    void loadCatalogs();
    void saveCatalogs();
    void loadData(QString path);
    void saveData();

private slots:    
    void setPattern(QString);
    void sendPattern(QString);
    void setAll(BookItem*bookName, ListItem*bookChapter, TextItem*bookSection, QString booksPath);
    void on_cbxCatalogs_currentIndexChanged(int index);
    void on_btnR_clicked();
    void on_lstChapters_clicked(const QModelIndex &index);
    void on_lstSections_clicked(const QModelIndex &index);

    void on_actionNotes_triggered();
    void on_actionContent_triggered();
    void on_actionSearch_triggered();
    void on_actionExport_triggered();
    void on_actionInfoDialog_triggered();
    void on_actionCatalogsEditor_triggered();
    void on_actionAuthorization_triggered();

    void on_edtPattern_textChanged(const QString &arg1);
    void on_btnFont_clicked();

signals:
    void shutdown();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
