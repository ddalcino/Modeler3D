#include "perspective3dwidget.h"
#include "perspectivewindow.h"
#include <QMouseEvent>
#include <QDebug>
#include <QPoint>

//#include "../Model/globject.h" // for DrawingDirections
#include "../global_structs.h" // for DrawingDirections

Perspective3DWidget::Perspective3DWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    texture(NULL),
    rotationAxis(0, 0, 0),
    cameraPosition(0, 0, -5),
    fov(45.0),
    lightPosition(-5, 5, -5, 1.0),
    trackballCam(true),
    trackballObj(true, &trackballCam),
    isWireframeMode(false),
    showGrid(true),
    showAxes(false),
    dragType(RotCam)
{
//    qDebug() << "Perspective3DWidget constructor called";
}

Perspective3DWidget::~Perspective3DWidget()
{
    if (texture) {
        delete texture;
        texture = NULL;
    }
}

void Perspective3DWidget::mouseMoveEvent(QMouseEvent *e) {
    if (e->buttons() & Qt::LeftButton) {
        switch (dragType) {
        case RotCam:
            trackballCam.dragMouse(e->pos());
            break;
        case RotObj:
            trackballObj.dragMouse(e->pos());
            // now copy qnow to the selected object
            ((PerspectiveWindow *)(this->parent()->parent()))->getTvWindow()
                    ->setRotationAtSel(trackballObj.getQnow());
            // we should actually apply this in a way that takes the ancestor
            // rotations into account; as it is, this should only work on top
            // level objects
            break;
        default:
            break;
        }
        update();
    }
}

void Perspective3DWidget::mousePressEvent(QMouseEvent *e) {

    if (e->buttons() & Qt::LeftButton) {
        switch (dragType) {
        case RotCam:
            trackballCam.startMouse(e->pos());
            break;
        case RotObj:
            trackballObj.startMouse(e->pos());
            break;
        default:
            break;
        }
    }
}

//void Perspective3DWidget::mouseReleaseEvent(QMouseEvent *e) {
//    qDebug() << "Buttons released: " << e->buttons();
//    rotationAxis.setX(0);
//    rotationAxis.setY(0);
//    rotationAxis.setZ(0);
//}

//void Perspective3DWidget::keyPressEvent(QKeyEvent *e)
//{
//    float scale = 0.0;
//    rotationAxis.setX(0);
//    rotationAxis.setY(0);
//    rotationAxis.setZ(0);
//    if (e->key() == Qt::Key_Left) {
//        rotationAxis.setY(1); scale = -1.0;
//    } else if (e->key() == Qt::Key_Right) {
//        rotationAxis.setY(1); scale = 1.0;
//    } else if (e->key() == Qt::Key_Up) {
//        rotationAxis.setX(1); scale = 1.0;
//    } else if (e->key() == Qt::Key_Down) {
//        rotationAxis.setX(1); scale = -1.0;
//    }
//    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, scale*6) * rotation;
//    qDebug() << "Buttons pressed: " << e->key() << ", rotationAxis: "
//             << rotationAxis << ", rotation: " << rotation;
//    update();

//}


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


    geometries = new GeometryEngine(this);

    //geometries = ((PerspectiveWindow *)(this->parent()))->getTvWindow()->getGEngine(); //  gEngine; //new GeometryEngine(PrimitiveDefinition::CUBE);

    //geometries->init();
    qDebug() << "got geometry engine";
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

void Perspective3DWidget::setTreeModel(const TreeModel *model) {this->model = model; }

GeometryEngine *Perspective3DWidget::getGeometryEngine() {
    qDebug() << "Geometries: " << geometries;
    return geometries;}

const QVector3D &Perspective3DWidget::getCameraPos() const { return cameraPosition; }

void Perspective3DWidget::setCameraPos(const QVector3D &pos) { cameraPosition = pos; update(); }

float Perspective3DWidget::getFov() const { return fov; }

