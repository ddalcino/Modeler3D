#ifndef TREEVIEWWINDOW_H
#define TREEVIEWWINDOW_H

#include <QMainWindow>
#include <QItemSelectionModel>
#include <QModelIndexList>

#include "../Model/treemodel.h"

namespace Ui {
class MainWindow;
}

class GlData;

class TreeViewWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit TreeViewWindow(QWidget *parent = NULL,
                            TreeModel *model = NULL);
    ~TreeViewWindow();

    const GlData *getGlDataAtSelection() const;
    void setTranslationAtSel(const QVector3D &t);
    void setScaleAtSel(const QVector3D &s);
    void setRotationAtSel(const QVector3D &r, float theta);


    const TreeModel *getTreeModel() const {return treeModel;}

signals:
    void model_changed();

public slots:

private slots:
    void on_action_Cube_triggered();

    void on_actionC_ylinder_triggered();

    void on_action_Sphere_triggered();

    void on_actionC_one_triggered();

    void on_action_Copy_triggered();

    void on_action_Move_triggered();

    void on_action_Paste_triggered();

    void on_action_Group_triggered();

    void on_action_Delete_triggered();

private:
    Ui::MainWindow *ui;
    TreeModel *treeModel;
    QItemSelectionModel selectionModel;
    QModelIndexList itemsToMove;

    QModelIndex getFirstSelectedIndex(bool noPrimitives=true) const;
};

#endif // TREEVIEWWINDOW_H
