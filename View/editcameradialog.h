#ifndef EDITCAMERADIALOG_H
#define EDITCAMERADIALOG_H

#include <QDialog>
#include <QVector3D>


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

    void on_lookXAxis_clicked();

    void on_lookYAxis_clicked();

    void on_lookZAxis_clicked();

private:
    Ui::EditCameraDialog *ui;
    PerspectiveWindow *parent;

    enum Dim {X, Y, Z};
    void setCamPos(Dim d, float amt);
    void setCamRot(Dim axis);
    QVector3D makePosOnAxis(Dim d, float distance);
};

#endif // EDITCAMERADIALOG_H
