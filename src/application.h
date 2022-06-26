#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
//#include <QTranslator>


class Application : public QApplication
{

    Q_OBJECT

//    QTranslator translator;

public:
    Application(int argc, char *argv[]);
};

#endif // APPLICATION_H
