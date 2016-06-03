#ifndef PERSPECTIVE3DWIDGET_H
#define PERSPECTIVE3DWIDGET_H


// QT includes
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QGraphicsSceneMouseEvent>

#include "geometryengine.h"
#include "../Model/treemodel.h"
#include "primitivedefinition.h"
#include "trackball.h"

/**
 * @brief The Perspective3DWidget class
 * A View class, used to display 3D objects using perspective projections.
 * Borrows come code from the "Cube OpenGL ES 2.0 Example" from Qt
 * documentation.
 */
class Perspective3DWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    enum MouseDragType {};

    explicit Perspective3DWidget(QWidget *parent = 0);
    ~Perspective3DWidget();

    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
//    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
//    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void initShaders();
    void setTreeModel(const TreeModel * model) {this->model = model; }
    //void initTextures();

    GeometryEngine* getGeometryEngine() {
        qDebug() << "Geometries: " << geometries;
        return geometries;}

//    void changeObject(PrimitiveDefinition::Types);

    const QVector3D& getCameraPos() const { return cameraPosition; }
    void setCameraPos(const QVector3D& pos) { cameraPosition = pos; update(); }
    float getFov() const { return fov; }
    void setFov(float f) { fov = f; resizeGL(width(), height()); update(); }

private:
    QOpenGLShaderProgram program;
    GeometryEngine *geometries;
    const TreeModel *model;

    QOpenGLTexture *texture;

    QMatrix4x4 projection;

//    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    QVector3D cameraPosition;
    float fov;
    QVector4D lightPosition;
//    qreal angularSpeed;
    QQuaternion rotation;

    Trackball trackball;

    bool isWireframeMode;

signals:

public slots:
};

#endif // PERSPECTIVE3DWIDGET_H
