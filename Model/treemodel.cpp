#include <QtWidgets>

#include "globject.h"
#include "treemodel.h"

TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent) {

    rootItem = new GlObject("Root");
    rootItem->addChild(new GlPrimitiveObject("Cube", rootItem));
    //setupModelData(data.split(QString("\n")), rootItem);
}

TreeModel::~TreeModel() {
    delete rootItem;
}

int TreeModel::columnCount(const QModelIndex & /* parent */) const {
    return 1; //rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    GlObject *item = getItem(index);

    return item->getName();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()){
        return 0;
    }
    const GlObject *item = getItem(index);
    if (item && !item->isPrimitive()) {
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    } else {
        return QAbstractItemModel::flags(index);
    }
}

GlObject *TreeModel::getItem(const QModelIndex &index) const {
//    qDebug() << "getItem(): Index is: " << index << ", and it's valid=" << index.isValid();
    if (index.isValid()) {
//        qDebug() << "Index is: " << index << ", and it's valid=" << index.isValid();

        GlObject *item = static_cast<GlObject*>(index.internalPointer());

//        const GlObject *itemC = item;
//        if (itemC) {
//            const GlData * data = itemC->getGlData();
//            qDebug() << "Data at index is: " << data->toString();
//        }


        if (item)
            return item;
    }
    return rootItem;
}

const GlData *TreeModel::getGlDataAt(const QModelIndex &index) const {
    if (index.isValid()) {
        const GlObject *item = getItem(index);
        if (item) {
//            const GlData * data = item->getGlData();
//            qDebug() << "Selected data is: " << data->toString();
            return getItem(index)->getGlData();
        }
    }
    return NULL;
}

void TreeModel::setTranslationAt(const QModelIndex &index, const QVector3D &t) {
    if (index.isValid()) {
        getItem(index)->setTranslation(t);
    }
}

void TreeModel::setScaleAt(const QModelIndex &index, const QVector3D &s) {
    if (index.isValid()) {
        getItem(index)->setScale(s);
    }
}

void TreeModel::setRotationAt(const QModelIndex &index, const QVector3D &rAxis, float theta) {
    if (index.isValid()) {
        getItem(index)->setRotation(QQuaternion(theta, rAxis));
    }
}

void TreeModel::setRotationAt(const QModelIndex &index, const QQuaternion &quat) {
    if (index.isValid()) {
        getItem(index)->setRotation(quat);
    }
}

void TreeModel::getDrawingDirections(std::vector<DrawDirections> &dirs,
                                     DrawDirections &next) const {
    rootItem->getDrawingDirections(dirs, next);
}

QVariant TreeModel::headerData(int, Qt::Orientation orientation,
                               int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->getName();

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    GlObject *parentItem = getItem(parent);

    GlObject *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
/*
bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}*/

//bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
//{
//    qDebug() << "insertRows(" << position << "," << rows << "," << parent << "," << ") called";
//    GlObject *parentItem = getItem(parent);
//    bool success;

//    beginInsertRows(parent, position, position + rows - 1);
//    success = parentItem->insertChildren(position, rows, 1);
//    endInsertRows();

//    return success;
//}

//! [7]
QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    GlObject *childItem = getItem(index);
    GlObject *parentItem = childItem->getParent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->getChildNumber(), 0, parentItem);
}

/*bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}*/

//bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent){
//    GlObject *parentItem = getItem(parent);
//    bool success = true;

//    beginRemoveRows(parent, position, position + rows - 1);
//    success = parentItem->removeChildren(position, rows);
//    endRemoveRows();

//    return success;
//}

