#include "editcameradialog.h"
#include "ui_editcameradialog.h"
#include "perspectivewindow.h"

EditCameraDialog::EditCameraDialog(PerspectiveWindow *parent) :
    QDialog(parent),
    ui(new Ui::EditCameraDialog)
{
    ui->setupUi(this);
}

EditCameraDialog::~EditCameraDialog()
{
    delete ui;
}

void EditCameraDialog::on_hSliderFOV_sliderMoved(int position)
{
    ((PerspectiveWindow *)this->parent())->setFov((float)position);
}

void EditCameraDialog::on_hSliderPosX_sliderMoved(int position){
    setCamPos(X, (float)position/50.0);
}

void EditCameraDialog::setCamPos(EditCameraDialog::Dim d, float amt){
    QVector3D pos = ((PerspectiveWindow *)this->parent())->getCameraPos();
    switch(d) {
    case X: pos.setX(amt); break;
    case Y: pos.setY(amt); break;
    case Z: pos.setZ(amt); break;
    }
    ((PerspectiveWindow *)this->parent())->setCameraPos(pos);
}

void EditCameraDialog::on_hSliderPosY_sliderMoved(int position){
    setCamPos(Y, (float)position/50.0);
}

void EditCameraDialog::on_hSliderPosZ_sliderMoved(int position){
    setCamPos(Z, (float)position/50.0);
}
