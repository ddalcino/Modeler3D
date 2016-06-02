#include "treeviewwindow.h"
#include "ui_treeviewwindow.h"
#include "../Model/treemodel.h"

#include <QItemSelectionModel>
#include <QDebug>

TreeViewWindow::TreeViewWindow(QWidget *parent, TreeModel *model)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      treeModel(model),
      selectionModel(model)
{
    ui->setupUi(this);

    ui->treeView->setModel(treeModel);

    ui->treeView->setSelectionModel(&selectionModel);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    //ui->setupUi(this);
    //ui->
}

TreeViewWindow::~TreeViewWindow()
{
    delete ui;
}

void TreeViewWindow::on_action_Cube_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimitiveDefinition::CUBE, parent);
}

void TreeViewWindow::on_actionC_ylinder_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimitiveDefinition::CYLINDER, parent);
}

void TreeViewWindow::on_action_Sphere_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimitiveDefinition::SPHERE, parent);
}

void TreeViewWindow::on_actionC_one_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimitiveDefinition::CONE, parent);
}

void TreeViewWindow::on_action_Copy_triggered()
{
    qDebug() << "Copy";
    // figure out who's selected

    QModelIndexList list = selectionModel.selectedIndexes();
    for (const QModelIndex& index : list) {
        qDebug() << "Copying item at index " << index;
        treeModel->addToRoot(treeModel->copyObjectAt(index));
    }
}

void TreeViewWindow::on_action_Move_triggered()
{
    qDebug() << "Move";

    // save who's selected
    itemsToMove.clear();
    itemsToMove = selectionModel.selectedIndexes();

    // unselect everybody
    selectionModel.clearSelection();

    ui->action_Paste->setEnabled(true);

}

void TreeViewWindow::on_action_Paste_triggered()
{
    //TODO LATER!!

    // Only proceed if there's one item selected, and something to move
    QModelIndexList destList = selectionModel.selectedIndexes();
    if (destList.size() == 1 && itemsToMove.size() > 0) {
        QModelIndex dest = destList.first();
        treeModel->moveItems(itemsToMove, dest);
        itemsToMove.clear();
    }
    //ui->treeView->repaint();

    ui->action_Paste->setEnabled(false);
}

void TreeViewWindow::on_action_Group_triggered()
{
    QModelIndex groupIndex = treeModel->addGroupToRoot();

    qDebug() << "groupIndex =" << groupIndex << ", isValid=" << groupIndex.isValid();

    // see who's selected
    itemsToMove.clear();
    itemsToMove = selectionModel.selectedIndexes();

    // if there's one item selected, move it into the new group
    if (itemsToMove.size() == 1) {
        // add everything selected to the last group
        treeModel->moveItems(itemsToMove, groupIndex);
        itemsToMove.clear();
    }
}

QModelIndex TreeViewWindow::getFirstSelectedIndex(bool noPrimitives) const
{
    if (selectionModel.selectedIndexes().size() == 1) {
        QModelIndex index = selectionModel.selectedIndexes().first();
        if (!(noPrimitives && treeModel->isItemPrimitive(index))) {
            return index;
        }
    }
    return QModelIndex();
}

void TreeViewWindow::on_action_Delete_triggered()
{
    qDebug() << "Delete";
    // figure out who's selected
    QModelIndexList selected = selectionModel.selectedIndexes();

    //while (!selectionModel.selectedIndexes().isEmpty()) {
    if (!selected.isEmpty()) {
        const QModelIndex& index = //selectionModel.selectedIndexes().first();
                selected.first();
        if (index.isValid()) {
            qDebug() << "Deleting item with name " << index.data();
            treeModel->removeRow(index);

        } else {
            qDebug() << "Can't deleting item with name " << index.data()
                     << ": index invalid";

        }

    }

//    while (selectionModel.hasSelection()) { //selectedIndexes().isEmpty()) {
//        const QModelIndex& index = selectionModel.selectedIndexes().first();
//        //index.data()
//        qDebug() << "Deleting item with name " << index.data();
//        treeModel->removeRow(index);
//        //treeModel->addToRoot(treeModel->copyObjectAt(index));
//    }

}
