#-------------------------------------------------
#
# Project created by QtCreator 2016-11-25T02:26:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlashMonitor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    usbbase.cpp \
    usbinfo.cpp \
    usbinfofinder.cpp \
    testwindow.cpp

HEADERS  += mainwindow.h \
    usbbase.h \
    usbinfo.h \
    usbinfofinder.h \
    testwindow.h

FORMS += \
    testwindow.ui
