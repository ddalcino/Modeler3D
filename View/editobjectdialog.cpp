#include "editobjectdialog.h"
#include "ui_editobjectdialog.h"
#include "perspectivewindow.h"

#define PI 3.14159265

EditObjectDialog::EditObjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditObjectDialog),
    parent(parent),
    hSliderPosScale(2.0),
    hSliderScaleScale(2.0),
    hSliderRotScale(180)
{
    ui->setupUi(this);
}

EditObjectDialog::~EditObjectDialog()
{
    delete ui;
}

void EditObjectDialog::init(GeometryEngine *e)
{
    if (e == NULL) {
        qDebug() << "No geometry yet!!";
    }
    geometryEngine=e;
    QVector3D translation = geometryEngine->getTranslation();
    ui->hSliderPosX->setValue((int)translation.x());
    ui->doubleSpinPosX->setRange(-hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE,
                                 hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->doubleSpinPosX->setValue(translation.x());
    ui->hSliderPosX->setMinimum(-hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderPosX->setMaximum(hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);

    ui->hSliderPosY->setValue((int)translation.y());
    ui->doubleSpinPosY->setRange(-hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE,
                                 hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->doubleSpinPosY->setValue(translation.y());
    ui->hSliderPosY->setMinimum(-hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderPosY->setMaximum(hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);

    ui->hSliderPosZ->setValue((int)translation.z());
    ui->doubleSpinPosZ->setRange(-hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE,
                                 hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->doubleSpinPosZ->setValue(translation.z());
    ui->hSliderPosZ->setMinimum(-hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderPosZ->setMaximum(hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);

    QVector3D scale = geometryEngine->getScale();
    ui->doubleSpinScaleX->setRange(0, hSliderScaleScale);
    ui->doubleSpinScaleX->setValue(scale.x());
    ui->hSliderScaleX->setValue((int)scale.x()* SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderScaleX->setMinimum(0);
    ui->hSliderScaleX->setMaximum(hSliderScaleScale * SLIDER_NOTCHES_PER_UNIT_SPACE);

    ui->doubleSpinScaleY->setRange(0, hSliderScaleScale);
    ui->doubleSpinScaleY->setValue(scale.y());
    ui->hSliderScaleY->setValue((int)scale.y()* SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderScaleY->setMinimum(0);
    ui->hSliderScaleY->setMaximum(hSliderScaleScale * SLIDER_NOTCHES_PER_UNIT_SPACE);

    ui->doubleSpinScaleZ->setRange(0, hSliderScaleScale);
    ui->doubleSpinScaleZ->setValue(scale.z());
    ui->hSliderScaleZ->setValue((int)scale.z()* SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderScaleZ->setMinimum(0);
    ui->hSliderScaleZ->setMaximum(hSliderScaleScale * SLIDER_NOTCHES_PER_UNIT_SPACE);

    float theta;
    QVector3D rotAngle = geometryEngine->getRotation(theta);
    ui->doubleSpinRotAngle->setRange(-180, 180);
    ui->doubleSpinRotAngle->setValue(theta);
    ui->hSliderRotAngle->setValue(theta/PI * 180);
    ui->hSliderRotAngle->setMinimum(-180);
    ui->hSliderRotAngle->setMaximum(180);

    ui->doubleSpinRotX->setRange(-180, 180);
    ui->doubleSpinRotX->setValue(rotAngle.x()* 180.0 / PI);
    ui->hSliderRotX->setValue((int)(rotAngle.x()* 180.0 / PI));
    ui->hSliderRotX->setMinimum(-hSliderRotScale);
    ui->hSliderRotX->setMaximum(hSliderRotScale);

    ui->doubleSpinRotY->setRange(-180, 180);
    ui->doubleSpinRotY->setValue(rotAngle.y()* 180.0 / PI);
    ui->hSliderRotY->setValue((int)(rotAngle.y()* 180.0 / PI));
    ui->hSliderRotY->setMinimum(-hSliderRotScale);
    ui->hSliderRotY->setMaximum(hSliderRotScale);

    ui->doubleSpinRotZ->setRange(-180, 180);
    ui->doubleSpinRotZ->setValue(rotAngle.z()* 180.0 / PI);
    ui->hSliderRotZ->setValue((int)(rotAngle.z()* 180.0 / PI));
    ui->hSliderRotZ->setMinimum(-hSliderRotScale);
    ui->hSliderRotZ->setMaximum(hSliderRotScale);
}

void EditObjectDialog::on_hSliderPosX_sliderMoved(int position)
{
    setTranslation(X, (double)position/SLIDER_NOTCHES_PER_UNIT_SPACE);
}

void EditObjectDialog::on_hSliderPosY_sliderMoved(int position)
{
    setTranslation(Y, (double)position/SLIDER_NOTCHES_PER_UNIT_SPACE);
}

void EditObjectDialog::on_hSliderPosZ_sliderMoved(int position)
{
    setTranslation(Z, (double)position/SLIDER_NOTCHES_PER_UNIT_SPACE);
}

void EditObjectDialog::setTranslation(EditObjectDialog::Axis ax, double amt)
{
    QVector3D translation = geometryEngine->getTranslation();
    switch(ax) {
    case X:
        translation.setX(amt);
        ui->doubleSpinPosX->setValue(amt);
        ui->hSliderPosX->setValue(amt*SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case Y:
        translation.setY(amt);
        ui->doubleSpinPosY->setValue(amt);
        ui->hSliderPosY->setValue(amt*SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case Z:
        translation.setZ(amt);
        ui->doubleSpinPosZ->setValue(amt);
        ui->hSliderPosZ->setValue(amt*SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    }
    geometryEngine->setTranslation(translation);
    ((PerspectiveWindow*)parent)->updateChildren();
}

void EditObjectDialog::setScale(EditObjectDialog::Axis ax, double amt)
{
    QVector3D scale = geometryEngine->getScale();
    switch(ax) {
    case X:
        scale.setX(amt);
        ui->doubleSpinScaleX->setValue(amt);
        ui->hSliderScaleX->setValue(amt * SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case Y:
        scale.setY(amt);
        ui->doubleSpinScaleY->setValue(amt);
        ui->hSliderScaleY->setValue(amt * SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case Z:
        scale.setZ(amt);
        ui->doubleSpinScaleZ->setValue(amt);
        ui->hSliderScaleZ->setValue(amt * SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    }
    geometryEngine->setScale(scale);
    ((PerspectiveWindow*)parent)->updateChildren();
}

void EditObjectDialog::setRotation(EditObjectDialog::Axis ax, double amt)
{
    float theta;
    QVector3D rotAxis = geometryEngine->getRotation(theta);
    switch(ax) {
    case X:
        rotAxis.setX(amt);
        ui->doubleSpinRotX->setValue(amt/SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case Y:
        rotAxis.setY(amt);
        ui->doubleSpinRotY->setValue(amt/SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case Z:
        rotAxis.setZ(amt);
        ui->doubleSpinRotZ->setValue(amt/SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    }
    geometryEngine->setRotation(rotAxis, theta);
    ((PerspectiveWindow*)parent)->updateChildren();
}

void EditObjectDialog::on_hSliderScaleX_sliderMoved(int position)
{
    setScale(X, (double)position/SLIDER_NOTCHES_PER_UNIT_SPACE);
}

void EditObjectDialog::on_hSliderScaleY_sliderMoved(int position)
{
    setScale(Y, (double)position/SLIDER_NOTCHES_PER_UNIT_SPACE);
}

void EditObjectDialog::on_hSliderScaleZ_sliderMoved(int position)
{
    setScale(Z, (double)position/SLIDER_NOTCHES_PER_UNIT_SPACE);
}

void EditObjectDialog::on_hSliderRotX_sliderMoved(int position)
{
    setRotation(X, position);
}

void EditObjectDialog::on_hSliderRotY_sliderMoved(int position)
{
    setRotation(Y, position);
}

void EditObjectDialog::on_hSliderRotZ_sliderMoved(int position)
{
    setRotation(Z, position);
}

void EditObjectDialog::on_hSliderRotAngle_sliderMoved(int position)
{
    float theta;
    QVector3D rotAxis = geometryEngine->getRotation(theta);
    geometryEngine->setRotation(rotAxis, (float)position);
    ui->doubleSpinRotAngle->setValue(position);
    ((PerspectiveWindow*)parent)->updateChildren();
}

void EditObjectDialog::on_doubleSpinRotAngle_valueChanged(double arg1)
{
    float theta;
    QVector3D rotAxis = geometryEngine->getRotation(theta);
    geometryEngine->setRotation(rotAxis, (float)arg1);
    ui->hSliderRotAngle->setValue(arg1);
    ((PerspectiveWindow*)parent)->updateChildren();
}

void EditObjectDialog::on_doubleSpinPosX_valueChanged(double arg1)
{
    setTranslation(X, arg1);
}
void EditObjectDialog::on_doubleSpinPosY_valueChanged(double arg1)
{
    setTranslation(Y, arg1);
}
void EditObjectDialog::on_doubleSpinPosZ_valueChanged(double arg1)
{
    setTranslation(Z, arg1);
}

void EditObjectDialog::on_doubleSpinScaleX_valueChanged(double arg1)
{
    setScale(X, arg1);
}
void EditObjectDialog::on_doubleSpinScaleY_valueChanged(double arg1)
{
    setScale(Y, arg1);
}
void EditObjectDialog::on_doubleSpinScaleZ_valueChanged(double arg1)
{
    setScale(Z, arg1);
}

void EditObjectDialog::on_doubleSpinRotX_valueChanged(double arg1)
{
    setRotation(X, arg1);
}
void EditObjectDialog::on_doubleSpinRotY_valueChanged(double arg1)
{
    setRotation(Y, arg1);
}
void EditObjectDialog::on_doubleSpinRotZ_valueChanged(double arg1)
{
    setRotation(Z, arg1);
}
