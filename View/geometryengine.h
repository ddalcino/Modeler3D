#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H


#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QObject>

#include <map>


#include "primitivedefinition.h"

struct DrawDirections;

struct Vbos {
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    int numIndices;

    void initPrimGeometry(PrimitiveDefinition::Types t);

    Vbos(PrimitiveDefinition::Types t) : indexBuf(QOpenGLBuffer::IndexBuffer) {
        arrayBuf.create();
        indexBuf.create();
        initPrimGeometry(t);
    }
    ~Vbos() {
        arrayBuf.destroy();
        indexBuf.destroy();
    }
};


/**
 * @brief The GeometryEngine class
 * This class handles all OpenGl calls, period.
 */
class GeometryEngine : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    GeometryEngine(QObject *parent=0);
    virtual ~GeometryEngine();

    //void drawCubeGeometry(QOpenGLShaderProgram *program);

    void drawPrimGeometry(const DrawDirections &dir,
                          QOpenGLShaderProgram * program, bool isWireframeMode);

    void setScale(const QVector3D& s) {scale = s;}
    void setTranslation(const QVector3D& s) {translation = s;}
    void setRotation(const QVector3D& s, float theta) {rotation = s;
                                                       rotationAngle=theta;}
    const QVector3D& getScale() const {return scale;}
    const QVector3D& getTranslation() const {return translation;}
    const QVector3D& getRotation(float& theta) const {theta=rotationAngle;
                                                return rotation;}

    static void init();

private:
    //void initCubeGeometry();

//    void initPrimGeometry(PrimitiveDefinition::Types t);

//    QOpenGLBuffer arrayBuf;
//    QOpenGLBuffer indexBuf;

//    PrimitiveDefinition* prim;

    QVector3D translation, scale, rotation;
    float rotationAngle;

    static bool initialized;
    static std::map<QString, Vbos *> gpuData;
};

#endif // GEOMETRYENGINE_H
