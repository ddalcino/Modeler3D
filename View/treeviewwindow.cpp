#include "treeviewwindow.h"
#include "ui_treeviewwindow.h"
#include "../Model/treemodel.h"

TreeViewWindow::TreeViewWindow(QWidget *parent, TreeModel *model)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      treeModel(model)
{
    ui->setupUi(this);

    ui->treeView->setModel(treeModel);
    //ui->setupUi(this);
    //ui->
}

void TreeViewWindow::on_action_Cube_triggered() {
    treeModel->addObject(PrimitiveDefinition::CUBE);
}

void TreeViewWindow::on_actionC_ylinder_triggered() {
    treeModel->addObject(PrimitiveDefinition::CYLINDER);
}

void TreeViewWindow::on_action_Sphere_triggered() {
    treeModel->addObject(PrimitiveDefinition::SPHERE);
}

void TreeViewWindow::on_actionC_one_triggered() {
    treeModel->addObject(PrimitiveDefinition::CONE);
}
