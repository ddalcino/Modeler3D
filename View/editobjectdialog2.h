#ifndef EDITOBJECTDIALOG2_H
#define EDITOBJECTDIALOG2_H

#include <QDialog>

class TreeViewWindow;

namespace Ui {
class EditObjectDialog2;
}

class EditObjectDialog2 : public QDialog
{
    Q_OBJECT

public:
    enum Axis {X, Y, Z};
    explicit EditObjectDialog2(QWidget *parent = NULL, TreeViewWindow *tvWin = NULL);
    ~EditObjectDialog2();

public slots:
    void init();

private slots:

    void updateTranslation();
    void updateScale();
    void updateRotation(Axis axis, bool isNegative);

    void on_setPosXY_clicked();

    void on_setPosXZ_clicked();

    void on_setPosYX_clicked();

    void on_setPosYZ_clicked();

    void on_setPosZX_clicked();

    void on_setPosZY_clicked();

    void on_setScaleXY_clicked();

    void on_setScaleXZ_clicked();

    void on_setScaleYX_clicked();

    void on_setScaleYZ_clicked();

    void on_setScaleZX_clicked();

    void on_setScaleZY_clicked();

    void on_rotateX_clicked();

    void on_rotNegX_clicked();

    void on_rotateY_clicked();

    void on_rotNegY_clicked();

    void on_rotateZ_clicked();

    void on_rotNegZ_clicked();

private:
    Ui::EditObjectDialog2 *ui;
    TreeViewWindow *tvWindow;
};

#endif // EDITOBJECTDIALOG2_H
