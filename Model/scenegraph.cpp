#include "scenegraph.h"

SceneGraph::SceneGraph() {
    root = new GlObject("root", NULL, false);
}

SceneGraph::~SceneGraph() {
    delete root;
}

std::vector<DrawDirections> SceneGraph::getDrawingDirections() const {
    std::vector<DrawDirections> dir;
    DrawDirections next;
    root->getDrawingDirections(dir, next);
    return dir;
}

bool SceneGraph::isValidObjectIndex(ObjectIndex oi) const {
    const GlObject* obj = root;
    for (size_t i : oi) {
        if (obj != NULL && obj->getChildren().size() > i) {
            obj = obj->getChildren()[i]; //  [i];
        } else {
            return false;
        }
    }
    return true;
}

GlObject *SceneGraph::getObjectAt(ObjectIndex oi) {
    GlObject* obj = root;
    size_t depth = 0;
    for (size_t i : oi) {
        if (obj != NULL && obj->getChildren().size() > i) {
            obj = obj->getChildren()[i]; //  [i];
            depth++;
        } else {
            return NULL;
        }
    }
    return (depth == oi.size()) ? obj : NULL;
}

bool SceneGraph::reparentObject(ObjectIndex src, ObjectIndex newParent) {
    GlObject* source = getObjectAt(src);
    GlObject* dest = getObjectAt(newParent);
    if (source != NULL && dest != NULL) {
        source->setParent(dest);
        //dest.addChild(source);
        return true;
    } else {
        return false;
    }
}


