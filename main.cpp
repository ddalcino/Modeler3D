//#include "View/mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#ifndef QT_NO_OPENGL
#include "Controller/treeviewwindow.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifndef QT_NO_OPENGL

    TreeViewWindow tw(NULL);
    tw.show();

#else
    QLabel note("OpenGL Support required");
    note.show();
#endif

    return a.exec();
}
