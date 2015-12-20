QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = kpf-qt
TEMPLATE = app

RC_FILE += src/res/resource.rc

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/inireader.cpp \
    src/registryreader.cpp \
    src/aboutdialog.cpp \
    src/logger.cpp

HEADERS  += src/mainwindow.h \
    src/inireader.h \
    src/registryreader.h \
    src/refs.h \
    src/aboutdialog.h \
    src/version.h \
    src/logger.h

FORMS    += src/mainwindow.ui \
    src/aboutdialog.ui

RESOURCES += src/qt_resource.qrc
