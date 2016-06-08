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
#include "trackball.h"

class TreeViewWindow;

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
    enum MouseDragType {RotCam, RotObj,
                        TrCamXY, TrCamYZ, TrCamXZ,
                        TrObjXY, TrObjYZ, TrObjXZ,
                        ScaleObjXY, ScaleObjYZ, ScaleObjXZ };

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
//    void setTreeModel(const TreeModel * model);
    //void initTextures();

    GeometryEngine* getGeometryEngine();

//    void changeObject(PrimitiveDefinition::Types);

    const QVector3D& getCameraPos() const;
    void setCameraPos(const QVector3D& pos);
    void setCameraRot(const QQuaternion& rot) { trackballCam.setQnow(rot);}
    float getFov() const;
    void setFov(float f);
    void setWireframeMode(bool b);
    void setShowGrid(bool b);
    void setShowAxes(bool b);
    void setMouseDragType(MouseDragType t);
    MouseDragType getMouseDragType() const;

    float getNearDrawDistance() const;
    void setNearDrawDistance(float value);

    float getFarDrawDistance() const;
    void setFarDrawDistance(float value);

private:
    QOpenGLShaderProgram program;
    //QOpenGLShaderProgram wireProgram;
    GeometryEngine *geometries;
    TreeViewWindow *tvWindow;
//    const TreeModel *model;

    QOpenGLTexture *texture;

    QMatrix4x4 projection;

//    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    QVector3D cameraPosition;
    float fov;
    QVector4D lightPosition;
//    qreal angularSpeed;
    QQuaternion rotation;

    Trackball trackballCam;
    Trackball trackballObj;

    bool isWireframeMode;
    bool showGrid;
    bool showAxes;
    float nearDrawDistance, farDrawDistance;

    MouseDragType dragType;
signals:

public slots:
};

#endif // PERSPECTIVE3DWIDGET_H
