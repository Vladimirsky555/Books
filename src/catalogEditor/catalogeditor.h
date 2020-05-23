#ifndef CATALOGEDITOR_H
#define CATALOGEDITOR_H

#include <QWidget>

#include "data/datatypes.h"
#include "dialog/nameenter.h"

class ListItem;

namespace Ui {
class catalogEditor;
}

class catalogEditor : public QWidget
{
    Q_OBJECT

private:
    Catalog *catalog;
    BookItem* currentBook;
    ListItem* currentChapter;
    TextItem* currentSection;

    QList<QAction*> bookActions;
    QAction *bookUp;
    QAction *bookDown;
    QAction *bookInsertFirst;
    QAction *bookInsert;
    QAction *bookInsertEnd;
    QAction *bookEdit;
    QAction *bookDelete;

    QList<QAction*> chapterActions;
    QAction *chapterUp;
    QAction *chapterDown;
    QAction *chapterInsertFirst;
    QAction *chapterInsert;
    QAction *chapterInsertEnd;
    QAction *chapterEdit;
    QAction *chapterDelete;

    QList<QAction*> sectionActions;
    QAction *sectionUp;
    QAction *sectionDown;
    QAction *sectionInsertFirst;
    QAction *sectionInsert;
    QAction *sectionInsertEnd;
    QAction *sectionEdit;
    QAction *sectionDelete;

public:
    catalogEditor(Catalog *catalog, QWidget *parent = nullptr);
    ~catalogEditor();

    static catalogEditor *Current(Catalog *catalog);
    void addActions();
    void refreshBooks();
    void refreshChapters();
    void refreshSections();

private slots:
    void contextMenuRequsted(const QPoint &p);

    //События
    void book_Up();
    void book_Down();
    void book_Insert_First();
    void book_Insert();
    void book_Insert_End();
    void book_Edit();
    void book_Delete();

    void chapter_Up();
    void chapter_Down();
    void chapter_Insert_First();
    void chapter_Insert();
    void chapter_Insert_End();
    void chapter_Edit();
    void chapter_Delete();

    void section_Up();
    void section_Down();
    void section_Insert_First();
    void section_Insert();
    void section_Insert_End();
    void section_Edit();
    void section_Delete();

    void on_btnSaveText_clicked();//сохранение текста

    //Клик по полю
    void on_lstBooks_clicked(const QModelIndex &index);
    void on_lstChapters_clicked(const QModelIndex &index);
    void on_lstSections_clicked(const QModelIndex &index);

    void shutdown();

private:
    Ui::catalogEditor *ui;
};

#endif // CATALOGEDITOR_H
