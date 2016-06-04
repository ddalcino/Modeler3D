#ifndef GLOBAL_STRUCTS_H
#define GLOBAL_STRUCTS_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QString>
#include <QTextStream>

typedef QString PrimDefinition;

#define NUM_PRIMITIVE_TYPES 6


struct DrawDirections {
    QMatrix4x4 mat, rot;
    PrimDefinition def;
//    DrawDirections() : mat(), rot(), def() {}
};

struct GlData {
    QVector3D translation, scale;
    QQuaternion rotation;
    GlData();
    QString toString() const;
};

struct PrimTypes {
    enum Types { CUBE, CYLINDER, CONE, SPHERE, GRID, LINE_ARROW };
    static const char *primTypeNames[NUM_PRIMITIVE_TYPES];

    static const char *toString(Types t);
    static QString toQString(Types t);

    static bool isInPrimTypeNames(QString name);
};

#endif // GLOBAL_STRUCTS_H
