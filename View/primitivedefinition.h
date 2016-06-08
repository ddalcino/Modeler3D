#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <vector>
#include <QVector3D>
#include <QOpenGLFunctions>

#include "../global_structs.h"

struct VertexData {
    QVector3D position, normal;
    VertexData(QVector3D pos, QVector3D norm) : position(pos), normal(norm) {}
};

/**
 * @brief The DrawData struct
 * A structure that pairs an OpenGL drawing primitive with the number of
 * indices that should be drawn with it, as part of the same object.
 */
struct DrawData {
    GLenum drawType;
    unsigned int boundaryIndex;
    DrawData(GLenum drawType, unsigned int boundary) :
        drawType(drawType), boundaryIndex(boundary) {}
};

class PrimitiveDefinition
{
public:
    //enum Types { CUBE, CYLINDER, CONE, SPHERE, GRID };

private:
    std::vector<VertexData> vertices;
    std::vector<GLushort> indices;
    std::vector<DrawData> drawingPrimitives;
    PrimTypes::Types primType;

public:

    PrimitiveDefinition(PrimTypes::Types t, int numVerticesPerCircle=20,
                        float radius=0.5, float height=0.5);

//    const std::vector<VertexData>* getVertices() const { return &vertices; }
//    const std::vector<GLushort>* getIndices() const { return &indices; }
    const VertexData* getVertices() const { return vertices.data(); }
    const GLushort* getIndices() const { return indices.data(); }

    int getNumVertices() const { return vertices.size(); }
    int getNumIndices() const { return indices.size(); }
    PrimTypes::Types getType() const { return primType; }

//    static const char *toString(Types t) {
//        switch (t) {
//        case PrimTypes::CUBE: return "Cube";
//        case PrimTypes::CYLINDER: return "Cylinder";
//        case PrimTypes::CONE: return "Cone";
//        case PrimTypes::SPHERE: return "Sphere";
//        case PrimTypes::GRID: return "Grid";
//        default: return "Default";
//        }
//    }
//    static QString toQString(Types t) { return QString(toString(t)); }

    static const float GRID_SIZE;
    static const float GRID_DENSITY;

protected:
    void addCircularFace(unsigned int numVerticesPerCircle, float y,
                         float radius, const QVector3D& normal);
};

#endif // PRIMITIVE_H
