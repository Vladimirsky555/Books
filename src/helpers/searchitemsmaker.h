#ifndef SEARCHITEMSMAKER_H
#define SEARCHITEMSMAKER_H

#include <QObject>

#include "data/storage.h"
#include "data/searchstorage.h"

//Класс для формирования списка текстов, в которых найдено хотя бы одно
//ключевое слово, а также списка цитат с найденным хотя бы одни ключевым словом, на
//основе этих текстов

class SearchItemsMaker : public QObject
{
    Q_OBJECT

    Storage *s;
    SearchStorage *ss;
    QString pattern;

public:
    SearchItemsMaker(Storage *s, SearchStorage *ss, QString pattern,
                              QObject *parent = nullptr);

    //формирует массив текстов, в которых есть искомое слово
    int searchItemsMakerfromCatalogs();
    int searchItemsMakerfromBooks(QList<BookItem*> booksList);
    int searchItemsMakerInChapters(ListItem *currentChapter, TextItem *currentSection);

     //формирует текст с цитатами. В программе QuoteBook это массив QList<qoteItem*>
    QString quoteMaker();

    //Вспомогательные для поиска функции
    int findInOneText(int *c, QString txt, QString pattern);
    bool checkRegExp(QRegExp rx);
};

#endif // SEARCHITEMSMAKER_H
