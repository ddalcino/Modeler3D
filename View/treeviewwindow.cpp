#include "treeviewwindow.h"
#include "perspectivewindow.h"
#include "ui_treeviewwindow.h"
#include "../Model/treemodel.h"
#include "../Model/globject.h"

#include <QItemSelectionModel>
#include <QDebug>

TreeViewWindow::TreeViewWindow(QWidget *parent, TreeModel *model)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      treeModel(model),
      gEngine(NULL),
      selectionModel(model)
{
    ui->setupUi(this);

    ui->treeView->setModel(treeModel);

    ui->treeView->setSelectionModel(&selectionModel);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);

    gEngine = NULL; // new GeometryEngine(this);

    //ui->setupUi(this);
    //ui->
    PerspectiveWindow *p = new PerspectiveWindow(this);
    p->show();

    // Connect selectionModel, selectionChanged to editobjectdialog
}

TreeViewWindow::~TreeViewWindow()
{
    delete ui;
    delete gEngine;
}

const GlData *TreeViewWindow::getGlDataAtSelection() const {
//    qDebug() << "TreeViewWindow::getGlDataAtSelection(): "
//             << "hasSelection()=" << selectionModel.hasSelection()
//             << "currentIndex()=" << selectionModel.currentIndex();


    if (selectionModel.hasSelection()) {
        const QModelIndex index = selectionModel.currentIndex(); // selectionModel.selectedRows().first();
        if (index.isValid()) {
            const GlData *data = treeModel->getGlDataAt(index);
//            qDebug() << "Selected data is: " << data->toString();
            return data;  //  ->getItem(index)
        }
    }
    return NULL;
}

void TreeViewWindow::setTranslationAtSel(const QVector3D &t) {
    if (selectionModel.hasSelection()) {
        const QModelIndex index = selectionModel.selectedRows().first();
        if (index.isValid()) {
            treeModel->setTranslationAt(index, t);
        }
    }
    emit model_changed();
}

void TreeViewWindow::setScaleAtSel(const QVector3D &s)
{
    if (selectionModel.hasSelection()) {
        const QModelIndex index = selectionModel.selectedRows().first();
        if (index.isValid()) {
            treeModel->setScaleAt(index, s);
        }
    }
    emit model_changed();
}

void TreeViewWindow::setRotationAtSel(const QVector3D &r, float theta)
{
    if (selectionModel.hasSelection()) {
        const QModelIndex index = selectionModel.selectedRows().first();
        if (index.isValid()) {
            treeModel->setRotationAt(index, r, theta);
        }
    }
    emit model_changed();
}

void TreeViewWindow::on_action_Cube_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimitiveDefinition::CUBE, parent);
    emit model_changed();
}

void TreeViewWindow::on_actionC_ylinder_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimitiveDefinition::CYLINDER, parent);
    emit model_changed();
}

void TreeViewWindow::on_action_Sphere_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimitiveDefinition::SPHERE, parent);
    emit model_changed();
}

void TreeViewWindow::on_actionC_one_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimitiveDefinition::CONE, parent);
    emit model_changed();
}

void TreeViewWindow::on_action_Copy_triggered()
{
    qDebug() << "Copy";
    // figure out who's selected
    const QModelIndex index = selectionModel.currentIndex();
    treeModel->addToRoot(treeModel->copyObjectAt(index));


//    QModelIndexList list = selectionModel.selectedIndexes();
//    for (const QModelIndex& index : list) {
//        qDebug() << "Copying item at index " << index;
//        treeModel->addToRoot(treeModel->copyObjectAt(index));
//    }
    emit model_changed();
}

void TreeViewWindow::on_action_Move_triggered()
{
    qDebug() << "Move";

    // save who's selected
    //itemsToMove.clear();
    itemsToMove.push_back(getFirstSelectedIndex()); //selectionModel.selectedIndexes();

    // unselect everybody
    selectionModel.clearSelection();

    ui->action_Paste->setEnabled(true);

    emit model_changed();
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
    emit model_changed();
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
    emit model_changed();
}

QModelIndex TreeViewWindow::getFirstSelectedIndex(bool noPrimitives) const
{
    //selectionModel.
    if (selectionModel.currentIndex().isValid()) {
//        qDebug() << "TreeViewWindow::getFirstSelectedIndex() got current index";
        return selectionModel.currentIndex();
    }
//    qDebug() << "TreeViewWindow::getFirstSelectedIndex() no current index";

//    if (selectionModel.selectedIndexes().size() == 1) {
//        QModelIndex index = selectionModel.selectedIndexes().first();
//        if (!(noPrimitives && treeModel->isItemPrimitive(index))) {
//            return index;
//        }
//    }
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
            emit model_changed();
        } else {
            qDebug() << "Can't delete item with name " << index.data()
                     << ": index invalid";
        }
    }
}

//    while (selectionModel.hasSelection()) { //selectedIndexes().isEmpty()) {
//        const QModelIndex& index = selectionModel.selectedIndexes().first();
//        //index.data()
//        qDebug() << "Deleting item with name " << index.data();
//        treeModel->removeRow(index);
//        //treeModel->addToRoot(treeModel->copyObjectAt(index));
//    }


//void TreeViewWindow::on_action_New_Perspective_Window_triggered() {
//    PerspectiveWindow *p = new PerspectiveWindow(this);
//    p->show();
//}
