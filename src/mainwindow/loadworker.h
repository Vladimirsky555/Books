#ifndef LOADWORKER_H
#define LOADWORKER_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QMutexLocker>

#include <data/catalog.h>
#include <data/bookitem.h>
#include <data/storage.h>

class LoadWorker : public QObject, public QRunnable
{
    Q_OBJECT

  QMutex* mutex;
  Storage *s;
  QList<BookItem*> currentBooks;
  QString name;
  QString path;

public:
    LoadWorker(Storage *s, QString name, QString path, QMutex* mutex = nullptr, QObject *parent = nullptr);

    void run();
};

#endif // LOADWORKER_H
