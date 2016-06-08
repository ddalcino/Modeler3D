#include "editcameradialog.h"
#include "ui_editcameradialog.h"
#include "perspectivewindow.h"

#include <QQuaternion>

EditCameraDialog::EditCameraDialog(PerspectiveWindow *parent) :
    QDialog(parent),
    ui(new Ui::EditCameraDialog),
    parent(parent)
{
}

EditCameraDialog::~EditCameraDialog()
{
    delete ui;
}

void EditCameraDialog::init()
{
    static const int MAX = 100000;
    ui->setupUi(this);
    ui->doubleInputFarDrawDist->init("Draw Distance", MAX, 0, 0, 20, 10, true, false);
    ui->doubleInputCamFov->init("Camera FOV", 180, 0, 10, 90, 45, true, true, 180, 1);
    ui->doubleInputCamPosX->init("Camera X Pos", MAX, -MAX, -10, 10, 0, false, false);
    ui->doubleInputCamPosY->init("Camera Y Pos", MAX, -MAX, -10, 10, 0, false, false);
    ui->doubleInputCamPosZ->init("Camera Z Pos", MAX, -MAX, -20, 0, -5, false, false);
    ui->doubleInputGridScale->init("Grid Scale", MAX, 0, 0, 10, 1, true, false);
}

void EditCameraDialog::on_doubleInputCamFov_changed_value(double val)
{
    parent->setFov(val);
}

void EditCameraDialog::on_doubleInputCamPosX_changed_value(double val){
    setCamPos(X, val);
}

void EditCameraDialog::setCamPos(EditCameraDialog::Dim d, float amt){
    QVector3D pos = parent->getCameraPos();
    switch(d) {
    case X: pos.setX(amt); break;
    case Y: pos.setY(amt); break;
    case Z: pos.setZ(amt); break;
    }
    parent->setCameraPos(pos);
}

void EditCameraDialog::setCamRot(Dim axis){
    switch (axis) {
    case X:
        parent->setCameraRot(QQuaternion::fromAxisAndAngle(0, 1, 0, 90));
        break;
    case Y:
        parent->setCameraRot(QQuaternion::fromAxisAndAngle(1, 0, 0, 90));
        break;
    case Z:
        parent->setCameraRot(QQuaternion());
        break;
    }
}

QVector3D EditCameraDialog::makePosOnAxis(EditCameraDialog::Dim d, float distance){
    switch (d) {
    case X: return QVector3D(-distance, 0, 0);
    case Y: return QVector3D(0, -distance, 0);
    case Z: return QVector3D(0, 0, -distance);
    }
    return QVector3D();
}

void EditCameraDialog::on_doubleInputCamPosY_changed_value(double val){
    setCamPos(Y, val);
}

void EditCameraDialog::on_doubleInputCamPosZ_changed_value(double val){
    setCamPos(Z, val);
}

void EditCameraDialog::on_doubleInputFarDrawDist_changed_value(double val)
{
    parent->setDrawDist(val);
}

void EditCameraDialog::on_doubleInputGridScale_changed_value(double val)
{
    qDebug() << "on_doubleInputGridScale_changed_value=" << val;
    parent->setGridScale(val);
}

void EditCameraDialog::on_lookXAxis_clicked()
{
    setCamPos(X, 0);
    setCamPos(Y, 0);
    setCamRot(X);
}

void EditCameraDialog::on_lookYAxis_clicked()
{
    setCamPos(X, 0);
    setCamPos(Y, 0);
    setCamRot(Y);
}

void EditCameraDialog::on_lookZAxis_clicked()
{
    setCamPos(X, 0);
    setCamPos(Y, 0);
    setCamRot(Z);
}
