#-------------------------------------------------
#
# Project created by QtCreator 2023-03-25T10:55:07
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Monopoly
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Goods.cpp \
        main.cpp \
        monopolygame.cpp \
    Money.cpp \
    Knapsack.cpp \
    Road.cpp \
    House.cpp \
    Bank.cpp \
    GameObject.cpp \
    AbstractMap.cpp \
    AbstractPlayer.cpp \
    OperablePlace.cpp \
    MapFactory.cpp \
    Grass.cpp \
    UndefinedMap.cpp \
    Player.cpp \
    land.cpp \
    prop.cpp \
    Affect.cpp \
    Buff.cpp \
    Debuff.cpp \
    GameStateController.cpp \
    GameRoundController.cpp \
    Controller.cpp \
    PlayerStateController.cpp \
    propFactory.cpp \
    sleepy.cpp \
    sleepcard.cpp \
    shop.cpp \
    blackhole.cpp \
    musicthread.cpp

HEADERS += \
    Goods.h \
        monopolygame.h \
    AbstractPlayer.h \
    AbstractMap.h \
    Money.h \
    Prop.h \
    Road.h \
    OperablePlace.h \
    House.h \
    Bank.h \
    GameObject.h \
    Knapsack.h \
    MapFactory.h \
    Grass.h \
    UndefinedMap.h \
    Player.h \
    land.h \
    Affect.h \
    Buff.h \
    Debuff.h \
    Controller.h \
    GameStateController.h \
    GameRoundController.h \
    PlayerStateController.h \
    propFactory.h \
    sleepy.h \
    sleepycard.h \
    shop.h \
    blackhole.h \
    musicthread.h

FORMS += \
        monopolygame.ui


RESOURCES += \
    res.qrc

DISTFILES +=
