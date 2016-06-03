#include "primitivedefinition.h"
#include <cmath>

#define PI 3.14159265

// TODO: Most of these primitives waste video card memory by using a lot of
// redundant normals, to simplify the drawing code. Optimize?

// TODO: The Cone normals are not normal to the plane of the triangles. This
// should be fixed.

PrimitiveDefinition::PrimitiveDefinition(GlData::Types t, int numVerticesPerCircle,
                                         float radius, float height) :
    primType(t)
{
    switch (t) {
    case GlData::CYLINDER:
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
//        // duplicate the last index to end the triangle strip
        indices.push_back(vertices.size()-1); //2*numVerticesPerCircle +1);
        indices.push_back(vertices.size()-1); //2*numVerticesPerCircle +1);

//        // add this triangle strip to the list of drawing primitives:
//        drawingPrimitives.push_back(
//                    DrawData(GL_TRIANGLE_STRIP, indices.size()-1));


        // now add the top and bottom faces, as triangle fans:
        enum {TOP, BOTTOM, LAST};
        for (int face = TOP; face != LAST; ++face) {
            //float x, z, startAngle;
            float y;
            QVector3D normal;
            switch (face) {
            case TOP:
                y = height;
                normal = QVector3D(0, 1, 0);
                break;
            case BOTTOM:
            default:
                y = 0;
                normal = QVector3D(0, -1, 0);
            }
            indices.push_back(vertices.size()-1);
            addCircularFace(numVerticesPerCircle, y, radius, normal);
            indices.push_back(vertices.size()-1);

//            // this should be a triangle strip, but that doesn't work yet
//            for (int i = 0; i <= numVerticesPerCircle; ++i) {
//                startAngle = (2*PI * i) / numVerticesPerCircle;
//                x = cos(startAngle) * radius;
//                z = sin(startAngle) * radius;
//                vertices.push_back(VertexData(QVector3D(x, y, z),
//                                              normal));
//                indices.push_back(vertices.size()-1);
//                vertices.push_back(VertexData(QVector3D(0, y, 0),
//                                              normal));
//                indices.push_back(vertices.size()-1);
//            }
//            indices.push_back(vertices.size()-1);
//            drawingPrimitives.push_back(
//                        DrawData(GL_TRIANGLE_FAN, indices.size()));
        }
        break;
    case GlData::CONE:
        // start by defining the curved surface
        for (int i = 0; i <= numVerticesPerCircle; ++i) {
            float startAngle = (2*PI * i) / numVerticesPerCircle;
            //float endAngle = (2*PI * (i+1)) / numVerticesPerCircle;
            float x = cos(startAngle) * radius;
            float z = sin(startAngle) * radius;
            float y = height;

            // Both normals should be: normal( tangent (to circle, in xz plane) cross QVector3D(-x, y, -z) )
            // But since it's a 45 degree angle, we can take the shortcut:
            QVector3D normal(x, radius, z); normal.normalize();

            vertices.push_back(VertexData(QVector3D(x, 0, z),
                                          normal));
            indices.push_back(vertices.size()-1); //2*i);
            vertices.push_back(VertexData(QVector3D(0, y, 0),
                                          normal));
            indices.push_back(vertices.size()-1); //2*i + 1);
        }
        // duplicate the last index to end the triangle strip
        indices.push_back(vertices.size()-1); //2*numVerticesPerCircle +1);
        indices.push_back(vertices.size()-1);
        addCircularFace(numVerticesPerCircle, 0, radius, QVector3D(0,-1,0));
        break;
    case GlData::SPHERE:
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
            indices.push_back(vertices.size()-1);

            for (int j = 1; j < numVerticesPerCircle/2; ++j) {
                float sliceRadius = sin((PI * j) / (numVerticesPerCircle/2));
                float height = cos((PI *j) / (numVerticesPerCircle/2));
                QVector3D first(cosTheta[0]*sliceRadius, height, sinTheta[0]*sliceRadius);
                QVector3D second(cosTheta[1]*sliceRadius, height, sinTheta[1]*sliceRadius);
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
    case GlData::GRID:
        static const float gridDensity = 0.25f;
        static const int numRows = 32;
        static const int numHorizontalRepeats = numRows >> 1;
        // horizontal rows
        for (int row = 0; row < numRows; ++row) {
            // start a new segment
            //if (row>0) {
            //indices.push_back(vertices.size());
            for (int i = 0; i < numHorizontalRepeats; ++i) {
                // Line strip: draw up, right, down, right
                vertices.push_back(VertexData(QVector3D(gridDensity*2*i, gridDensity*row, 0.0f), QVector3D()));
                indices.push_back(vertices.size()-1);
                vertices.push_back(VertexData(QVector3D(gridDensity*2*i, gridDensity*(row+1), 0.0f), QVector3D()));
                indices.push_back(vertices.size()-1);
                vertices.push_back(VertexData(QVector3D(gridDensity*(2*i+1), gridDensity*(row+1), 0.0f), QVector3D()));
                indices.push_back(vertices.size()-1);
                vertices.push_back(VertexData(QVector3D(gridDensity*(2*i+1), gridDensity*row, 0.0f), QVector3D()));
                indices.push_back(vertices.size()-1);
            }
            // end the segment
            vertices.push_back(VertexData(QVector3D(gridDensity*(2*numHorizontalRepeats), gridDensity*row, 0.0f), QVector3D()));
            indices.push_back(vertices.size()-1);
            vertices.push_back(VertexData(QVector3D(gridDensity*(2*numHorizontalRepeats), gridDensity*(row+1), 0.0f), QVector3D()));
            indices.push_back(vertices.size()-1);
        }
        break;
    case GlData::LINE_ARROW:
        static const float arrowLength = 0.75f;
        static const float tipHalfWidth = 0.25f;

        vertices.push_back(VertexData(QVector3D(0.0f, 0.0f, 0.0f), QVector3D()));
        vertices.push_back(VertexData(QVector3D(arrowLength, 0.0f, 0.0f), QVector3D()));
        vertices.push_back(VertexData(QVector3D(arrowLength-tipHalfWidth, tipHalfWidth, 0.0f), QVector3D()));
        vertices.push_back(VertexData(QVector3D(arrowLength-tipHalfWidth, -tipHalfWidth, 0.0f), QVector3D()));
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(1);
        indices.push_back(3);

        break;
    default:
    case GlData::CUBE:
            // For cube we would need only 8 vertices but we have to
            // duplicate vertex for each face because texture coordinate
            // is different.
//            _VertexData vertices[] = {
                // Vertex data for face 0
                vertices.push_back(VertexData(QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
                vertices.push_back(VertexData(QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
                vertices.push_back(VertexData(QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f)));
                vertices.push_back(VertexData(QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f)));

                // Vertex data for face 1
                vertices.push_back(VertexData(QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f)));

                // Vertex data for face 2
                vertices.push_back(VertexData(QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
                vertices.push_back(VertexData(QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
                vertices.push_back(VertexData(QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f)));
                vertices.push_back(VertexData(QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f)));

                // Vertex data for face 3
                vertices.push_back(VertexData(QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(-1.0f, 0.0f, 0.0f)));

                // Vertex data for face 4
                vertices.push_back(VertexData(QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(0.0f, -1.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(0.0f, -1.0f, 0.0f)));

                // Vertex data for face 5
                vertices.push_back(VertexData(QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
                vertices.push_back(VertexData(QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f)));
//            };

            // Indices for drawing cube faces using triangle strips.
            // Triangle strips can be connected by duplicating indices
            // between the strips. If connecting strips have opposite
            // vertex order then last index of the first strip and first
            // index of the second strip needs to be duplicated. If
            // connecting strips have same vertex order then only last
            // index of the first strip needs to be duplicated.
            GLushort indices[] = {
                 0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
                 4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
                 8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
                12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
                16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
                20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
            };
            for (int i = 0; i < 34; ++i) {
                this->indices.push_back(indices[i]);
            }
        break;
    }
}

void PrimitiveDefinition::addCircularFace(unsigned int numVerticesPerCircle,
                                          float y,
                                          float radius, const QVector3D &normal)
{
    // this will implement an endless loop if numVerticesPerCircle == UINT32_MAX
    Q_ASSERT(numVerticesPerCircle < UINT32_MAX);

    bool isFirst = true;
    float startAngle, x, z;
    // this should be a triangle strip, but that doesn't work yet
    for (unsigned int i = 0; i <= numVerticesPerCircle; ++i) {
        startAngle = (2*PI * i) / numVerticesPerCircle;
        x = cos(startAngle) * radius;
        z = sin(startAngle) * radius;
        vertices.push_back(VertexData(QVector3D(x, y, z),
                                      normal));
        indices.push_back(vertices.size()-1);
        // duplicate the first index
        if (isFirst) { indices.push_back(vertices.size()-1); isFirst=false; }
        vertices.push_back(VertexData(QVector3D(0, y, 0),
                                      normal));
        indices.push_back(vertices.size()-1);
    }
    //indices.push_back(vertices.size()-1);

}
