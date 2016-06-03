#include "geometryengine.h"
//#include "../Model/globject.h" // for DrawDirections
#include "../shared_structs.h" // for DrawDirections
#include "primitivedefinition.h"
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <QDebug>

bool GeometryEngine::initialized = false;
std::map<QString, Vbos *> GeometryEngine::gpuData;

struct _VertexData
{
    QVector3D position;
    QVector3D normal;
};

GeometryEngine::GeometryEngine(QObject *parent)
    : //indexBuf(QOpenGLBuffer::IndexBuffer),
      QObject(parent),
      translation(0,0,0),
      scale(1,1,1),
      rotation(0,0,0),
      rotationAngle(0.0)
{
    initializeOpenGLFunctions();

    init();

//    // Generate 2 VBOs
//    arrayBuf.create();
//    indexBuf.create();


    // Initializes cube geometry and transfers it to VBOs
    //initCubeGeometry();

//    initPrimGeometry(t);
}

GeometryEngine::~GeometryEngine()
{
//    arrayBuf.destroy();
//    indexBuf.destroy();

//    if (prim) { delete prim; }
}

/*
//void GeometryEngine::initCubeGeometry()
//{
//    // For cube we would need only 8 vertices but we have to
//    // duplicate vertex for each face because texture coordinate
//    // is different.
//    _VertexData vertices[] = {
//        // Vertex data for face 0
//        {QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f)},  // v0
//        {QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f)}, // v1
//        {QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f)},  // v2
//        {QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(0.0f, 0.0f, 1.0f)}, // v3

//        // Vertex data for face 1
//        {QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f)}, // v4
//        {QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f)}, // v5
//        {QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(1.0f, 0.0f, 0.0f)},  // v6
//        {QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(1.0f, 0.0f, 0.0f)}, // v7

//        // Vertex data for face 2
//        {QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f)}, // v8
//        {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f)},  // v9
//        {QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f)}, // v10
//        {QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(0.0f, 0.0f, -1.0f)},  // v11

//        // Vertex data for face 3
//        {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(-1.0f, 0.0f, 0.0f)}, // v12
//        {QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(-1.0f, 0.0f, 0.0f)},  // v13
//        {QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(-1.0f, 0.0f, 0.0f)}, // v14
//        {QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(-1.0f, 0.0f, 0.0f)},  // v15

//        // Vertex data for face 4
//        {QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(0.0f, -1.0f, 0.0f)}, // v16
//        {QVector3D( 1.0f, -1.0f, -1.0f), QVector3D(0.0f, -1.0f, 0.0f)}, // v17
//        {QVector3D(-1.0f, -1.0f,  1.0f), QVector3D(0.0f, -1.0f, 0.0f)}, // v18
//        {QVector3D( 1.0f, -1.0f,  1.0f), QVector3D(0.0f, -1.0f, 0.0f)}, // v19

//        // Vertex data for face 5
//        {QVector3D(-1.0f,  1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f)}, // v20
//        {QVector3D( 1.0f,  1.0f,  1.0f), QVector3D(0.0f, 1.0f, 0.0f)}, // v21
//        {QVector3D(-1.0f,  1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f)}, // v22
//        {QVector3D( 1.0f,  1.0f, -1.0f), QVector3D(0.0f, 1.0f, 0.0f)}  // v23
//    };

//    // Indices for drawing cube faces using triangle strips.
//    // Triangle strips can be connected by duplicating indices
//    // between the strips. If connecting strips have opposite
//    // vertex order then last index of the first strip and first
//    // index of the second strip needs to be duplicated. If
//    // connecting strips have same vertex order then only last
//    // index of the first strip needs to be duplicated.
//    GLushort indices[] = {
//         0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
//         4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
//         8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
//        12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
//        16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
//        20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
//    };

//    // Transfer vertex data to VBO 0
//    arrayBuf.bind();
//    arrayBuf.allocate(vertices, 24 * sizeof(_VertexData));

//    // Transfer index data to VBO 1
//    indexBuf.bind();
//    indexBuf.allocate(indices, 34 * sizeof(GLushort));
//}
*/

void Vbos::initPrimGeometry(GlData::Types t)
{
    PrimitiveDefinition prim(t);

    arrayBuf.bind();
    arrayBuf.allocate(prim.getVertices(), prim.getNumVertices() * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(prim.getIndices(), prim.getNumIndices() * sizeof(GLushort));

    numIndices = prim.getNumIndices();
}

void GeometryEngine::drawPrimGeometry(const DrawDirections &dir,
                                      QOpenGLShaderProgram *program,
                                      bool isWireframeMode)
{

    // Tell OpenGL which VBOs to use
    try {
        Vbos *vbo = gpuData.at(dir.def);
        vbo->arrayBuf.bind();
        vbo->indexBuf.bind();

        //    arrayBuf.bind();
        //    indexBuf.bind();

        // Offset for position
        quintptr offset = 0;

        // Tell OpenGL programmable pipeline how to locate vertex position data
        int vertexLocation = program->attributeLocation("a_position");
        program->enableAttributeArray(vertexLocation);
        program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(_VertexData));

        // Offset for texture coordinate
        offset += sizeof(QVector3D);

        // Tell OpenGL programmable pipeline how to locate vertex normal coordinate data
        int normalLocation = program->attributeLocation("a_normal");
        program->enableAttributeArray(normalLocation);
        program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, sizeof(_VertexData));

        const QMatrix4x4 &mat = dir.mat;
//        mat.setToIdentity();
//        mat.rotate(rotationAngle, rotation);
//        mat.translate(translation);
//        mat.scale(scale);
//        qDebug() << "Model matrix:" << mat;

        program->setUniformValue("mModel", mat);
        // Draw cube geometry using indices from VBO 1
        glDrawElements(isWireframeMode ? GL_LINE_STRIP : GL_TRIANGLE_STRIP,
                       vbo->numIndices, GL_UNSIGNED_SHORT, 0);
    } catch(const std::out_of_range &ex) {
        qDebug() << "Tried to draw an object of type " << dir.def
                 << ", but none was found in my gpuData. Error=" << ex.what();
    }
}

