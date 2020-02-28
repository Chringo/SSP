#-------------------------------------------------
#
# Project created by QtCreator 2020-02-28T12:06:30
#
#-------------------------------------------------

QT       += widgets

TARGET = ResourceBrowser
TEMPLATE = lib

DEFINES += RESOURCEBROWSER_LIBRARY

SOURCES += \
    browserUi.cpp \
    resourceBrowser.cpp

HEADERS +=\
    browserUi.h \
    resourceBrowser.h \
    resourceBrowser_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

FORMS += \
    browserUi.ui
