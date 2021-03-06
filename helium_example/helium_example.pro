#-------------------------------------------------
#
# Project created by QtCreator 2017-12-20T21:50:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = helium_example
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp

HEADERS +=

FORMS +=

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-helium-Desktop_Qt_5_9_0_MSVC2015_64bit-Debug/release/ -lhelium
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-helium-Desktop_Qt_5_9_0_MSVC2015_64bit-Debug/debug/ -lhelium
else:unix: LIBS += -L$$PWD/../build-helium-Desktop-Debug/ -lhelium

INCLUDEPATH += $$PWD/../helium
DEPENDPATH += $$PWD/../helium

HEADERS += $$PWD/../helium
