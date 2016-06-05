#ifndef GLOBAL_STRUCTS_H
#define GLOBAL_STRUCTS_H

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QString>
#include <QTextStream>

typedef QString PrimDefinition;

#define NUM_PRIMITIVE_TYPES 6

struct GlData {
    QVector3D translation, scale;
    QQuaternion rotation;
    GlData();
    QString toString() const;
    void apply(const GlData *other);
};

struct DrawDirections {
    GlData glData;
    QMatrix4x4 matModel;
    PrimDefinition def;
    bool isSelected=false;
    bool isParentSelected=false;
    bool isAxesOnly=false;
//    DrawDirections() : mat(), rot(), def() {}
};


struct PrimTypes {
    enum Types { CUBE, CYLINDER, CONE, SPHERE, GRID, LINE_ARROW };
    static const char *primTypeNames[NUM_PRIMITIVE_TYPES];

    static const char *toString(Types t);
    static QString toQString(Types t);

    static bool isInPrimTypeNames(QString name);
};

#endif // GLOBAL_STRUCTS_H
