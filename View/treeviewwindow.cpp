#include "treeviewwindow.h"
#include "perspectivewindow.h"
#include "ui_treeviewwindow.h"
#include "../Model/scenexmlhandler.h"
#include "../Model/treemodel.h"
#include "../Model/globject.h"

#include <QItemSelectionModel>
#include <QDebug>
#include <QFileDialog>

class GlObject;

TreeViewWindow::TreeViewWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
//      p(new PerspectiveWindow(this)),
      treeModel(new TreeModel(this)),
      gEngine(NULL),
      selectionModel(new QItemSelectionModel(treeModel, this))
{
    ui->setupUi(this);
    init();

    gEngine = NULL; // new GeometryEngine(this);

    p = new PerspectiveWindow(this),
    p->show();

}

TreeViewWindow::~TreeViewWindow()
{
    delete ui;
    if (treeModel) { delete treeModel; treeModel=NULL; }
    //if (selectionModel) { delete selectionModel; selectionModel=NULL; }
    if (gEngine) { delete gEngine; gEngine=NULL; }
}

void TreeViewWindow::init()
{
    ui->treeView->setModel(treeModel);
    ui->treeView->setSelectionModel(selectionModel);
    ui->treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            this, SLOT(on_selection_changed(const QItemSelection &, const QItemSelection &)));
    emit selectionChanged();
}

GlData TreeViewWindow::getGlDataAtSelection() const {
//    qDebug() << "TreeViewWindow::getGlDataAtSelection(): "
//             << "hasSelection()=" << selectionModel.hasSelection()
//             << "currentIndex()=" << selectionModel.currentIndex();


//    if (selectionModel.hasSelection()) {
//        const QModelIndex index = selectionModel.currentIndex(); // selectionModel.selectedRows().first();
        if (selectedQIndex.isValid()) {
            const GlData *data = treeModel->getGlDataAt(selectedQIndex);
//            qDebug() << "Selected data is: " << data->toString();
            return GlData(*data);  //  ->getItem(index)
        }
//    }
        throw "Nothing selected yet";
        return GlData();
}

const GlObject *TreeViewWindow::getSelectedObject() const
{
    if (selectedQIndex.isValid()) {
        const GlObject *data = treeModel->getConstItem(selectedQIndex);
        return data;
    }
    return NULL;
}

void TreeViewWindow::setTranslationAtSel(const QVector3D &t) {
    qDebug() << "TreeViewWindow::setTranslationAtSel()";
//    if (selectionModel.hasSelection()) {
//        const QModelIndex index = selectionModel.selectedRows().first();
    if (selectedQIndex.isValid()) {
        treeModel->setTranslationAt(selectedQIndex, t);
//        }
    } else {
        qDebug() << "selection invalid";
    }
    emit model_changed();
}

void TreeViewWindow::setScaleAtSel(const QVector3D &s)
{
    qDebug() << "TreeViewWindow::setScaleAtSel()";
//    if (selectionModel.hasSelection()) {
//        const QModelIndex index = selectionModel.selectedRows().first();
        if (selectedQIndex.isValid()) {
            treeModel->setScaleAt(selectedQIndex, s);
        }
//    }
    emit model_changed();
}

void TreeViewWindow::setRotationAtSel(const QVector3D &r, float theta)
{
    qDebug() << "TreeViewWindow::setRotationAtSel(qvector3d, theta)";
//    if (selectionModel.hasSelection()) {
//        const QModelIndex index = selectionModel.selectedRows().first();
        if (selectedQIndex.isValid()) {
            treeModel->setRotationAt(selectedQIndex, r, theta);
        }
    //}
    emit model_changed();
}

void TreeViewWindow::setRotationAtSel(const QQuaternion &quat) {
    qDebug() << "TreeViewWindow::setRotationAtSel(quat)";
    if (selectedQIndex.isValid()) {
        treeModel->setRotationAt(selectedQIndex, quat);

    } else {
        qDebug() << "Selection invalid";
    }
//    //if (selectionModel.hasSelection())
//    {
//        const QModelIndex index = selectionModel.currentIndex();
//                //selectionModel.selectedRows().first();
//        if (index.isValid()) {
//            treeModel->setRotationAt(index, quat);
//        }
//    }
    emit model_changed();
}

void TreeViewWindow::getDrawingDirections(std::vector<DrawDirections> &dir) const
{
    DrawDirections next;
    return treeModel->getDrawingDirections(dir, next, getSelectedObject());
}

//const TreeModel *TreeViewWindow::getTreeModel() const {return treeModel;}

//GeometryEngine *TreeViewWindow::getGEngine() { return gEngine; }

//const QItemSelectionModel *TreeViewWindow::getSelectionModel() const {return selectionModel; }

void TreeViewWindow::on_action_Cube_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimTypes::CUBE, parent);
    emit model_changed();
}

