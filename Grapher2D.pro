#-------------------------------------------------
#
# Project created by QtCreator 2015-12-23T19:53:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Grapher2D
TEMPLATE = lib

CONFIG += dll

DESTDIR = $$PWD/../Library

SOURCES += grapher2d.cpp

HEADERS  += grapher2d.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
