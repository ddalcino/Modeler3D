#include "perspectivewindow.h"
#include "ui_perspectivewindow.h"
#include <QResizeEvent>
#include <QSize>

PerspectiveWindow::PerspectiveWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PerspectiveWindow),
    editCameraDlg(new EditCameraDialog(this)),
    editObjectDlg(new EditObjectDialog(this))

{
    ui->setupUi(this);
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
    editObjectDlg->init(geometryEngine);

}

void PerspectiveWindow::on_action_Cube_triggered()
{
    ui->perspectiveGLWidget->changeObject(PrimitiveDefinition::CUBE);
    updateChildren();
}

void PerspectiveWindow::on_action_Sphere_triggered()
{
    ui->perspectiveGLWidget->changeObject(PrimitiveDefinition::SPHERE);
    updateChildren();
}

void PerspectiveWindow::on_actionC_ylinder_triggered()
{
    ui->perspectiveGLWidget->changeObject(PrimitiveDefinition::CYLINDER);
    updateChildren();
}

void PerspectiveWindow::on_actionC_one_triggered()
{
    ui->perspectiveGLWidget->changeObject(PrimitiveDefinition::CONE);
    updateChildren();
}
