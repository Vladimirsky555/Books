#include "booksworker.h"

#include <QMessageBox>

BooksWorker::BooksWorker(Storage *s, BookItem *currentBook, QString searchText, QObject *parent) :
    QObject(parent), QRunnable ()
{
    this->s = s;
    this->currentBook = currentBook;
    this->searchText = searchText;
    this->c = 0;
}

bool BooksWorker::checkRegExp(QRegExp rx)
{
    if(rx.isValid() && !rx.isEmpty() && !rx.exactMatch("")){
        return true;
    } else {
        QMessageBox b;
        b.setText("Некорректный шаблон регулярного выражения!");
        b.exec();
        return false;
    }
}

void BooksWorker::run()
{
    int cnt = 0;
    currentCatalog = s->getCatalogByBook(currentBook);

    for(int i = 0; i < currentBook->getCount(); i++){
        currentChapter = currentBook->getChapterById(i);

        for(int j = 0; j < currentChapter->getCount(); j++){
            currentSection = currentChapter->getSectionById(j);

            QRegExp rx(searchText);
            if(!checkRegExp(rx)) return;
            int pos = 0;
            while((pos = rx.indexIn(currentSection->getData(), pos)) != -1){
                pos += rx.matchedLength();
                c += 1;
                cnt++;
            }

            if(cnt != 0){
                SearchItem *si = new SearchItem();
                si->setCatalog(currentCatalog);
                si->setBook(currentBook);
                si->setChapter(currentChapter);
                si->setSection(currentSection);
                si->setTextCount(cnt);
                s->addSearchItem(si);
            }
            cnt = 0;
        }
    }

    s->setC(c);
}

