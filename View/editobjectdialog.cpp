#include "editobjectdialog.h"
#include "ui_editobjectdialog.h"
#include "perspectivewindow.h"
//#include "../Model/globject.h"
#include "../shared_structs.h"

#define PI 3.14159265

EditObjectDialog::EditObjectDialog(PerspectiveWindow *parent) :
    QDialog(parent),
    ui(new Ui::EditObjectDialog),
    parent(parent),
    hSliderPosScale(2.0),
    hSliderScaleScale(2.0),
    hSliderRotScale(180)
{
    ui->setupUi(this);

    connect(parent->getTvWindow()->getSelectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(setSliders()));
}

EditObjectDialog::~EditObjectDialog()
{
    delete ui;
}

void EditObjectDialog::init()
{

    ui->doubleSpinPosX->setRange(-hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE,
                                 hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderPosX->setMinimum(-hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderPosX->setMaximum(hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->doubleSpinPosY->setRange(-hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE,
                                 hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderPosY->setMinimum(-hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderPosY->setMaximum(hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->doubleSpinPosZ->setRange(-hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE,
                                 hSliderPosScale* SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderPosZ->setMinimum(-hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->hSliderPosZ->setMaximum(hSliderPosScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->doubleSpinScaleX->setRange(0, hSliderScaleScale);
    ui->hSliderScaleX->setMinimum(0);
    ui->hSliderScaleX->setMaximum(hSliderScaleScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->doubleSpinScaleY->setRange(0, hSliderScaleScale);
    ui->hSliderScaleY->setMinimum(0);
    ui->hSliderScaleY->setMaximum(hSliderScaleScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->doubleSpinScaleZ->setRange(0, hSliderScaleScale);
    ui->hSliderScaleZ->setMinimum(0);
    ui->hSliderScaleZ->setMaximum(hSliderScaleScale * SLIDER_NOTCHES_PER_UNIT_SPACE);
    ui->doubleSpinRotAngle->setRange(-180, 180);
    ui->hSliderRotAngle->setMinimum(-180);
    ui->hSliderRotAngle->setMaximum(180);
    ui->doubleSpinRotX->setRange(-180, 180);
    ui->doubleSpinRotY->setRange(-180, 180);
    ui->hSliderRotX->setMinimum(-hSliderRotScale);
    ui->hSliderRotX->setMaximum(hSliderRotScale);
    ui->hSliderRotY->setMinimum(-hSliderRotScale);
    ui->hSliderRotY->setMaximum(hSliderRotScale);
    ui->doubleSpinRotZ->setRange(-180, 180);
    ui->hSliderRotZ->setMinimum(-hSliderRotScale);
    ui->hSliderRotZ->setMaximum(hSliderRotScale);


}

void EditObjectDialog::setSliders()
{
    const GlData *selectedData = parent->getTvWindow()->getGlDataAtSelection();
    if (!selectedData) {
        qDebug() << "EditObjectDialog::init(): No data at selection!!";
        return;
    }

    const QVector3D &translation = selectedData->translation; //geometryEngine->getTranslation();
    ui->hSliderPosX->setValue((int)translation.x());
    ui->doubleSpinPosX->setValue(translation.x());

    ui->hSliderPosY->setValue((int)translation.y());
    ui->doubleSpinPosY->setValue(translation.y());

    ui->hSliderPosZ->setValue((int)translation.z());
    ui->doubleSpinPosZ->setValue(translation.z());

    const QVector3D &scale = selectedData->scale; //geometryEngine->getScale();
    ui->doubleSpinScaleX->setValue(scale.x());
    ui->hSliderScaleX->setValue((int)scale.x()* SLIDER_NOTCHES_PER_UNIT_SPACE);

    ui->doubleSpinScaleY->setValue(scale.y());
    ui->hSliderScaleY->setValue((int)scale.y()* SLIDER_NOTCHES_PER_UNIT_SPACE);

    ui->doubleSpinScaleZ->setValue(scale.z());
    ui->hSliderScaleZ->setValue((int)scale.z()* SLIDER_NOTCHES_PER_UNIT_SPACE);

    const QQuaternion &quat = selectedData->rotation;
    QVector4D rot4 = quat.toVector4D();
    float theta = rot4.w();
    QVector3D rotAngle = rot4.toVector3D();  //geometryEngine->getRotation(theta);
    ui->doubleSpinRotAngle->setValue(theta);
    ui->hSliderRotAngle->setValue(theta/PI * 180);

    ui->doubleSpinRotX->setValue(rotAngle.x()* 180.0 / PI);
    ui->hSliderRotX->setValue((int)(rotAngle.x()* 180.0 / PI));

    ui->doubleSpinRotY->setValue(rotAngle.y()* 180.0 / PI);
    ui->hSliderRotY->setValue((int)(rotAngle.y()* 180.0 / PI));

    ui->doubleSpinRotZ->setValue(rotAngle.z()* 180.0 / PI);
    ui->hSliderRotZ->setValue((int)(rotAngle.z()* 180.0 / PI));

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
//    updateConstraintsPos();

    // should we just pull this data from the spin boxes? could be a lot faster
    const QVector3D *translationp = getSelectedTranslation(); // selectedData->scale;

    if (!translationp) {
        return;
    }
    QVector3D translation = *translationp;

    switch(ax) {
    case X:
        setUiControls(PosX, amt, &translation);
//        if (constraintPos.x && constraintPos.y) {
//            set(PosY, amt, &translation);
//        }
//        if (constraintPos.x && constraintPos.z) {
//            set(PosZ, amt, &translation);
//        }
//        translation.setX(amt);
//        ui->doubleSpinPosX->setValue(amt);
//        ui->hSliderPosX->setValue(amt*SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case Y:
        setUiControls(PosY, amt, &translation);
//        if (constraintPos.y && constraintPos.x) {
//            set(PosX, amt, &translation);
//        }
//        if (constraintPos.y && constraintPos.z) {
//            set(PosZ, amt, &translation);
//        }
//        translation.setY(amt);
//        ui->doubleSpinPosY->setValue(amt);
//        ui->hSliderPosY->setValue(amt*SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case Z:
        setUiControls(PosZ, amt, &translation);
//        if (constraintPos.z && constraintPos.x) {
//            set(PosX, amt, &translation);
//        }
//        if (constraintPos.z && constraintPos.y) {
//            set(PosY, amt, &translation);
//        }
//        translation.setZ(amt);
//        ui->doubleSpinPosZ->setValue(amt);
//        ui->hSliderPosZ->setValue(amt*SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    }
    parent->getTvWindow()->setTranslationAtSel(translation);
    //geometryEngine->setTranslation(translation);
    //((PerspectiveWindow*)parent)->updateChildren();
}

void EditObjectDialog::setScale(EditObjectDialog::Axis ax, double amt)
{
//    updateConstraintsScale();

    // should we just pull this data from the spin boxes? could be a lot faster
    const QVector3D *scalep = getSelectedScale(); // selectedData->scale;
    if (!scalep) {
        return;
    }
    QVector3D scale = *scalep;

    //QVector3D scale = geometryEngine->getScale();
    switch(ax) {
    case X:
        setUiControls(ScaleX, amt, &scale);
//        if (constraintScale.x && constraintScale.y) {
//            set(ScaleY, amt, &scale);
//        }
//        if (constraintScale.x && constraintScale.z) {
//            set(ScaleZ, amt, &scale);
//        }
        break;
    case Y:
        setUiControls(ScaleY, amt, &scale);
//        if (constraintScale.y && constraintScale.x) {
//            set(ScaleX, amt, &scale);
//        }
//        if (constraintScale.y && constraintScale.z) {
//            set(ScaleZ, amt, &scale);
//        }
        break;
    case Z:
        setUiControls(ScaleZ, amt, &scale);
//        if (constraintScale.z && constraintScale.x) {
//            set(ScaleX, amt, &scale);
//        }
//        if (constraintScale.z && constraintScale.y) {
//            set(ScaleY, amt, &scale);
//        }
        break;
    }
    parent->getTvWindow()->setScaleAtSel(scale);
    //geometryEngine->setScale(scale);
    //((PerspectiveWindow*)parent)->updateChildren();
}

void EditObjectDialog::setRotation(EditObjectDialog::Axis ax, double amt)
{
//    GlData *selectedData = parent->getTvWindow()->getGlDataAtSelection();
//    if (!selectedData) {
//        qDebug() << "setScale failed: No data at selection!!";
//        return;
//    }
//    const QQuaternion &quat = selectedData->rotation;
//    QVector4D rot4 = quat.toVector4D();
//    float theta = rot4.w();
//    QVector3D rotAxis = rot4.toVector3D();  //geometryEngine->getRotation(theta);

    float theta;
    QVector3D rotAxis = getSelectedRotation(theta);
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
    parent->getTvWindow()->setRotationAtSel(rotAxis, theta);
    //geometryEngine->setRotation(rotAxis, theta);
    //((PerspectiveWindow*)parent)->updateChildren();
}

void EditObjectDialog::setTranslation(EditObjectDialog::Axis dest,
                                      EditObjectDialog::Axis src)
{
    QVector3D pos = *getSelectedTranslation();
    double amt = (src == X) ? pos.x() :
                 (src == Y) ? pos.y() : pos.z();
    switch(dest) {
    case X:
        setUiControls(PosX, amt, &pos);
        break;
    case Y:
        setUiControls(PosY, amt, &pos);
        break;
    case Z:
        setUiControls(PosZ, amt, &pos);
        break;
    }
    parent->getTvWindow()->setTranslationAtSel(pos);
}

void EditObjectDialog::setScale(EditObjectDialog::Axis dest, EditObjectDialog::Axis src)
{
    QVector3D scale = *getSelectedScale();
    double amt = (src == X) ? scale.x() :
                 (src == Y) ? scale.y() : scale.z();
    switch(dest) {
    case X:
        setUiControls(ScaleX, amt, &scale);
        break;
    case Y:
        setUiControls(ScaleY, amt, &scale);
        break;
    case Z:
        setUiControls(ScaleZ, amt, &scale);
        break;
    }
    parent->getTvWindow()->setScaleAtSel(scale);
}

const QVector3D *EditObjectDialog::getSelectedTranslation() const
{
    const GlData *selectedData = parent->getTvWindow()->getGlDataAtSelection();
    if (!selectedData) {
        qDebug() << "getSelectedTranslation failed: No data at selection!!";
        return NULL;
    }
//    qDebug() << "Got GlData=" << selectedData->toString();
    return &selectedData->translation;
}

const QVector3D *EditObjectDialog::getSelectedScale() const
{
    const GlData *selectedData = parent->getTvWindow()->getGlDataAtSelection();
    if (!selectedData) {
        qDebug() << "getSelectedScale failed: No data at selection!!";
        return NULL;
    }
    return &selectedData->scale;
}

QVector3D EditObjectDialog::getSelectedRotation(float &theta) const {
    const GlData *selectedData = parent->getTvWindow()->getGlDataAtSelection();
    if (!selectedData) {
        qDebug() << "getSelectedRotation failed: No data at selection!!";
        theta = -1;
        return QVector3D();
    }
//    qDebug() << "Selected data is: " << selectedData->toString();

    const QQuaternion *quat = &selectedData->rotation;
    if (quat) {
        QVector4D rot4 = quat->toVector4D();
        theta = rot4.w();
        return rot4.toVector3D();
    } else {
        qDebug() << "There's no quaternion there.";
        return QVector3D();
    }
}

//void EditObjectDialog::updateConstraintsPos() {
//    constraintPos.x = ui->ckConstrainPosX->isChecked();
//    constraintPos.y = ui->ckConstrainPosY->isChecked();
//    constraintPos.z = ui->ckConstrainPosZ->isChecked();
//}

//void EditObjectDialog::updateConstraintsScale() {
//    constraintPos.x = ui->ckConstrainScaleX->isChecked();
//    constraintPos.y = ui->ckConstrainScaleY->isChecked();
//    constraintPos.z = ui->ckConstrainScaleZ->isChecked();
//}

void EditObjectDialog::setUiControls(EditObjectDialog::EditVals v, double amt,
                                     QVector3D *vec)
{
    //ScaleX, ScaleY, ScaleZ, PosX, PosY, PosZ, RotX, RotY, RotZ, RotTheta
    switch(v) {
    case ScaleX:
        vec->setX(amt);
        ui->doubleSpinScaleX->setValue(amt);
        ui->hSliderScaleX->setValue(amt * SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case ScaleY:
        vec->setY(amt);
        ui->doubleSpinScaleY->setValue(amt);
        ui->hSliderScaleY->setValue(amt * SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case ScaleZ:
        vec->setZ(amt);
        ui->doubleSpinScaleZ->setValue(amt);
        ui->hSliderScaleZ->setValue(amt * SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case PosX:
        vec->setX(amt);
        ui->doubleSpinPosX->setValue(amt);
        ui->hSliderPosX->setValue(amt*SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case PosY:
        vec->setY(amt);
        ui->doubleSpinPosY->setValue(amt);
        ui->hSliderPosY->setValue(amt*SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case PosZ:
        vec->setZ(amt);
        ui->doubleSpinPosZ->setValue(amt);
        ui->hSliderPosZ->setValue(amt*SLIDER_NOTCHES_PER_UNIT_SPACE);
        break;
    case RotX:
        break;
    case RotY:
        break;
    case RotZ:
        break;
    case RotTheta:
        break;
    }
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
    QVector3D rotAxis = getSelectedRotation(theta); //geometryEngine->getRotation(theta);
    parent->getTvWindow()->setRotationAtSel(rotAxis, (float)position);

    //geometryEngine->setRotation(rotAxis, (float)position);
    ui->doubleSpinRotAngle->setValue(position);
    //((PerspectiveWindow*)parent)->updateChildren();
}

void EditObjectDialog::on_doubleSpinRotAngle_valueChanged(double arg1)
{
    float theta;
    QVector3D rotAxis = getSelectedRotation(theta); //geometryEngine->getRotation(theta);
    parent->getTvWindow()->setRotationAtSel(rotAxis, (float)arg1);
    //geometryEngine->setRotation(rotAxis, (float)arg1);
    ui->hSliderRotAngle->setValue(arg1);
    //((PerspectiveWindow*)parent)->updateChildren();
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

void EditObjectDialog::on_setposXY_clicked() {
    setTranslation(X,Y);
}

void EditObjectDialog::on_setposXZ_clicked() {
    setTranslation(X,Z);
}

void EditObjectDialog::on_setposYX_clicked() {
    setTranslation(Y,X);
}

void EditObjectDialog::on_setposYZ_clicked() {
    setTranslation(Y,Z);
}

void EditObjectDialog::on_setposZX_clicked() {
    setTranslation(Z,X);
}

void EditObjectDialog::on_setposZY_clicked() {
    setTranslation(Z,Y);
}



void EditObjectDialog::on_setscaleXY_clicked() {
    setScale(X,Y);
}

void EditObjectDialog::on_setscaleXZ_clicked() {
    setScale(X,Z);
}

void EditObjectDialog::on_setscaleYX_clicked() {
    setScale(Y,X);
}

void EditObjectDialog::on_setscaleYZ_clicked() {
    setScale(Y,Z);
}

void EditObjectDialog::on_setscaleZX_clicked() {
    setScale(Z,X);
}

void EditObjectDialog::on_setscaleZY_clicked() {
    setScale(Z,Y);
}
