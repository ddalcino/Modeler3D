#ifndef DOUBLEINPUTFORM_H
#define DOUBLEINPUTFORM_H

#include <QWidget>

namespace Ui {
class DoubleInputForm;
}

class DoubleInputForm : public QWidget
{
    Q_OBJECT

public:
    enum ControlType {Spinner, Slider, Both};

    explicit DoubleInputForm(QWidget *parent = 0);
    ~DoubleInputForm();
    void init(const char* name, double hardMax, double hardMin,
              double initMin, double initMax, double initValue,
              bool isFixedMin, bool isFixedMax, unsigned int granularity=200,
              double stepSize=0.1);
    void setValue(double newVal, ControlType controlType);
    bool setMax(double newMax, bool fixSlider=false);
    bool setMin(double newMin, bool fixSlider=false);
    double getVal() const;

signals:
    void changed_value();

private slots:
    void on_doubleSpinBoxValue_editingFinished();

    void on_doubleSpinBoxMin_editingFinished();

    void on_doubleSpinBoxMax_editingFinished();

    void on_horizontalSlider_sliderMoved(int position);

    void on_doubleSpinBoxValue_valueChanged(double arg1);

    void on_doubleSpinBoxMin_valueChanged(double arg1);

    void on_doubleSpinBoxMax_valueChanged(double arg1);

private:
    Ui::DoubleInputForm *ui;

    double hardMax, hardMin;
    bool isFixedMin, isFixedMax;
    double min, max;
    double realValue, granularity;

};

#endif // DOUBLEINPUTFORM_H
