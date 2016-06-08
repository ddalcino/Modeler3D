#include "doubleinputform.h"
#include "ui_doubleinputform.h"

#include <QString>

#define MAX_D 100000

DoubleInputForm::DoubleInputForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DoubleInputForm)
{
    ui->setupUi(this);
}

DoubleInputForm::~DoubleInputForm()
{
    delete ui;
}

void DoubleInputForm::init(const char *name, double hardMax, double hardMin,
                           double initMin, double initMax, double initValue,
                           bool isFixedMin, bool isFixedMax,
                           unsigned int granularity, double stepSize) {
    if (hardMax < hardMin || initMax < initMin) {
        throw "Invalid ranges given";
    }
    this->isFixedMax = isFixedMax;
    this->isFixedMin = isFixedMin;
    this->hardMax = hardMax;
    this->hardMin = hardMin;
    this->granularity = granularity;
    realValue = initValue;
    ui->doubleSpinBoxValue->setMinimum(0);
    ui->doubleSpinBoxValue->setMaximum(granularity);
    ui->doubleSpinBoxValue->setSingleStep(stepSize);
    if (isFixedMax) {
        ui->doubleSpinBoxMax->setValue(hardMax);
        ui->doubleSpinBoxMax->setMaximum(hardMax);
        ui->doubleSpinBoxMin->setMaximum(hardMax);
        ui->doubleSpinBoxMax->setEnabled(false);
        ui->doubleSpinBoxValue->setMaximum(hardMax);
        max = hardMax;
    } else {
        ui->doubleSpinBoxMax->setValue(initMax);
        ui->doubleSpinBoxMax->setMaximum(MAX_D);
        ui->doubleSpinBoxMin->setMaximum(MAX_D);
        ui->doubleSpinBoxMax->setEnabled(true);
        ui->doubleSpinBoxValue->setMaximum(MAX_D);
        max = initMax;
    }
    if (isFixedMin) {
        ui->doubleSpinBoxMin->setValue(hardMin);
        ui->doubleSpinBoxMin->setMinimum(hardMin);
        ui->doubleSpinBoxMax->setMinimum(hardMin);
        ui->doubleSpinBoxMin->setEnabled(false);
        ui->doubleSpinBoxValue->setMinimum(hardMin);
        min = hardMin;
    } else {
        ui->doubleSpinBoxMin->setValue(initMin);
        ui->doubleSpinBoxMin->setMinimum(-MAX_D);
        ui->doubleSpinBoxMax->setMinimum(-MAX_D);
        ui->doubleSpinBoxMin->setEnabled(true);
        ui->doubleSpinBoxValue->setMinimum(-MAX_D);
        min = initMin;
    }
    //ui->doubleSpinBoxValue->setValue(realValue);
    ui->labelName->setText(QString(name));
}

void DoubleInputForm::setValue(double newVal, ControlType controlType) {
    int translatedValue = ((newVal - min) * granularity / (max - min));
    if (translatedValue > granularity && !setMax(newVal)) {
        // value is out of range; fail to set value and set back to old value
        if (controlType == Spinner || controlType == Both) {
            ui->doubleSpinBoxValue->setValue(realValue);
        }
        return;
    } else if (translatedValue < 0 && !setMin(newVal)) {
        // value is out of range; fail to set value and set back to old value
        if (controlType == Spinner || controlType == Both) {
            ui->doubleSpinBoxValue->setValue(realValue);
        }
        return;
    }
    // it's ok to set the value
    if (controlType == Spinner || controlType == Both) {
        ui->doubleSpinBoxValue->setValue(newVal);
    }
    if (controlType == Slider || controlType == Both) {
        // recalculate translated value, in case the range has been updated
        int translatedValue = ((newVal - min) * granularity / (max - min));
        ui->horizontalSlider->setValue(translatedValue);
    }
    realValue = newVal;
}

bool DoubleInputForm::setMax(double newMax, bool fixSlider) {
    if (newMax < hardMax && !isFixedMax && newMax > min) {
        ui->doubleSpinBoxMax->setValue(newMax);
        max = newMax;
        if (fixSlider) {
            setValue(realValue, Slider);
        }
        return true;
    } else {
        ui->doubleSpinBoxMax->setValue(max);
        if (fixSlider) {
            setValue(realValue, Slider);
        }
        return false;
    }
}

bool DoubleInputForm::setMin(double newMin, bool fixSlider) {
    if (newMin > hardMin && !isFixedMin && newMin < max) {
        ui->doubleSpinBoxMin->setValue(newMin);
        min = newMin;
        if (fixSlider) {
            setValue(realValue, Slider);
        }
        return true;
    } else {
        ui->doubleSpinBoxMin->setValue(min);
        if (fixSlider) {
            setValue(realValue, Slider);
        }
        return false;
    }
}

double DoubleInputForm::getVal() const {
    return realValue;
}

void DoubleInputForm::on_doubleSpinBoxValue_editingFinished() {
    setValue(ui->doubleSpinBoxValue->value(), Slider);
    emit changed_value(realValue);
}

void DoubleInputForm::on_doubleSpinBoxMin_editingFinished() {
    setMin(ui->doubleSpinBoxMin->value(), true);
}

void DoubleInputForm::on_doubleSpinBoxMax_editingFinished() {
    setMax(ui->doubleSpinBoxMax->value(), true);
}

void DoubleInputForm::on_horizontalSlider_sliderMoved(int position) {
    double translatedValue = ((double)position * (max - min)) / granularity + min;
    setValue(translatedValue, Spinner);
    emit changed_value(realValue);
}

void DoubleInputForm::on_doubleSpinBoxValue_valueChanged(double arg1) {
    setValue(arg1, Slider);
    emit changed_value(realValue);
}

void DoubleInputForm::on_doubleSpinBoxMin_valueChanged(double arg1) {
    setMin(arg1, true);

}

void DoubleInputForm::on_doubleSpinBoxMax_valueChanged(double arg1) {
    setMax(arg1, true);
}
