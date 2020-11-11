#include "worker.h"

#include <QThread>
#include <QMessageBox>
#include <QRegExp>
#include <QDebug>

Worker::Worker(Storage *s, Catalog *currentCatalog, QString searchText, QObject *parent) :
     QObject(parent), QRunnable ()
{
    this->s = s;
    this->currentCatalog = currentCatalog;
    this->searchText = searchText;
    this->c = 0;
}

bool Worker::checkRegExp(QRegExp rx)
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

void Worker::run()
{
    qInfo() << QThread::currentThread();
    int cnt = 0;
    for(int l = 0; l < currentCatalog->getCount(); l++){
        currentBook = currentCatalog->getBookById(l);

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
                    si->_catalog = currentCatalog;
                    si->_book = currentBook;
                    si->_chapter = currentChapter;
                    si->_section = currentSection;
                    si->textCount = cnt;
                    s->addSearchItem(si);
                }
                cnt = 0;
            }
        }
    }
    s->setC(c);
}


