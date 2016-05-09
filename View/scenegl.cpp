#include "scenegl.h"

SceneGl::SceneGl()
{
    initializeOpenGLFunctions();

}

SceneGl::~SceneGl()
{
    for (QOpenGLBuffer& buf : arrayBufs) {
        buf.destroy();
    }
    for (QOpenGLBuffer& buf : indexBufs) {
        buf.destroy();
    }
}

void SceneGl::addPrimitive(PrimitiveDefinition::Types t)
{
    if (!containsPrim(t)) {
        int i = primitives.size();
        primitives.push_back(PrimitiveDefinition(t));
        arrayBufs.push_back(QOpenGLBuffer());
        indexBufs.push_back(QOpenGLBuffer(QOpenGLBuffer::IndexBuffer));

        PrimitiveDefinition& prim = primitives[i];
        QOpenGLBuffer& arrayBuf = arrayBufs[i];
        QOpenGLBuffer& indexBuf = indexBufs[i];

        // Generate VBOs
        arrayBuf.create();
        indexBuf.create();

        // Transfer vertex data to VBO 0
        arrayBuf.bind();
        arrayBuf.allocate(prim.getVertices(), prim.getNumVertices() * sizeof(VertexData));

        // Transfer index data to VBO 1
        indexBuf.bind();
        indexBuf.allocate(prim.getIndices(), prim.getNumIndices() * sizeof(GLushort));
    }
}

//void SceneGl::removePrimitive(PrimitiveDefinition::Types t)
//{
//    int i = getIndexOfPrim(t);
//    if (i < 0) { return; }

//    // destroy bufs
//    arrayBufs[i].destroy();
//    indexBufs[i].destroy();

//    // remove index
//    //primitives.
//}

/**
 * @brief SceneGl::getIndexOfPrim
 * Looks at every PrimitiveDefinition in primitives, and returns the index
 * of the first entry of type t. Returns -1 if t isn't in the list.
 * @param t         The type of primitive you're looking for
 * @return          The index of the first entry of type t, or -1 if none exists.
 */
int SceneGl::getIndexOfPrim(PrimitiveDefinition::Types t) const {
    int i = 0;
    for (const PrimitiveDefinition& p : primitives) {
        if (p.getType() == t) { return i; }
        ++i;
    }
    return -1;
}

bool SceneGl::containsPrim(PrimitiveDefinition::Types t) const {
    return getIndexOfPrim(t) > -1;
}
