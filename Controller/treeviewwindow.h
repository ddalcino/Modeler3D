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
class PerspectiveWindow;
class HelpDialog;
class AboutDialog;

/**
 * @brief The TreeViewWindow class
 * This class is used as a proxy for the controller. Given that this program
 * uses QTreeView as a means of interacting with the central data structure,
 * which uses a Delegate pattern, it is impossible to implement a true MVC
 * pattern here. Instead, I am using the TreeViewWindow class as a controller
 * that sits between the model classes and view classes in general, and
 * manages all interactions between the two. However, the TreeViewWindow
 * class contains model classes and view classes as data members, and they are
 * allowed to interact with each other, as required by Qt's implementation
 * of the Delegate pattern, so this cannot be considered a real 'controller',
 * although it is used as such.
 */
class TreeViewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TreeViewWindow(QWidget *parent = NULL);
    ~TreeViewWindow();

    void init();

    GlData getGlDataAtSelection() const;
    void setTranslationAtSel(const QVector3D &t);
    void setScaleAtSel(const QVector3D &s);
//    void setRotationAtSel(const QVector3D &r, float theta);
    void setRotationAtSel(const QQuaternion &quat);

    void getDrawingDirections(std::vector<DrawDirections>& dir) const;
//    const TreeModel *getTreeModel() const;
//    GeometryEngine *getGEngine();

//    const QItemSelectionModel *getSelectionModel() const;

signals:
    void model_changed();
    void selectionChanged();

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

    void on_action_Import_Scene_triggered();

    void on_action_Instructions_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
    PerspectiveWindow *p;
    AboutDialog *aboutDialog;
    HelpDialog *helpDialog;
    TreeModel *treeModel;
    GeometryEngine *gEngine;
    QItemSelectionModel *selectionModel;
    QModelIndexList itemsToMove;

    QModelIndex selectedQIndex;

    const GlObject *getSelectedObject() const;
    QModelIndex getFirstSelectedIndex(bool noPrimitives=true) const;
};

#endif // TREEVIEWWINDOW_H
