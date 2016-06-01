#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include <vector>

#include "globject.h"

/**
 * @brief ObjectIndex: used to make references to a particular object in the
 * SceneGraph. This will be a list of indices. The whole point of this is to
 * avoid giving anyone else access to pointers directly into this structure.
 */
typedef std::vector<size_t> ObjectIndex;

class SceneGraph
{
    GlObject* root;
public:
    SceneGraph();
    ~SceneGraph();

    std::vector<DrawDirections> getDrawingDirections() const;

    bool isValidObjectIndex(ObjectIndex oi) const;
    GlObject *getObjectAt(ObjectIndex oi);

    bool reparentObject(ObjectIndex src, ObjectIndex newParent);
};

#endif // SCENE_H
