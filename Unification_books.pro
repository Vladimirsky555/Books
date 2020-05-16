#-------------------------------------------------
#
# Project created by QtCreator 2017-10-26T11:45:30
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 4.0
QMAKE_TARGET_COMPANY = BearWin
QMAKE_TARGET_PRODUCT = Unification Books
QMAKE_TARGET_DESCRIPTION = Unification Books
QMAKE_TARGET_COPYRIGHT = BearWin

TARGET = Unification_books
TEMPLATE = app

INCLUDEPATH += $${_PRO_FILE_PWD_}/src
win32: RC_ICONS = $$PWD/bin/images/book.ico

    RESOURCES       = \
        bin/res1.qrc \
        bin/res2.qrc \
        bin/res3.qrc \
        bin/res4.qrc \
        bin/images.qrc
	
	HEADERS  += src/mainwindow/mainwindow.h \
        src/application.h \
        src/data/datatypes.h \
        src/diary/iteminfoform.h \        
        src/helpers/qregexphighlighter.h \
        src/search/searchwindow.h \
        src/search/listwidget.h \
        src/search/findchooser.h \
        src/info/list.h \
        src/info/info.h

    SOURCES += src/main.cpp\
       src/application.cpp \
        src/mainwindow/mainwindow.cpp \
        src/data/bookitem.cpp \
        src/data/textitem.cpp \
        src/data/listitem.cpp \
        src/data/iteminfo.cpp \
        src/diary/iteminfoform.cpp \        
        src/helpers/qregexphighlighter.cpp \        
        src/search/searchwindow.cpp \
        src/search/listwidget.cpp \
        src/search/findchooser.cpp \
        src/info/list.cpp \
        src/info/info.cpp

    FORMS += src/mainwindow/mainwindow.ui \
        src/diary/iteminfoform.ui \        
        src/search/searchwindow.ui \
        src/search/listwidget.ui \
        src/search/findchooser.ui \
        src/info/list.ui \
        src/info/info.ui

