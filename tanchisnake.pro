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
        g_s.h \
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
    image/img.qrc

DISTFILES += \
    guide/1.txt \
    guide/10.txt \
    guide/2.txt \
    guide/3.txt \
    guide/4.txt \
    guide/5.txt \
    guide/6.txt \
    guide/7.txt \
    guide/8.txt \
    guide/9.txt \
    image/60ed4209cccbf115b7a47d7b075c0ca5.jpeg \
    image/background.jpeg \
    image/background.png \
    image/cd1.png \
    image/cd2.png \
    image/cd3.png \
    image/cdgo.png \
    image/food.jpg \
    image/food1.jpg \
    image/food1.png \
    image/food2.jpeg \
    image/food2.png \
    image/food3.jpeg \
    image/food3.png \
    image/health.png \
    image/hptext.png \
    image/magnet.png \
    image/marsh.jpg \
    image/marsh.png \
    image/no_health.png \
    image/snakebody_down.png \
    image/snakebody_left.png \
    image/snakebody_right.png \
    image/snakebody_up.png \
    image/snakehead_down.png \
    image/snakehead_left.png \
    image/snakehead_right.png \
    image/snakehead_up.png \
    image/wall.jpeg \
    image/wall.png \
    map/addwallgame.txt \
    map/level10.txt \
    map/level3.txt \
    map/level4.txt \
    map/level5.txt \
    map/level6.txt \
    map/level7.txt \
    map/level9.txt \
    map/testaisnake.txt
