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
    Model/scenexmlhandler.cpp \
    global_structs.cpp \
    View/doubleinputform.cpp \
    View/editobjectdialog2.cpp \
    Controller/treeviewwindow.cpp \
    View/aboutdialog.cpp \
    View/helpdialog.cpp

HEADERS  += \
    View/perspective3dwidget.h \
    View/geometryengine.h \
    View/primitivedefinition.h \
    View/perspectivewindow.h \
    View/trackball.h \
    View/editcameradialog.h \
    Model/globject.h \
    Model/treemodel.h \
    Model/scenexmlhandler.h \
    global_structs.h \
    View/doubleinputform.h \
    View/editobjectdialog2.h \
    Controller/treeviewwindow.h \
    View/aboutdialog.h \
    View/helpdialog.h

INCLUDEPATH += View/


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
    View/doubleinputform.ui \
    View/aboutdialog.ui \
    View/helpdialog.ui

