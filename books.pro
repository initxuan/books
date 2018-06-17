#-------------------------------------------------
#
# Project created by QtCreator 2018-05-21T17:24:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = books
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    updatebynetwork.cpp

HEADERS  += dialog.h \
    updatebynetwork.h

FORMS    += dialog.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    ini/config.ini \
    ini/updateversion.ini
