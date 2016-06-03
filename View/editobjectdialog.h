#ifndef EDITOBJECTDIALOG_H
#define EDITOBJECTDIALOG_H

#include <QDialog>

#include "geometryengine.h"

class PerspectiveWindow;

namespace Ui {
class EditObjectDialog;
}

class EditObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditObjectDialog(PerspectiveWindow *parent = 0);
    ~EditObjectDialog();

    void init();

public slots:
    void setSliders();

private slots:
    void on_hSliderPosX_sliderMoved(int position);
    void on_hSliderPosY_sliderMoved(int position);
    void on_hSliderPosZ_sliderMoved(int position);

    void on_hSliderScaleX_sliderMoved(int position);
    void on_hSliderScaleY_sliderMoved(int position);
    void on_hSliderScaleZ_sliderMoved(int position);

    void on_hSliderRotX_sliderMoved(int position);
    void on_hSliderRotY_sliderMoved(int position);
    void on_hSliderRotZ_sliderMoved(int position);
    void on_hSliderRotAngle_sliderMoved(int position);

    void on_doubleSpinPosX_valueChanged(double arg1);
    void on_doubleSpinPosY_valueChanged(double arg1);
    void on_doubleSpinPosZ_valueChanged(double arg1);

    void on_doubleSpinScaleX_valueChanged(double arg1);
    void on_doubleSpinScaleY_valueChanged(double arg1);
    void on_doubleSpinScaleZ_valueChanged(double arg1);

    void on_doubleSpinRotX_valueChanged(double arg1);
    void on_doubleSpinRotY_valueChanged(double arg1);
    void on_doubleSpinRotZ_valueChanged(double arg1);
    void on_doubleSpinRotAngle_valueChanged(double arg1);

private:
    Ui::EditObjectDialog *ui;
    //GeometryEngine *geometryEngine;
    PerspectiveWindow *parent;

    static const float constexpr SLIDER_NOTCHES_PER_UNIT_SPACE = 100.0;

    float hSliderPosScale, hSliderScaleScale, hSliderRotScale;

    struct Constraints {
        bool x, y, z;
        Constraints() : x(false), y(false), z(false) {}
    };
    Constraints constraintPos, constraintScale;

    enum Axis {X, Y, Z};
    void setTranslation(Axis ax, double amt);
    void setScale(Axis ax, double amt);
    void setRotation(Axis ax, double amt);

    const QVector3D *getSelectedTranslation() const;
    const QVector3D *getSelectedScale() const;
    QVector3D getSelectedRotation(float &theta) const;

    void updateConstraintsPos();
    void updateConstraintsScale();

    //void setRotationAngle(double amt);
};

#endif // EDITOBJECTDIALOG_H
