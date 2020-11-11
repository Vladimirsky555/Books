#include "loadworker.h"

#include <QFile>
#include <QDataStream>
#include <QThread>
#include <QDebug>

LoadWorker::LoadWorker(Storage *s, QString name, QString path, QMutex *mutex, QObject *parent)  :
    QObject(parent), QRunnable ()
{
    this->mutex = mutex;
    this->s = s;
    this->name = name;
    this->path = path;
}

void LoadWorker::run()
{
    Catalog *catalog = new Catalog(name, path);

    currentBooks.clear();
    QFile f(path);
    if(!f.exists()) return;

    QMutexLocker locker(mutex);

    f.open(QFile::ReadOnly);
    QDataStream reader(&f);


    while(!reader.atEnd()){
        QByteArray arr;
        reader >> arr;
        currentBooks.append(new BookItem(arr));
    }

    f.close();

    catalog->setBook(currentBooks);
    s->addCatalog(catalog);
}
