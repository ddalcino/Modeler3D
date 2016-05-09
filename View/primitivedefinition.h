#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <vector>
#include <QVector3D>
#include <QOpenGLFunctions>

struct VertexData {
    QVector3D position, normal;
    VertexData(QVector3D pos, QVector3D norm) : position(pos), normal(norm) {}
};

class PrimitiveDefinition
{
public:
    enum Types { CUBE, CYLINDER, CONE, SPHERE };

private:
    std::vector<VertexData> vertices;
    std::vector<GLushort> indices;
    Types primType;

public:

    PrimitiveDefinition(Types t, int numVerticesPerCircle=20, float radius=1, float height=1);

//    const std::vector<VertexData>* getVertices() const { return &vertices; }
//    const std::vector<GLushort>* getIndices() const { return &indices; }
    const VertexData* getVertices() const { return vertices.data(); }
    const GLushort* getIndices() const { return indices.data(); }

    int getNumVertices() const { return vertices.size(); }
    int getNumIndices() const { return indices.size(); }
    Types getType() const { return primType; }
};

#endif // PRIMITIVE_H
