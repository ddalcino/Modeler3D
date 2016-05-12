#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "primitivedefinition.h"

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine(PrimitiveDefinition::Types t);
    virtual ~GeometryEngine();

    //void drawCubeGeometry(QOpenGLShaderProgram *program);

    void drawPrimGeometry(QOpenGLShaderProgram * program, bool isWireframeMode);

private:
    //void initCubeGeometry();

    void initPrimGeometry(PrimitiveDefinition::Types t);

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

    PrimitiveDefinition* prim;
};

#endif // GEOMETRYENGINE_H
