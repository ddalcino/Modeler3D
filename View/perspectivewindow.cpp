#include "perspectivewindow.h"
#include "ui_perspectivewindow.h"
#include "editcameradialog.h"
#include "editobjectdialog.h"
#include <QResizeEvent>
#include <QSize>

PerspectiveWindow::PerspectiveWindow(TreeViewWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::PerspectiveWindow),
    editCameraDlg(new EditCameraDialog(this)),
    editObjectDlg(new EditObjectDialog(this)),
    tvWindow(parent)

{
    if (!parent) {
        throw "PerspectiveWindow must be a child of a TreeViewWindow to communicate with the model.";
    }
    ui->setupUi(this);
    ui->perspectiveGLWidget->setTreeModel(tvWindow->getTreeModel());

    // Any time the model changes in an important way, notify this object and update it
    QObject::connect(tvWindow, SIGNAL(model_changed()),
                     this, SLOT(updateChildren()));
    QObject::connect(tvWindow, SIGNAL(selectionChanged()),
                     this, SLOT(updateChildren()));
}

PerspectiveWindow::~PerspectiveWindow()
{
    delete ui;
}

void PerspectiveWindow::resizeEvent(QResizeEvent *e)
{
    ui->perspectiveGLWidget->resizeGL(e->size().width(), e->size().height());
}

void PerspectiveWindow::updateChildren()
{
    ui->perspectiveGLWidget->update();
}

const QVector3D &PerspectiveWindow::getCameraPos() const {
    return ui->perspectiveGLWidget->getCameraPos();
}

void PerspectiveWindow::setCameraPos(const QVector3D &pos) {
    ui->perspectiveGLWidget->setCameraPos(pos);
}

float PerspectiveWindow::getFov() const {
    return ui->perspectiveGLWidget->getFov();
}

void PerspectiveWindow::setFov(float f) {
    ui->perspectiveGLWidget->setFov(f);
}

TreeViewWindow *PerspectiveWindow::getTvWindow() {
    return (TreeViewWindow *)this->parent();
}

/**
 * @brief PerspectiveWindow::validateMouseBehaviorType
 * Maybe don't use? It caused some seg faults; not sure why
 * @param newest
 */
void PerspectiveWindow::validateMouseBehaviorType(Perspective3DWidget::MouseDragType newest) {
    // only one of the mouse behavior types is allowed
    if(ui->actionRotate_Camera->isChecked() && newest != Perspective3DWidget::RotCam) {
        ui->actionRotate_Camera->setChecked(false);
    }
    if(ui->action_Rotate_Selected_Object->isChecked() && newest != Perspective3DWidget::RotObj) {
        ui->action_Rotate_Selected_Object->setChecked(false);
    }
    if(ui->actionTranslate_Camera_XY->isChecked() && newest != Perspective3DWidget::TrCamXY) {
        ui->actionTranslate_Camera_XY->setChecked(false);
    }
    if(ui->actionTranslate_Camera_X_Z->isChecked() && newest != Perspective3DWidget::TrCamXZ) {
        ui->actionTranslate_Camera_XY->setChecked(false);
    }
    if(ui->actionTranslate_Camera_YZ->isChecked() && newest != Perspective3DWidget::TrCamYZ) {
        ui->actionTranslate_Camera_YZ->setChecked(false);
    }
    if(ui->actionTranslate_Obj_ect_XY->isChecked() && newest != Perspective3DWidget::TrObjXY) {
        ui->actionTranslate_Obj_ect_XY->setChecked(false);
    }
    if(ui->actionTranslate_Objec_t_XZ->isChecked() && newest != Perspective3DWidget::TrObjXZ) {
        ui->actionTranslate_Objec_t_XZ->setChecked(false);
    }
    if(ui->actionTranslate_Obje_ct_YZ->isChecked() && newest != Perspective3DWidget::TrObjYZ) {
        ui->actionTranslate_Obje_ct_YZ->setChecked(false);
    }
}

void PerspectiveWindow::on_actionEdit_Camera_triggered()
{
    editCameraDlg->show();
}

void PerspectiveWindow::on_actionEdit_Object_triggered()
{
    editObjectDlg->show();

    while ((geometryEngine = ui->perspectiveGLWidget->getGeometryEngine())
           == NULL) {
        qDebug() << "No geometries yet!!";
    }
    editObjectDlg->init();//geometryEngine);

}

//void PerspectiveWindow::on_action_Cube_triggered()
//{
//    ui->perspectiveGLWidget->changeObject(PrimitiveDefinition::CUBE);
//    updateChildren();
//}

//void PerspectiveWindow::on_action_Sphere_triggered()
//{
//    ui->perspectiveGLWidget->changeObject(PrimitiveDefinition::SPHERE);
//    updateChildren();
//}

//void PerspectiveWindow::on_actionC_ylinder_triggered()
//{
//    ui->perspectiveGLWidget->changeObject(PrimitiveDefinition::CYLINDER);
//    updateChildren();
//}

//void PerspectiveWindow::on_actionC_one_triggered()
//{
//    ui->perspectiveGLWidget->changeObject(PrimitiveDefinition::CONE);
//    updateChildren();
//}

void PerspectiveWindow::on_action_Wireframe_toggled(bool arg1) {
    ui->perspectiveGLWidget->setWireframeMode(arg1);
    update();
}

void PerspectiveWindow::on_actionShow_Grid_toggled(bool arg1) {
    ui->perspectiveGLWidget->setShowGrid(arg1);
    update();
}

void PerspectiveWindow::on_actionShow_Axes_toggled(bool arg1) {
    ui->perspectiveGLWidget->setShowAxes(arg1);
    update();
}

void PerspectiveWindow::on_action_Rotate_Selected_Object_toggled(bool arg1) {
    if (arg1) {
        validateMouseBehaviorType(Perspective3DWidget::RotObj);
        ui->perspectiveGLWidget->setMouseDragType(Perspective3DWidget::RotObj);
    } else {
        //ui->action_Rotate_Selected_Object->setChecked(true);
    }
}

void PerspectiveWindow::on_actionRotate_Camera_toggled(bool arg1) {
    if (arg1) {
        validateMouseBehaviorType(Perspective3DWidget::RotCam);
        ui->perspectiveGLWidget->setMouseDragType(Perspective3DWidget::RotCam);
    } else {
        //ui->actionRotate_Camera->setChecked(true);
    }
}
