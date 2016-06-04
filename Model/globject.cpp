#include "globject.h"

#include <QDebug>

GlObject::GlObject(const GlObject &other)
    : parent(NULL), glData(other.glData),
      // translation(other.translation), scale(other.scale), rotation(other.rotation),
      _isPrimitive(other._isPrimitive),
      _isSelected(false),
      name(other.name) {
    // make new copies of all the children
    for (const GlObject *c : other.children) {
        GlObject *newChild = new GlObject(*c);
        newChild->setParent(this);
        //children.push_back(newChild);
    }
}

/**
 * @brief GlObject::~GlObject
 * Virtual destructor
 * This destructor recursively calls the
 * destructor for each of this object's children, deleting them all.
 */
GlObject::~GlObject() {
//    if (parent != NULL) {
//        reparentChildren();
//        parent->removeChild(this);
//    } else {
        for (GlObject* child : children) {
            if (child == NULL) {
                qDebug() << "Dtor for GlObject " << this << " found null child";
            } else {
                child->setParent(NULL);
                delete child;
                child = NULL;
            }
        }
//    }
}

/**
 * @brief GlObject::setParent
 * Sets the parent of this object to another object, and adds a reference to
 * this object to the new object's list of children. Also removes any reference
 * to this child from the old parent.
 * This is the preferred method of reparenting an individual object.
 * @param newParent     A pointer to the new parent.
 */
void GlObject::setParent(GlObject *newParent) {
    if (newParent != NULL) {
        // remove old parent's reference to self
        if (parent != NULL) {
            parent->removeChild(this, false);
        }
        // add self to newParent's list of children
        newParent->addChild(this);
    }
    parent=newParent;
}

/**
 * @brief GlObject::addChild
 * Adds the parameter GlObject, newChild, to this GlObject's list of children.
 * Does not modify the newChild's parent element; this must be set externally.
 * @param newChild        the child to add
 */
void GlObject::addChild(GlObject *newChild) {
    if (newChild != NULL) {
        children.push_back(newChild);
    } else {
        qDebug() << "addChild for GlObject " << this
                 << " failed to add null child";
    }
}

/**
 * @brief GlObject::removeChild
 * Removes a child, identified by the child's address in the params list,
 * from this object.
 * @param childToRemove  A reference to the child to remove.
 * @param deleteChild    If set to true, the child will be deleted; if not,
 *                       it won't be, and unless you're saving a reference to
 *                       this object somewhere else, we have a memory leak.
 */
void GlObject::removeChild(GlObject *childToRemove, bool deleteChild){
    int i = 0;
    for (GlObject* child : children) {
        if (child == childToRemove) {
            removeChild(i, deleteChild);
        }
        ++i;
    }
}

/**
 * @brief GlObject::removeChild
 * Removes a child, at an index specified in the params list, from this object.
 * @param childIndex
 * @param deleteChild    If set to true, the child will be deleted; if not,
 *                       it won't be, and unless you're saving a reference to
 *                       this object somewhere else, we have a memory leak.
 */
bool GlObject::removeChild(size_t childIndex, bool deleteChild) {
    if (childIndex >= children.size()) {
        qDebug() << "Failed to remove nonexistent child at index" << childIndex;
        return false;
    } else {
        if (deleteChild) {
            // delete child
            delete children[childIndex];
        }
        // move later children up the vector
        for (size_t i = childIndex+1; i < children.size(); ++i) {
            children[i-1] = children[i];
        }
        children.resize(children.size()-1);
        return true;
    }
}

void GlObject::clearChildren() {
    for (GlObject* child : children) {
        if (child == NULL) {
            qDebug() << "GlObject::clearChildren() for " << this << " found null child";
        } else {
            child->setParent(NULL);
            delete child;
            child = NULL;
        }
    }
}

/**
 * @brief GlObject::reparentChildren
 * Moves all of this object's children into the list of children maintained by
 * the parent, and sets their parent pointers accordingly. Empties this object's
 * list of children.
 */
void GlObject::reparentChildren() {
    if (parent == NULL) {
        qDebug() << "Failed to reparent children to null pointer";
    } else {
        for (GlObject* child : children) {
            if (child == NULL) {
                qDebug() << "reparentChildren for GlObject " << this
                         << " found null child";
            } else {
                //parent->addChild(child);
                child->setParent(parent);
                child = NULL;
            }
        }
        children.clear();
    }
}

bool GlObject::setName(const QString &newName)
{
    if (newName.compare(name)) {
        name = newName;
        return true;
    } else {
        return false;
    }
}

int GlObject::getIndexOfChild(const GlObject *child) const
{
    int i = 0;
    for (const GlObject* c : children) {
        if (c == child) { return i; }
        ++i;
    }
    return -1;
}

bool GlObject::hasAncestor(const GlObject *other) const
{
    const GlObject *ancestor = this->parent;
    while (ancestor != NULL) {
        if (ancestor == other) { return true; }
        ancestor = ancestor->parent;
    }
    return false;
}

//bool GlObject::insertChildren(int position, int rowCount)
//{

//}

//bool GlObject::removeChildren(int position, int rowCount)
//{

//}

void GlObject::getDrawingDirections(std::vector<DrawDirections> &dir,
                                    const DrawDirections &next) const {
    // copy the draw directions for the next item
    DrawDirections current = next;

    // apply rotation, translation, and scale transformations
    current.mat.rotate(glData.rotation);
    current.rot.rotate(glData.rotation);
    current.mat.translate(glData.translation);
    current.rot.translate(glData.translation);
    current.mat.scale(glData.scale);

    // loop through all the children and call getDrawingDirections on them
    for (const GlObject* child : children) {
        if (child) {
            child->getDrawingDirections(dir, current);
        } else {
            qDebug() << "Called getDrawingDirections on null child";
        }
    }
}

void GlPrimitiveObject::getDrawingDirections(std::vector<DrawDirections> &dir,
                                             const DrawDirections &next) const {
    // copy the draw directions for the next item
    DrawDirections current = next;

    // apply rotation, translation, and scale transformations
    current.mat.rotate(glData.rotation);
    current.rot.rotate(glData.rotation);
    current.mat.translate(glData.translation);
    current.rot.translate(glData.translation);
    current.mat.scale(glData.scale);

    // set definition to whatever this object if defined as
    current.def = this->definition;

    // add directions to draw this item to the vector
    dir.push_back(current);
}
