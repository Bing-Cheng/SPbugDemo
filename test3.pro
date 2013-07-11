#-------------------------------------------------
#
# Project created by QtCreator 2013-07-10T11:25:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test3
TEMPLATE = app
DEPENDPATH += . /home/bcheng/workspacecpp/test2/Lib/sup
INCLUDEPATH +=  /home/bcheng/workspacecpp/test2/Lib/sup
LIBS+= -L/home/bcheng/workspacecpp/test2/Lib/sup
LIBS+= -lViesus
LIBS+= -lcv
LIBS+= -llcms
LIBS+= -lpthread

SOURCES += main.cpp\
        mainwindow.cpp \
    worker.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    worker.h \
    mythread.h

FORMS    += mainwindow.ui