void Perspective3DWidget::setFov(float f) { fov = f; resizeGL(width(), height()); update(); }

void Perspective3DWidget::setWireframeMode(bool b) { isWireframeMode = b; }

void Perspective3DWidget::setShowGrid(bool b) { showGrid = b; }

void Perspective3DWidget::setShowAxes(bool b) { showAxes = b; }

void Perspective3DWidget::setMouseDragType(Perspective3DWidget::MouseDragType t) { dragType = t;  qDebug() << "Set mousedragtype to " << t;}

Perspective3DWidget::MouseDragType Perspective3DWidget::getMouseDragType() const { return dragType; }

//void Perspective3DWidget::changeObject(PrimitiveDefinition::Types t)
//{
////    delete geometries;
////    geometries = new GeometryEngine(t);
//}

void Perspective3DWidget::resizeGL(int w, int h)
{
    qDebug() << "resizeGL(" << w << "," << h << ") called";
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    static const qreal zNear = 0.1, zFar = 10.0; //, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);

    this->resize(w, h);
    trackballCam.recenter(this->size());
}

void Perspective3DWidget::paintGL()
{
//    qDebug() << "paintGL() called";
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (texture) { texture->bind(); }

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(cameraPosition);
    matrix.rotate(trackballCam.getQnow());
    //matrix.rotate(rotation);

    // Set modelview and projection matrices
    program.setUniformValue("mProjection", projection);
    program.setUniformValue("mView", matrix);
//    qDebug() << "mView matrix:" << matrix;

    program.setUniformValue("LightPosition", lightPosition);
    program.setUniformValue("mvp_matrix", projection * matrix);

//    // Use texture unit 0 which contains nothing, yet
//    program.setUniformValue("vertex_color", QVector3D(0.2, 0.6, 0.8));

    // Set Ambient, Diffuse, Specular products, and Shininess for Phong-Blinn shader
    program.setUniformValue("AmbientProduct", QVector4D( 0.2, 0.2, 0.2, 1.0 ));
    program.setUniformValue("DiffuseProduct", QVector4D( 0.7, 0.7, 0.7, 1.0 ));
    program.setUniformValue("SpecularProduct", QVector4D( 0.1, 0.1, 0.1, 1.0 ));
    program.setUniformValue("Shininess", 200.0f);


    //figure out the address of the selected GlObject
    const GlObject *selected = ((PerspectiveWindow *)(this->parent()->parent()))
            ->getTvWindow()->getSelectedObject();


    // get list of directions
    //const GlObject *root = model->getRoot();
    std::vector<DrawDirections> dirs;
    DrawDirections next;
    if (model) { model->getDrawingDirections(dirs, next, selected); }

    if (showGrid) {
        program.setUniformValue("wireframe_color", QVector4D(0.2f, 1.0f, 1.0f, 1.0f));
        geometries->drawGrid(&program);
    }

    DrawDirections axes;
    for (const DrawDirections &dir : dirs) {
        // save axes for drawing in later
        if (dir.isAxesOnly && dir.isSelected) {
            axes = dir;
        } else {
            // Draw geometry
            program.setUniformValue("add_color",
                                    (dir.isSelected || dir.isParentSelected) ?
                                        QVector4D(0.0f, 0.3f, 0.3f, 0.0f) :
                                        QVector4D());

            if (isWireframeMode) {
                program.setUniformValue("wireframe_color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
                geometries->drawPrimGeometry(dir, &program, isWireframeMode);
            } else {
                program.setUniformValue("wireframe_color", QVector4D());
                geometries->drawPrimGeometry(dir, &program, isWireframeMode);
            }
            if (dir.isSelected) {
                axes = dir;
            }
        }
    }
    if (showAxes && axes.isSelected) {
        // Disable depth buffer
        glDisable(GL_DEPTH_TEST);

        // Draw axes on top of objects
        geometries->drawAxes(axes, &program);

        // Enable depth buffer
        glEnable(GL_DEPTH_TEST);
    }


}

