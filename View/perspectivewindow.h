#ifndef PERSPECTIVEWINDOW_H
#define PERSPECTIVEWINDOW_H

#include <QMainWindow>

#include "editcameradialog.h"
#include "editobjectdialog.h"
#include "geometryengine.h"

namespace Ui {
class PerspectiveWindow;
}

class PerspectiveWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PerspectiveWindow(QWidget *parent = 0);
    ~PerspectiveWindow();

    void resizeEvent(QResizeEvent *e);

    void updateChildren();

private:
    Ui::PerspectiveWindow *ui;
    EditCameraDialog *editCameraDlg;
    EditObjectDialog *editObjectDlg;
    GeometryEngine *geometryEngine;

public slots:

private slots:
    void on_actionEdit_Camera_triggered();
    void on_actionEdit_Object_triggered();
    void on_action_Cube_triggered();
    void on_action_Sphere_triggered();
    void on_actionC_ylinder_triggered();
    void on_actionC_one_triggered();
};

#endif // PERSPECTIVEWINDOW_H