void TreeViewWindow::on_actionC_ylinder_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimTypes::CYLINDER, parent);
    emit model_changed();
}

void TreeViewWindow::on_action_Sphere_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimTypes::SPHERE, parent);
    emit model_changed();
}

void TreeViewWindow::on_actionC_one_triggered() {
    QModelIndex parent = getFirstSelectedIndex();
    treeModel->addObject(PrimTypes::CONE, parent);
    emit model_changed();
}

void TreeViewWindow::on_action_Copy_triggered()
{
    qDebug() << "Copy";
    // figure out who's selected
    const QModelIndex index = selectionModel->currentIndex();
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
    itemsToMove.push_back(getFirstSelectedIndex(false)); //selectionModel.selectedIndexes();

    // unselect everybody
    selectionModel->clearSelection();

    ui->action_Paste->setEnabled(true);

    emit model_changed();
}

void TreeViewWindow::on_action_Paste_triggered()
{
    qDebug() << "Paste";


    // Only proceed if there's one item selected, and something to move
//    QModelIndexList destList = selectionModel.selectedIndexes();
//    if (destList.size() == 1 && itemsToMove.size() > 0) {
    QModelIndex dest = getFirstSelectedIndex();
    if(dest.isValid() && itemsToMove.size() > 0) {
        treeModel->moveItems(itemsToMove, dest);
        itemsToMove.clear();
        ui->action_Paste->setEnabled(false);
        emit model_changed();
    }
    //ui->treeView->repaint();

}

void TreeViewWindow::on_action_Group_triggered()
{
    QModelIndex groupIndex = treeModel->addGroupToRoot();

    qDebug() << "groupIndex =" << groupIndex << ", isValid=" << groupIndex.isValid();

    // see who's selected
    itemsToMove.clear();
    itemsToMove = selectionModel->selectedIndexes();

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
    QModelIndex index = selectionModel->currentIndex();
    if (index.isValid()) {
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
    QModelIndexList selected = selectionModel->selectedIndexes();

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

void TreeViewWindow::on_selection_changed(const QItemSelection &current, const QItemSelection &) {
    qDebug() << "TreeViewWindow::on_selection_changed()";
    if (!current.isEmpty() && !current.indexes().isEmpty()) {
        selectedQIndex = current.indexes().first();
    } else {
        selectedQIndex = QModelIndex();
    }
    emit selectionChanged();  //selectionChanged();
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

void TreeViewWindow::on_action_Open_Scene_triggered()
{
    QString inFileName = QFileDialog::getOpenFileName(this,
        tr("Open SceneGraphXML File"), QString(), tr("SceneGraphXML Files (*.xml *.sgXML)"));

    SceneGraphXMLHandler xmlReader(NULL);
    GlObject * newRoot = xmlReader.readFile(inFileName);
    if (newRoot && treeModel) {
        // success

        delete selectionModel;
        selectionModel = NULL;
        delete treeModel;
        treeModel = new TreeModel(this, newRoot);
        //treeModel->addToRoot();
        selectionModel = new QItemSelectionModel(treeModel, this);
        selectedQIndex = QModelIndex();

        init();
        update();
        emit model_changed();
    }
}

void TreeViewWindow::on_action_Save_Scene_triggered()
{
    QString outFileName = QFileDialog::getSaveFileName(this,
        tr("Save SceneGraphXML File"), QString(), tr("SceneGraphXML Files (*.xml *.sgXML)"));
    if (treeModel && treeModel->getRoot()) {
        SceneGraphXMLHandler::writeXml(outFileName, treeModel->getRoot());
    }

}

void TreeViewWindow::on_action_New_Scene_triggered()
{
    delete selectionModel;
    selectionModel = NULL;
    delete treeModel;
    treeModel = new TreeModel(this);
    selectionModel = new QItemSelectionModel(treeModel, this);
    selectedQIndex = QModelIndex();

    init();
    update();
    emit model_changed();
 }

void TreeViewWindow::on_action_Import_Scene_triggered()
{
    QString inFileName = QFileDialog::getOpenFileName(this,
        tr("Import From SceneGraphXML File"), QString(), tr("SceneGraphXML Files (*.xml *.sgXML)"));

    SceneGraphXMLHandler xmlReader(NULL);
    GlObject * importedRoot = xmlReader.readFile(inFileName);
    if (importedRoot && treeModel) {
        // success
        importedRoot->setName(inFileName);
        treeModel->addToRoot(importedRoot);
        GlObject *root = treeModel->getRoot();

        delete selectionModel;
        selectionModel = NULL;
        delete treeModel;
        treeModel = new TreeModel(this, root);
        selectionModel = new QItemSelectionModel(treeModel, this);
        selectedQIndex = QModelIndex();

        init();
        update();
        emit model_changed();
    }

}
