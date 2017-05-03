#-------------------------------------------------
#
# Project created by QtCreator 2016-11-25T02:26:49
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlashMonitor
TEMPLATE = app

CONFIG += c++11

QMAKE_CXXFLAGS += -O0

SOURCES += main.cpp\
        mainwindow.cpp \
    usbbase.cpp \
    usbinfo.cpp \
    usbinfofinder.cpp \
    testwindow.cpp \
    usbclient.cpp \
    myclient.cpp \
    DB/dbfacade.cpp \
    DB/host.cpp \
    DB/hostrepository.cpp \
    DB/rule.cpp \
    DB/rulerepository.cpp \
    DB/usb.cpp \
    DB/usbrepository.cpp

HEADERS  += mainwindow.h \
    usbbase.h \
    usbinfo.h \
    usbinfofinder.h \
    testwindow.h \
    usbclient.h \
    myclient.h \
    DB/basesqlrepository.h \
    DB/dbfacade.h \
    DB/host.h \
    DB/hostrepository.h \
    DB/irepository.h \
    DB/property.h \
    DB/rule.h \
    DB/rulerepository.h \
    DB/usb.h \
    DB/usbrepository.h

FORMS += \
    testwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    DB/schema.sql
