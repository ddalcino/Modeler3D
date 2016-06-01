#ifndef TREEVIEWWINDOW_H
#define TREEVIEWWINDOW_H

#include <QMainWindow>

#include "../Model/treemodel.h"

namespace Ui {
class MainWindow;
}

class TreeViewWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit TreeViewWindow(QWidget *parent = NULL,
                            TreeModel *model = new TreeModel());

signals:

public slots:

private slots:
    void on_action_Cube_triggered();

    void on_actionC_ylinder_triggered();

    void on_action_Sphere_triggered();

    void on_actionC_one_triggered();

private:
    Ui::MainWindow *ui;
    TreeModel *treeModel;
};

#endif // TREEVIEWWINDOW_H
