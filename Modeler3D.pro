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
    View/geometryengine.cpp \
    View/primitivedefinition.cpp \
    View/perspectivewindow.cpp \
    View/editcameradialog.cpp \
    Model/globject.cpp \
    Model/treemodel.cpp \
    View/treeviewwindow.cpp \
    Model/scenexmlhandler.cpp \
    global_structs.cpp \
    View/doubleinputform.cpp \
    View/editobjectdialog2.cpp
#    View/scenegl.cpp
#    View/mainwindow.cpp

HEADERS  += \
    View/perspective3dwidget.h \
    View/geometryengine.h \
    View/primitivedefinition.h \
    View/perspectivewindow.h \
    View/trackball.h \
    View/editcameradialog.h \
    Model/globject.h \
    Model/treemodel.h \
    View/treeviewwindow.h \
    Model/scenexmlhandler.h \
    global_structs.h \
    View/doubleinputform.h \
    View/editobjectdialog2.h
#    View/scenegl.h
#    View/mainwindow.h

INCLUDEPATH += View/


#FORMS    += mainwindow.ui

DISTFILES +=

RESOURCES += \
    shaders.qrc

CONFIG += c++11

FORMS += \
    View/perspectivewindow.ui \
    View/editcameradialog.ui \
    View/editobjectdialog.ui \
    View/treeviewwindow.ui \
    View/editobjectdialog2.ui \
    View/doubleinputform.ui