void GeometryEngine::drawGrid(QOpenGLShaderProgram *program) {
    Vbos *vbo = gpuData.at("Grid");
    vbo->arrayBuf.bind();
    vbo->indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(_VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex normal coordinate data
    int normalLocation = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, sizeof(_VertexData));

    QMatrix4x4 matRot, matPos;

    for (int i = 0; i < 3; ++i) {
        //matRot.rotate(90.0f, 1.0f, 0.0f, 0.0f);
        program->setUniformValue("mModel", matRot * matPos);
        glDrawElements(GL_LINE_STRIP, vbo->numIndices, GL_UNSIGNED_SHORT, 0);
        matPos.translate(0.0f, -8.0f, 0.0f);
        program->setUniformValue("mModel", matRot * matPos);
        glDrawElements(GL_LINE_STRIP, vbo->numIndices, GL_UNSIGNED_SHORT, 0);
        matPos.translate(-8.0f, 8.0f, 0.0f);
        program->setUniformValue("mModel", matRot * matPos);
        glDrawElements(GL_LINE_STRIP, vbo->numIndices, GL_UNSIGNED_SHORT, 0);
        matPos.translate(0.0f, -8.0f, 0.0f);
        program->setUniformValue("mModel", matRot * matPos);
        glDrawElements(GL_LINE_STRIP, vbo->numIndices, GL_UNSIGNED_SHORT, 0);
        matPos.translate(8.0f, 8.0f, 0.0f);


        if (i == 0) {
            matRot.rotate(90.0f, 0.0f, 1.0f, 0.0f);
        } else if (i == 1) {
            matRot.rotate(-90.0f, 0.0f, 1.0f, 0.0f);
            matRot.rotate(90.0f, 1.0f, 0.0f, 0.0f);
        }
    }
}

void GeometryEngine::drawAxes(const DrawDirections &dir,
                              QOpenGLShaderProgram *program) {
    Vbos *vbo = gpuData.at("Line Arrow");
    vbo->arrayBuf.bind();
    vbo->indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(_VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex normal coordinate data
    int normalLocation = program->attributeLocation("a_normal");
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, sizeof(_VertexData));

    // the X axis
    program->setUniformValue("mModel", dir.rot);
    program->setUniformValue("wireframe_color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
    glDrawElements(GL_LINE_STRIP, vbo->numIndices, GL_UNSIGNED_SHORT, 0);

    // the Y axis
    QMatrix4x4 matRot;
    matRot.rotate(90.0f, 0,0,1);
    program->setUniformValue("mModel", matRot * dir.rot);
    program->setUniformValue("wireframe_color", QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
    glDrawElements(GL_LINE_STRIP, vbo->numIndices, GL_UNSIGNED_SHORT, 0);

    // the Z axis
    matRot.setToIdentity();
    matRot.rotate(90.0f, 0,1,0);
    program->setUniformValue("mModel", matRot * dir.rot);
    program->setUniformValue("wireframe_color", QVector4D(0.0f, 0.0f, 1.0f, 1.0f));
    glDrawElements(GL_LINE_STRIP, vbo->numIndices, GL_UNSIGNED_SHORT, 0);

}

void GeometryEngine::init()
{
    if (!initialized) {
        //initializeOpenGLFunctions();

        // set up gpuData to have references to all the geometry we could possibly need
        gpuData.insert(std::pair<QString, Vbos *>(QString("Cube"), new Vbos(GlData::CUBE) ));
        gpuData.insert(std::pair<QString, Vbos *>(QString("Cylinder"), new Vbos(GlData::CYLINDER) ));
        gpuData.insert(std::pair<QString, Vbos *>(QString("Sphere"), new Vbos(GlData::SPHERE) ));
        gpuData.insert(std::pair<QString, Vbos *>(QString("Cone"), new Vbos(GlData::CONE) ));
        gpuData.insert(std::pair<QString, Vbos *>(QString("Grid"), new Vbos(GlData::GRID) ));
        gpuData.insert(std::pair<QString, Vbos *>(QString("Line Arrow"), new Vbos(GlData::LINE_ARROW) ));
    }
    initialized = true;
}
