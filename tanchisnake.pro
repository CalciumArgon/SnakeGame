#-------------------------------------------------
#
# Project created by QtCreator 2023-07-02T22:35:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tanchisnake
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

CONFIG += c++11

SOURCES += \
        aisnake.cpp \
        clock.cpp \
        dialog.cpp \
        field.cpp \
        game.cpp \
        gamewidget.cpp \
        item.cpp \
        main.cpp \
        mainwindow.cpp \
        qlevel.cpp \
        snake.cpp

HEADERS += \
        aisnake.h \
        clock.h \
        dialog.h \
        field.h \
        game.h \
        gamewidget.h \
        item.h \
        mainwindow.h \
        path.h \
        qlevel.h \
        snake.h

FORMS += \
        dialog.ui \
        gamewidget.ui \
        mainwindow.ui \
        qlevel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image/image.qrc \
    image/image.qrc

DISTFILES += \
    map/addwallgame.txt \
    map/level3.txt \
    map/level4.txt \
    map/level5.txt \
    map/level6.txt \
    map/testaisnake.txt
