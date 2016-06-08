#include "editobjectdialog2.h"
#include "ui_editobjectdialog2.h"
#include "Controller/treeviewwindow.h"
#include "doubleinputform.h"
#include "global_structs.h"

#include <QVector3D>
#include <QQuaternion>


EditObjectDialog2::EditObjectDialog2(QWidget *parent, TreeViewWindow *tvWin) :
    QDialog(parent),
    ui(new Ui::EditObjectDialog2),
    tvWindow(tvWin)
{
    ui->setupUi(this);

    static const double MAX = 10000.0;

    // Setup DoubleInputForms
    ui->posXSlider->init("Pos X", MAX, -MAX, -2, 2, 0, false, false);
    ui->posYSlider->init("Pos Y", MAX, -MAX, -2, 2, 0, false, false);
    ui->posZSlider->init("Pos Z", MAX, -MAX, -2, 2, 0, false, false);
    ui->scaleXSlider->init("Scale X", MAX, 0, 0, 2, 1, true, false);
    ui->scaleYSlider->init("Scale Y", MAX, 0, 0, 2, 1, true, false);
    ui->scaleZSlider->init("Scale Z", MAX, 0, 0, 2, 1, true, false);

    connect(ui->posXSlider, SIGNAL(changed_value(double)),
            this, SLOT(updateTranslation()));
    connect(ui->posYSlider, SIGNAL(changed_value(double)),
            this, SLOT(updateTranslation()));
    connect(ui->posZSlider, SIGNAL(changed_value(double)),
            this, SLOT(updateTranslation()));
    connect(ui->scaleXSlider, SIGNAL(changed_value(double)),
            this, SLOT(updateScale()));
    connect(ui->scaleYSlider, SIGNAL(changed_value(double)),
            this, SLOT(updateScale()));
    connect(ui->scaleZSlider, SIGNAL(changed_value(double)),
            this, SLOT(updateScale()));
    connect(tvWindow, SIGNAL(selectionChanged()), this, SLOT(init()));
}

EditObjectDialog2::~EditObjectDialog2()
{
    delete ui;
}

void EditObjectDialog2::init() {
    try {
        // get selected GlData from tvWindow
        const GlData inData = tvWindow->getGlDataAtSelection();
        //if (inData) {
        ui->posXSlider->setValue(inData.translation.x(), DoubleInputForm::Both);
        ui->posYSlider->setValue(inData.translation.y(), DoubleInputForm::Both);
        ui->posZSlider->setValue(inData.translation.z(), DoubleInputForm::Both);
        ui->scaleXSlider->setValue(inData.scale.x(), DoubleInputForm::Both);
        ui->scaleYSlider->setValue(inData.scale.y(), DoubleInputForm::Both);
        ui->scaleZSlider->setValue(inData.scale.z(), DoubleInputForm::Both);
        update();
    } catch (const char *msg) {
        qDebug() << "Error: " << msg;
    }
}

void EditObjectDialog2::updateTranslation()
{
    QVector3D translation(ui->posXSlider->getVal(), ui->posYSlider->getVal(),
                          ui->posZSlider->getVal());
    tvWindow->setTranslationAtSel(translation);
}

void EditObjectDialog2::updateScale()
{
    QVector3D scale(ui->scaleXSlider->getVal(), ui->scaleYSlider->getVal(),
                          ui->scaleZSlider->getVal());
    tvWindow->setScaleAtSel(scale);
}

void EditObjectDialog2::updateRotation(EditObjectDialog2::Axis axis,
                                       bool isNegative)
{
    try {
        // get selected GlData from tvWindow
        const GlData inData = tvWindow->getGlDataAtSelection();
        //if (inData) {
        float theta = (isNegative ? -1 : 1) *
                ((axis == X) ? ui->doubleSpinBoxRotX->value() :
                 (axis == Y) ? ui->doubleSpinBoxRotY->value() :
                 ui->doubleSpinBoxRotZ->value());

        QQuaternion quat = QQuaternion::fromAxisAndAngle((axis == X) ? 1 : 0,
                                                         (axis == Y) ? 1 : 0,
                                                         (axis == Z) ? 1 : 0,
                                                         theta);

        tvWindow->setRotationAtSel((quat * inData.rotation).normalized());
    } catch (const char *msg) {
        qDebug() << "Error: " << msg;
    }
}

void EditObjectDialog2::on_setPosXY_clicked() {
    ui->posXSlider->setValue(ui->posYSlider->getVal(), DoubleInputForm::Both);
    updateTranslation();
}

void EditObjectDialog2::on_setPosXZ_clicked() {
    ui->posXSlider->setValue(ui->posZSlider->getVal(), DoubleInputForm::Both);
    updateTranslation();
}


void EditObjectDialog2::on_setPosYX_clicked() {
    ui->posYSlider->setValue(ui->posXSlider->getVal(), DoubleInputForm::Both);
    updateTranslation();
}


void EditObjectDialog2::on_setPosYZ_clicked() {
    ui->posYSlider->setValue(ui->posZSlider->getVal(), DoubleInputForm::Both);
    updateTranslation();
}


void EditObjectDialog2::on_setPosZX_clicked() {
    ui->posZSlider->setValue(ui->posXSlider->getVal(), DoubleInputForm::Both);
    updateTranslation();
}


void EditObjectDialog2::on_setPosZY_clicked() {
    ui->posZSlider->setValue(ui->posYSlider->getVal(), DoubleInputForm::Both);
    updateTranslation();
}


void EditObjectDialog2::on_setScaleXY_clicked() {
    ui->scaleXSlider->setValue(ui->scaleYSlider->getVal(), DoubleInputForm::Both);
    updateScale();
}


void EditObjectDialog2::on_setScaleXZ_clicked() {
    ui->scaleXSlider->setValue(ui->scaleZSlider->getVal(), DoubleInputForm::Both);
    updateScale();
}


void EditObjectDialog2::on_setScaleYX_clicked() {
    ui->scaleYSlider->setValue(ui->scaleXSlider->getVal(), DoubleInputForm::Both);
    updateScale();
}


void EditObjectDialog2::on_setScaleYZ_clicked() {
    ui->scaleYSlider->setValue(ui->scaleZSlider->getVal(), DoubleInputForm::Both);
    updateScale();
}


void EditObjectDialog2::on_setScaleZX_clicked() {
    ui->scaleZSlider->setValue(ui->scaleXSlider->getVal(), DoubleInputForm::Both);
    updateScale();
}

void EditObjectDialog2::on_setScaleZY_clicked() {
    ui->scaleZSlider->setValue(ui->scaleYSlider->getVal(), DoubleInputForm::Both);
    updateScale();
}

void EditObjectDialog2::on_rotateX_clicked() {
    updateRotation(X, false);
}

void EditObjectDialog2::on_rotNegX_clicked() {
    updateRotation(X, true);
}

void EditObjectDialog2::on_rotateY_clicked() {
    updateRotation(Y, false);
}

void EditObjectDialog2::on_rotNegY_clicked() {
    updateRotation(Y, true);
}

void EditObjectDialog2::on_rotateZ_clicked() {
    updateRotation(Z, false);
}

void EditObjectDialog2::on_rotNegZ_clicked() {
    updateRotation(Z, true);
}

