#include "editcameradialog.h"
#include "ui_editcameradialog.h"
#include "perspectivewindow.h"

#include <QQuaternion>

EditCameraDialog::EditCameraDialog(PerspectiveWindow *parent) :
    QDialog(parent),
    ui(new Ui::EditCameraDialog),
    parent(parent)
{
    ui->setupUi(this);
}

EditCameraDialog::~EditCameraDialog()
{
    delete ui;
}

void EditCameraDialog::on_hSliderFOV_sliderMoved(int position)
{
    parent->setFov((float)position);
}

void EditCameraDialog::on_hSliderPosX_sliderMoved(int position){
    setCamPos(X, (float)position/50.0);
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
        parent->setCameraRot(QQuaternion::fromAxisAndAngle(0, 1, 0, 180));
    case Y:
        parent->setCameraRot(QQuaternion::fromAxisAndAngle(1, 0, 0, -90));
    case Z:
        parent->setCameraRot(QQuaternion());
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

void EditCameraDialog::on_hSliderPosY_sliderMoved(int position){
    setCamPos(Y, (float)position/50.0);
}

void EditCameraDialog::on_hSliderPosZ_sliderMoved(int position){
    setCamPos(Z, (float)position/50.0);
}

void EditCameraDialog::on_lookXAxis_clicked()
{
    QVector3D pos = makePosOnAxis(X, 5);
    setCamPos(X, pos.x());
    setCamPos(Y, pos.y());
    setCamPos(Z, pos.z());
    setCamRot(X);
}

void EditCameraDialog::on_lookYAxis_clicked()
{
    QVector3D pos = makePosOnAxis(Y, 5);
    setCamPos(X, pos.x());
    setCamPos(Y, pos.y());
    setCamPos(Z, pos.z());
    setCamRot(Y);
}

void EditCameraDialog::on_lookZAxis_clicked()
{
    QVector3D pos = makePosOnAxis(Z, 5);
    setCamPos(X, pos.x());
    setCamPos(Y, pos.y());
    setCamPos(Z, pos.z());
    setCamRot(Z);
}
