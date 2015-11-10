#-------------------------------------------------
#
# Project created by QtCreator 2015-10-26T17:22:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kpf-qt
TEMPLATE = app


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/inireader.cpp \
    src/registryreader.cpp \
    src/aboutdialog.cpp

HEADERS  += src/mainwindow.h \
    src/inireader.h \
    src/registryreader.h \
    src/refs.h \
    src/aboutdialog.h

FORMS    += src/mainwindow.ui \
    src/aboutdialog.ui
