#include "primitivedefinition.h"
#include <cmath>

#define PI 3.14159265


PrimitiveDefinition::PrimitiveDefinition(Types t, int numVerticesPerCircle,
                                         float radius, float height) : primType(t)
{
    switch (t) {
    case CYLINDER:
        // start by defining the curved surface
        for (int i = 0; i <= numVerticesPerCircle; ++i) {
            float startAngle = (2*PI * i) / numVerticesPerCircle;
            //float endAngle = (2*PI * (i+1)) / numVerticesPerCircle;
            float x = cos(startAngle) * radius;
            float z = sin(startAngle) * radius;
            float y = height;
            vertices.push_back(VertexData(QVector3D(x, 0, z),
                                          QVector3D(x, 0, z)));
            indices.push_back(vertices.size()-1); //2*i);
            vertices.push_back(VertexData(QVector3D(x, y, z),
                                          QVector3D(x, 0, z)));
            indices.push_back(vertices.size()-1); //2*i + 1);
        }
        // duplicate the last index to end the triangle strip
        indices.push_back(vertices.size()-1); //2*numVerticesPerCircle +1);
        break;
    case CONE:
        // start by defining the curved surface
        for (int i = 0; i <= numVerticesPerCircle; ++i) {
            float startAngle = (2*PI * i) / numVerticesPerCircle;
            //float endAngle = (2*PI * (i+1)) / numVerticesPerCircle;
            float x = cos(startAngle) * radius;
            float z = sin(startAngle) * radius;
            float y = height;
            vertices.push_back(VertexData(QVector3D(x, 0, z),
                                          QVector3D(x, 0, z)));
            indices.push_back(vertices.size()-1); //2*i);
            vertices.push_back(VertexData(QVector3D(0, y, 0),
                                          QVector3D(0, 1.0, 0)));
            indices.push_back(vertices.size()-1); //2*i + 1);
        }
        // duplicate the last index to end the triangle strip
        indices.push_back(vertices.size()-1); //2*numVerticesPerCircle +1);
        break;
    case SPHERE:
        // each i represents an 'orange wedge' shaped triangle strip, from top to bottom
        for (int i = 0; i < numVerticesPerCircle; ++i) {
            double startAngle = (2*PI * i) / numVerticesPerCircle;
            double endAngle = (2*PI * (i+1)) / numVerticesPerCircle;
            float cosTheta[] = {(float)cos(startAngle), (float)cos(endAngle)};    // x = cosTheta * radius
            float sinTheta[] = {(float)sin(startAngle), (float)sin(endAngle)};    // z = sinTheta * radius

            // now fan out a triangle strip over the longitude of the wedge

            // straight up
            vertices.push_back(VertexData(QVector3D(0, radius, 0),
                                          QVector3D(0, 1.0, 0)));
            indices.push_back(vertices.size()-1);

            for (int j = 1; j < numVerticesPerCircle/2; ++j) {
                float sliceRadius = sin((PI * j) / (numVerticesPerCircle/2));
                QVector3D first(cosTheta[0]*sliceRadius, 0, sinTheta[0]*sliceRadius);
                QVector3D second(cosTheta[1]*sliceRadius, 0, sinTheta[1]*sliceRadius);
                vertices.push_back(VertexData(first, first.normalized()));
                indices.push_back(vertices.size()-1);
                vertices.push_back(VertexData(second, second.normalized()));
                indices.push_back(vertices.size()-1);
            }

            // straight down
            vertices.push_back(VertexData(QVector3D(0, -radius, 0),
                                          QVector3D(0, -1.0, 0)));
            indices.push_back(vertices.size()-1);
            // duplicate the last index to end the triangle strip
            indices.push_back(vertices.size()-1);
        }
        break;
    default:
    case CUBE:
        enum Face {FRONT, RIGHT, BACK, LEFT, BOTTOM, TOP};
        // TODO: implement later
        break;
    }
}
