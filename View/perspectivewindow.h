#ifndef PERSPECTIVEWINDOW_H
#define PERSPECTIVEWINDOW_H

#include <QMainWindow>

#include "geometryengine.h"
#include "treeviewwindow.h"
#include "perspective3dwidget.h"

//#include "../Model/treemodel.h"

namespace Ui {
class PerspectiveWindow;
}
class EditCameraDialog;
class EditObjectDialog2;

class PerspectiveWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PerspectiveWindow(TreeViewWindow *parent=NULL);
    ~PerspectiveWindow();

    void resizeEvent(QResizeEvent *e);


    const QVector3D& getCameraPos() const;
    void setCameraPos(const QVector3D& pos);
    float getFov() const;
    void setFov(float f);

    TreeViewWindow *getTvWindow();

private:
    Ui::PerspectiveWindow *ui;
    EditCameraDialog *editCameraDlg;
    EditObjectDialog2 *editObjectDlg;

    //TreeModel *model;
    TreeViewWindow *tvWindow;

    GeometryEngine *geometryEngine;

    void validateMouseBehaviorType(Perspective3DWidget::MouseDragType newest);

public slots:
    void updateChildren();

private slots:
    void on_actionEdit_Camera_triggered();
    void on_actionEdit_Object_triggered();
//    void on_action_Cube_triggered();
//    void on_action_Sphere_triggered();
//    void on_actionC_ylinder_triggered();
//    void on_actionC_one_triggered();
    void on_action_Wireframe_toggled(bool arg1);
    void on_actionShow_Grid_toggled(bool arg1);
    void on_actionShow_Axes_toggled(bool arg1);
    void on_action_Rotate_Selected_Object_toggled(bool arg1);
    void on_actionRotate_Camera_toggled(bool arg1);
};

#endif // PERSPECTIVEWINDOW_H
