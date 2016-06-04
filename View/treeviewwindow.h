#ifndef TREEVIEWWINDOW_H
#define TREEVIEWWINDOW_H

#include <QMainWindow>
#include <QItemSelectionModel>
#include <QModelIndexList>

#include "../Model/treemodel.h"
#include "../global_structs.h"

namespace Ui {
class MainWindow;
}

class GlData;
class GeometryEngine;

class TreeViewWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit TreeViewWindow(QWidget *parent = NULL);
    ~TreeViewWindow();

    void init();

    const GlData *getGlDataAtSelection() const;
    void setTranslationAtSel(const QVector3D &t);
    void setScaleAtSel(const QVector3D &s);
    void setRotationAtSel(const QVector3D &r, float theta);
    void setRotationAtSel(const QQuaternion &quat);


    const TreeModel *getTreeModel() const {return treeModel;}
    GeometryEngine *getGEngine() { return gEngine; }

    const QItemSelectionModel *getSelectionModel() const {return &selectionModel; }

signals:
    void model_changed();
//    void selectionChanged(QModelIndex);

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

    void on_selection_changed(const QItemSelection &current, const QItemSelection&);

//    void on_action_New_Perspective_Window_triggered();

    void on_action_Open_Scene_triggered();

    void on_action_Save_Scene_triggered();

    void on_action_New_Scene_triggered();

private:
    Ui::MainWindow *ui;
    TreeModel *treeModel;
    GeometryEngine *gEngine;
    QItemSelectionModel selectionModel;
    QModelIndexList itemsToMove;

    QModelIndex selectedQIndex;

    QModelIndex getFirstSelectedIndex(bool noPrimitives=true) const;
};

#endif // TREEVIEWWINDOW_H
