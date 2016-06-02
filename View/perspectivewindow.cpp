#include "perspectivewindow.h"
#include "ui_perspectivewindow.h"
#include "editcameradialog.h"
#include "editobjectdialog.h"
#include <QResizeEvent>
#include <QSize>

PerspectiveWindow::PerspectiveWindow(TreeModel *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PerspectiveWindow),
    editCameraDlg(new EditCameraDialog(this)),
    editObjectDlg(new EditObjectDialog(this)),
    model(model)

{
    ui->setupUi(this);
    ui->perspectiveGLWidget->setTreeModel(model);
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

const QVector3D &PerspectiveWindow::getCameraPos() const { return ui->perspectiveGLWidget->getCameraPos(); }

void PerspectiveWindow::setCameraPos(const QVector3D &pos) { ui->perspectiveGLWidget->setCameraPos(pos); }

float PerspectiveWindow::getFov() const { return ui->perspectiveGLWidget->getFov(); }

void PerspectiveWindow::setFov(float f) { ui->perspectiveGLWidget->setFov(f); }

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
