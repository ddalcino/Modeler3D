#include "perspective3dwidget.h"
#include <QMouseEvent>
#include <QDebug>

Perspective3DWidget::Perspective3DWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    texture(NULL),
    rotationAxis(0, 0, 0),
    cameraPosition(0, 0, -5),
    lightPosition(-5, 5, -5, 1.0)
{
    qDebug() << "Perspective3DWidget constructor called";
}

Perspective3DWidget::~Perspective3DWidget()
{
    if (texture) {
        delete texture;
        texture = NULL;
    }
}

void Perspective3DWidget::mousePressEvent(QMouseEvent *e) {
//    qDebug() << "Buttons pressed: " << e->buttons();
//    if (e->buttons() & Qt::LeftButton) {
//        rotation.setX(rotation.x() + 30.0);
//    }
//    if (e->buttons() & Qt::RightButton) {
//        rotation.setY(rotation.y() + 30.0);
//    }
//    if (e->buttons() & Qt::MiddleButton) {
//        rotation.setZ(rotation.z() + 30.0);
//    }
//    static const float MULTIPLIER = 6.0;
    rotationAxis.setX(/*rotationAxis.x() +
                      MULTIPLIER */ (e->buttons() & Qt::LeftButton) ? 1 : 0);
    rotationAxis.setY(/*rotationAxis.y() +
                      MULTIPLIER */ (e->buttons() & Qt::RightButton) ? 1 : 0);
    rotationAxis.setZ(/*rotationAxis.z() +
                      MULTIPLIER */ (e->buttons() & Qt::MiddleButton) ? 1 : 0);

    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, 20.0) * rotation;
    qDebug() << "Buttons pressed: " << e->buttons() << ", rotationAxis: "
             << rotationAxis << ", rotation: " << rotation;

    update();
}

void Perspective3DWidget::mouseReleaseEvent(QMouseEvent *e) {
    qDebug() << "Buttons released: " << e->buttons();
    rotationAxis.setX(0);
    rotationAxis.setY(0);
    rotationAxis.setZ(0);
}

void Perspective3DWidget::keyPressEvent(QKeyEvent *e)
{
    float scale = 0.0;
    rotationAxis.setX(0);
    rotationAxis.setY(0);
    rotationAxis.setZ(0);
    if (e->key() == Qt::Key_Left) {
        rotationAxis.setY(1); scale = -1.0;
    } else if (e->key() == Qt::Key_Right) {
        rotationAxis.setY(1); scale = 1.0;
    } else if (e->key() == Qt::Key_Up) {
        rotationAxis.setX(1); scale = 1.0;
    } else if (e->key() == Qt::Key_Down) {
        rotationAxis.setX(1); scale = -1.0;
    }
    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, scale*6) * rotation;
    qDebug() << "Buttons pressed: " << e->key() << ", rotationAxis: "
             << rotationAxis << ", rotation: " << rotation;
    update();

}


void Perspective3DWidget::initializeGL()
{
    qDebug() << "initializeGL() called";
    initializeOpenGLFunctions();

    glClearColor(0.1, 0.1, 0.1, 1);

    initShaders();
    //initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);


    //geometries = new GeometryEngine;
    geometries = new GeometryEngine(PrimitiveDefinition::CUBE);
}

void Perspective3DWidget::initShaders()
{
    qDebug() << "initShaders() called";
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshaderbasic.glsl")){
        qDebug() << "failed to add vertex shader";
        close();
    }

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshaderbasic.glsl")){
        qDebug() << "failed to add fragment shader";
        close();
    }

    // Link shader pipeline
    if (!program.link()) {
        qDebug() << "failed to link shaders";
        close();
    }

    // Bind shader pipeline for use
    if (!program.bind()) {
        qDebug() << "failed to bind shaders";
        close();
    }
    qDebug() << "initShaders() finished successfully";
}

void Perspective3DWidget::resizeGL(int w, int h)
{
    qDebug() << "resizeGL(" << w << "," << h << ") called";
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    static const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void Perspective3DWidget::paintGL()
{
    qDebug() << "paintGL() called";
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (texture) { texture->bind(); }

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(cameraPosition);
    matrix.rotate(rotation);

//    // Set modelview and projection matrix
    program.setUniformValue("Projection", projection);
    program.setUniformValue("ModelView", matrix);
    program.setUniformValue("LightPosition", lightPosition);
    program.setUniformValue("mvp_matrix", projection * matrix);

//    // Use texture unit 0 which contains nothing, yet
//    program.setUniformValue("vertex_color", QVector3D(0.2, 0.6, 0.8));

//    // Set Ambient, Diffuse, Specular products, and Shininess for Phong-Blinn shader
    program.setUniformValue("AmbientProduct", QVector4D( 0.2, 0.2, 0.2, 1.0 ));
    program.setUniformValue("DiffuseProduct", QVector4D( 0.7, 0.7, 0.7, 1.0 ));
    program.setUniformValue("SpecularProduct", QVector4D( 0.1, 0.1, 0.1, 1.0 ));
    program.setUniformValue("Shininess", 200.0f);


    // Draw cube geometry
    geometries->drawPrimGeometry(&program);
}

