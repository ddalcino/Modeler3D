#ifndef PERSPECTIVEWINDOW_H
#define PERSPECTIVEWINDOW_H

#include <QMainWindow>

#include "geometryengine.h"
#include "Controller/treeviewwindow.h"
#include "perspective3dwidget.h"

#include <QQuaternion>


namespace Ui {
class PerspectiveWindow;
}
class EditCameraDialog;
class EditObjectDialog2;
class AboutDialog;
class HelpDialog;

class PerspectiveWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PerspectiveWindow(TreeViewWindow *parent=NULL);
    ~PerspectiveWindow();

    void resizeEvent(QResizeEvent *e);


    const QVector3D& getCameraPos() const;
    void setCameraPos(const QVector3D& pos);
    void setCameraRot(const QQuaternion& rot);
    float getFov() const;
    void setFov(float f);
    void setGridScale(double f);
    float getDrawDist() const;

    TreeViewWindow *getTvWindow();

private:
    Ui::PerspectiveWindow *ui;
    EditCameraDialog *editCameraDlg;
    EditObjectDialog2 *editObjectDlg;
    AboutDialog *aboutDialog;
    HelpDialog *helpDialog;

    //TreeModel *model;
    TreeViewWindow *tvWindow;

    GeometryEngine *geometryEngine;

    void validateMouseBehaviorType(Perspective3DWidget::MouseDragType newest);

public slots:
    void updateChildren();
    void setDrawDist(double d);

private slots:
    void on_actionEdit_Camera_triggered();
    void on_actionEdit_Object_triggered();
    void on_action_Wireframe_toggled(bool arg1);
    void on_actionShow_Grid_toggled(bool arg1);
    void on_actionShow_Axes_toggled(bool arg1);
    void on_action_Rotate_Selected_Object_toggled(bool arg1);
    void on_actionRotate_Camera_toggled(bool arg1);
    void on_actionTranslate_Obj_ect_XY_toggled(bool arg1);
    void on_actionInstructions_triggered();
    void on_actionAbout_triggered();
};

#endif // PERSPECTIVEWINDOW_H
