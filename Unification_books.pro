#-------------------------------------------------
#
# Project created by QtCreator 2017-10-26T11:45:30
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 5.0
QMAKE_TARGET_COMPANY = BearWin
QMAKE_TARGET_PRODUCT = Unification Books
QMAKE_TARGET_DESCRIPTION = Unification Books
QMAKE_TARGET_COPYRIGHT = BearWin

TARGET = Unification_books
TEMPLATE = app

INCLUDEPATH += $${_PRO_FILE_PWD_}/src
win32: RC_ICONS = $$PWD/bin/images/book.ico

    RESOURCES       = \
        bin/images.qrc
	
	HEADERS  += src/mainwindow/mainwindow.h \
        src/application.h \
        src/catalogEditor/catalogeditor.h \
    src/catalogEditor/catalogseditor.h \
    src/data/bookitem.h \
    src/data/catalog.h \
    src/data/iteminfo.h \
    src/data/listitem.h \
    src/data/storage.h \
    src/data/textitem.h \
    src/dialog/catalogenter.h \
    src/dialog/exportdialog.h \
    src/dialog/logindialog.h \
        src/diary/iteminfoform.h \        
        src/helpers/qregexphighlighter.h \
        src/search/searchwindow.h \
        src/search/findchooser.h \
        src/info/list.h \
        src/info/info.h \
        src/dialog/nameenter.h

    SOURCES += src/main.cpp\
        src/application.cpp \
        src/catalogEditor/catalogeditor.cpp \
    src/catalogEditor/catalogseditor.cpp \
    src/data/catalog.cpp \
    src/data/storage.cpp \
    src/dialog/catalogenter.cpp \
    src/dialog/exportdialog.cpp \
    src/dialog/logindialog.cpp \
        src/mainwindow/mainwindow.cpp \
        src/data/bookitem.cpp \
        src/data/textitem.cpp \
        src/data/listitem.cpp \
        src/data/iteminfo.cpp \
        src/diary/iteminfoform.cpp \        
        src/helpers/qregexphighlighter.cpp \
        src/search/searchwindow.cpp \
        src/search/findchooser.cpp \
        src/info/list.cpp \
        src/info/info.cpp \
        src/dialog/nameenter.cpp

    FORMS += src/mainwindow/mainwindow.ui \
        src/catalogEditor/catalogeditor.ui \
    src/catalogEditor/catalogseditor.ui \
    src/dialog/catalogenter.ui \
    src/dialog/exportdialog.ui \
    src/dialog/logindialog.ui \
        src/diary/iteminfoform.ui \        
        src/search/searchwindow.ui \
        src/search/findchooser.ui \
        src/info/list.ui \
        src/info/info.ui \
        src/dialog/nameenter.ui

