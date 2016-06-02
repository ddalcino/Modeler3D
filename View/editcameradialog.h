#ifndef EDITCAMERADIALOG_H
#define EDITCAMERADIALOG_H

#include <QDialog>



namespace Ui {
class EditCameraDialog;
}
class PerspectiveWindow;

class EditCameraDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditCameraDialog(PerspectiveWindow *parent = 0);
    ~EditCameraDialog();

private slots:
    void on_hSliderFOV_sliderMoved(int position);

    void on_hSliderPosX_sliderMoved(int position);

    void on_hSliderPosY_sliderMoved(int position);

    void on_hSliderPosZ_sliderMoved(int position);

private:
    Ui::EditCameraDialog *ui;

    enum Dim {X, Y, Z};
    void setCamPos(Dim d, float amt);
};

#endif // EDITCAMERADIALOG_H
