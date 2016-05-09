#-------------------------------------------------
#
# Project created by QtCreator 2016-05-07T20:14:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Modeler3D
TEMPLATE = app


SOURCES += main.cpp\
    View/perspective3dwidget.cpp \
    View/geometryengine.cpp
#    View/mainwindow.cpp

HEADERS  += \
    View/perspective3dwidget.h \
    View/geometryengine.h
#    View/mainwindow.h

#FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    shaders.qrc

