/****************************************************************************
**
** This class handles all the OpenGL calls, in order to keep all OpenGL state
** machine code in one place, to keep it manageable. It is meant to be used
** as a singleton, so that no other code can get in the way.
**
****************************************************************************/


#ifndef SCENEGL_H
#define SCENEGL_H

// STL includes
#include <vector>

// QT includes
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

// custom code
#include "primitivedefinition.h"

class SceneGl
{

    std::vector<PrimitiveDefinition> primitives;
    std::vector<QOpenGLBuffer> arrayBufs;
    std::vector<QOpenGLBuffer> indexBufs;

public:
    SceneGl();
    ~SceneGl();

    void addPrimitive(PrimitiveDefinition::Types t);
//    void removePrimitive(PrimitiveDefinition::Types t);

    int getIndexOfPrim(PrimitiveDefinition::Types t) const;
    bool containsPrim(PrimitiveDefinition::Types t) const;
};

#endif // SCENEGL_H
