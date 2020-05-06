#ifndef QREGEXPHIGHLIGHTER_H
#define QREGEXPHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>

class QRegexpHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    QString pattern;

public:
    explicit QRegexpHighlighter(QObject *parent = 0);

    void highlightBlock(const  QString &text);
    void setPattern(QString str){pattern = str;}

signals:

public slots:
};

#endif // QREGEXPHIGHLIGHTER_H