int TreeModel::rowCount(const QModelIndex &parent) const{
    GlObject *parentItem = getItem(parent);

    return parentItem->getNumChildren();
}
//! [8]

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    GlObject *item = getItem(index);
    bool result = item->setName(value.toString()); // ->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setName(value.toString()); // ->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void TreeModel::addObject(PrimTypes::Types t, QModelIndex& parentIndex)
{
    //    beginInsertRows(parent, position, position + rows - 1);
    //    success = parentItem->insertChildren(position, rows, 1);
    //    endInsertRows();
    if (!parentIndex.isValid()) { parentIndex = QModelIndex(); }
    GlObject *parent = getItem(parentIndex);
    int newChildIndex = parent->getNumChildren();
    beginInsertRows(parentIndex, newChildIndex,
                    newChildIndex);
    parent->addChild(new GlPrimitiveObject(
                         PrimTypes::toQString(t), parent));
    endInsertRows();
}

void TreeModel::addToRoot(GlObject *item)
{
    if (rootItem && item) {
        beginInsertRows(QModelIndex(), rootItem->getNumChildren(),
                        rootItem->getNumChildren());
        item->setParent(rootItem);
        endInsertRows();
    }
}

void TreeModel::clear() {
    beginRemoveRows(QModelIndex(), 0, rootItem ->getNumChildren()-1);
    rootItem->clearChildren();
    endRemoveRows();
}

QModelIndex TreeModel::addGroupToRoot()
{
    beginInsertRows(QModelIndex(), rootItem->getNumChildren(),
                    rootItem->getNumChildren());
    rootItem->addChild(new GlObject("Group", rootItem));
    endInsertRows();
    return index(rootItem->getNumChildren()-1, 0, QModelIndex());
}

bool TreeModel::moveItems(const QModelIndexList &toMove, const QModelIndex &dest)
{
    GlObject *destItem = getItem(dest);
    bool itemsMoved = false;
    // only allow movement into an item that's not a primitive
    if (destItem->isPrimitive()) { return false; }

    for (const QModelIndex& srcIndex : toMove) {
        GlObject * srcItem = getItem(srcIndex);
        // first, ensure that we're not moving a parent item into one
        // of its children, and creating a cycle in the acyclic graph
        if (!destItem->hasAncestor(srcItem)) {


//            qDebug() << " beginMoveRows(srcIndex.parent()= " << srcIndex.parent()
//                     << ", srcItem->getChildNumber()=" << srcItem->getChildNumber()
//                     << ", srcItem->getChildNumber()+1=" << srcItem->getChildNumber()+1
//                     << ", dest=" << dest
//                     << ", destItem->getNumChildren()=" <<destItem->getNumChildren()
//                     << " );";
            beginMoveRows(srcIndex.parent(), srcItem->getChildNumber(),
                          srcItem->getChildNumber(),
                          dest, destItem->getNumChildren() );
            srcItem->setParent(destItem);
            endMoveRows();
            itemsMoved = true;
        }
    }
    return itemsMoved;
}

bool TreeModel::removeRow(int row, const QModelIndex &parent) {
    GlObject *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, row, row);
    //    success = parentItem->removeChildren(position, rows);
    success = parentItem->removeChild(row, true);
    endRemoveRows();

    return success;

}

bool TreeModel::removeRow(const QModelIndex &toRemove)
{
    // get the row number for toRemove:
    const GlObject *item = (const GlObject *)(toRemove.internalPointer());
    int row = item->getChildNumber();
    return removeRow(row, toRemove.parent());
}

GlObject *TreeModel::copyObjectAt(const QModelIndex &index) {
    if (!index.isValid()) { return NULL; }

    const GlObject* toCopy = getItem(index);
    if (!toCopy) { return NULL; }
    GlObject* copy = new GlObject(*toCopy);
    return copy;
}

bool TreeModel::isItemPrimitive(const QModelIndex &index) const {
    GlObject *item = getItem(index);
    if (item) { return item->isPrimitive(); }
    else { return false; }
}

/*
void TreeModel::setupModelData(const QStringList &lines, GlObject *parent)
{
    QList<GlObject*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            ++position;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            GlObject *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }

        ++number;
    }
}*/
