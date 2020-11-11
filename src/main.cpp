#include "mainwindow/mainwindow.h"
#include "application.h"


int main(int argc, char *argv[])
{
    Application app(argc, argv);

    MainWindow w;
    w.show();



    return app.exec();
}


