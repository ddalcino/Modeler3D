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

#include "geometryengine.h"
#include "primitivedefinition.h"

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
    explicit Perspective3DWidget(QWidget *parent = 0);
    ~Perspective3DWidget();

    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

    void initShaders();
    //void initTextures();


private:
    QOpenGLShaderProgram program;
    GeometryEngine *geometries;

    QOpenGLTexture *texture;

    QMatrix4x4 projection;

//    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    QVector3D cameraPosition;
    QVector4D lightPosition;
//    qreal angularSpeed;
    QQuaternion rotation;


signals:

public slots:
};

#endif // PERSPECTIVE3DWIDGET_H
