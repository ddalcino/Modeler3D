#ifndef SHARED_STRUCTS_H
#define SHARED_STRUCTS_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QString>
#include <QTextStream>

typedef QString PrimDefinition;


struct DrawDirections {
    QMatrix4x4 mat, rot;
    PrimDefinition def;
//    DrawDirections() : mat(), rot(), def() {}
};

struct GlData {
    QVector3D translation, scale;
    QQuaternion rotation;
    GlData() : translation(0,0,0), scale(1,1,1), rotation() {}
    QString toString() const {
        QString str; // = QString();
        QTextStream ts(&str); //, QIODevice::ReadWrite);
        ts << "Trans=(" << translation.x() << "," << translation.y() << ","
                        << translation.z()
           << "), Scale=(" << scale.x() << "," << scale.y() << "," << scale.z()
           << "), Rot=(" << rotation.scalar() << "," << rotation.x() << ","
                         << rotation.y() << "," << rotation.z() << ")";
        return str;
    }

    enum Types { CUBE, CYLINDER, CONE, SPHERE, GRID, LINE_ARROW };

    static const char *toString(Types t) {
        switch (t) {
        case CUBE: return "Cube";
        case CYLINDER: return "Cylinder";
        case CONE: return "Cone";
        case SPHERE: return "Sphere";
        case GRID: return "Grid";
        case LINE_ARROW: return "Line Arrow";
        default: return "Default";
        }
    }
    static QString toQString(Types t) {
        return QString(toString(t));
    }
};



#endif // SHARED_STRUCTS_H
