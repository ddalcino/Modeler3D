#ifndef TRACKBALL_H
#define TRACKBALL_H

//
// Trackball.h - ala Gavin Bell's version of Ken Shoemake's idea
//

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
#include <QPoint>
#include <QSize>
#include <QVector4D>
#include <QVector3D>
#include <QVector2D>
#include <QQuaternion>
#include <QtMath>
#include <QMatrix4x4>

class Trackball {
    // screen coords
    QPoint center;
    float     radius;

    // on unit sphere
    QVector3D dragStart;

    // Quaternions
    QQuaternion qdown;
    QQuaternion qnow;

    bool invertYAxis;

public:
    Trackball (bool invertYAxis=true, float cx=400, float cy=280, float radius=280.0 )
        : center(cx,cy),
          radius (radius),
          dragStart(),
          qdown(),
          qnow(),
          invertYAxis(invertYAxis)
    {}

    void setInvertYAxis(bool b) { invertYAxis = b; }

    void startMouse (const QPoint& pos) {
        QPoint relative = (pos - center);
        if (invertYAxis) {relative.setY(-relative.y());}
        dragStart = //mouseToSphere ( start / radius );
                mouseToSphere ( QVector2D(relative) / radius );
        qdown = qnow;
    }

    QQuaternion dragMouse (const QPoint& pos) {
        QPoint relative = (pos - center);
        if (invertYAxis) {relative.setY(-relative.y());}
        QVector3D v1 = //mouseToSphere ( QVector2D(x,y) / radius );
                mouseToSphere ( QVector2D(relative) / radius );
        QQuaternion qdrag(QVector3D::dotProduct(dragStart, v1),
                          QVector3D::crossProduct(dragStart, v1));
        qnow = qdrag.normalized() * qdown;
        qnow.normalize();
        return qnow;
    }
    void recenter(const QSize& size) {
        center.setX(size.width()/2);
        center.setY(size.height()/2);
        radius = center.x() > center.y() ? center.y() : center.x();
    }

    QQuaternion getQnow() const { return qnow; }
    void setQnow(const QQuaternion &quat) { qnow = quat; }

    /**
     * @brief getMat4
     * In Qt 5.5 and above, this function simply returns
     * "QMatrix4x4(qnow.toRotationMatrix())"
     * In Qt 5.4 and below, we need to build the matrix by hand
     * @return The matrix version of the quaternion
     */
    QMatrix4x4 getMat4() const {
        QVector4D quat = qnow.toVector4D();
        float qw = quat.w(), qx = quat.x(), qy = quat.y(), qz = quat.z();
        /* Quaternion to Matrix is:
         * 1 - 2*qy2 - 2*qz2,	2*qx*qy - 2*qz*qw,	2*qx*qz + 2*qy*qw,
         * 2*qx*qy + 2*qz*qw,	1 - 2*qx2 - 2*qz2,	2*qy*qz - 2*qx*qw,
         * 2*qx*qz - 2*qy*qw,	2*qy*qz + 2*qx*qw,	1 - 2*qx2 - 2*qy2,
         */
        return QMatrix4x4(
                    1 - 2*qy*qy - 2*qz*qz, 2*qx*qy - 2*qz*qw,	  2*qx*qz + 2*qy*qw,     0,
                    2*qx*qy + 2*qz*qw,	   1 - 2*qx*qx - 2*qz*qz, 2*qy*qz - 2*qx*qw,     0,
                    2*qx*qz - 2*qy*qw,	   2*qy*qz + 2*qx*qw,	  1 - 2*qx*qx - 2*qy*qy, 0,
                    0,                     0,                     0,                     1);
    } // {return QMatrix4x4(qnow.toRotationMatrix());}

private:
    QVector3D mouseToSphere ( QVector2D s0 ) {
        // from Gavin Bell's track ball + hyperboloid code in GLUT's trackball.c
        // s0 should be normalized
        float d,t,z;
        float r = 1.0f;

        d = s0.length(); // qSqrt(s0.x() .x() * s0.x() + s0.y() * s0.y()); //glm::length(s0);
        if ( d < r * 0.70710678118654752440 ) {
            z = sqrt ( r*r - d*d );
        } else {
            t = r / 1.41421356237309504880;
            z = t*t / d;
        }
        return QVector3D(s0, z).normalized(); // glm::normalize ( QVector3D ( s0, z ) );
    }
};

#endif // TRACKBALL_H
