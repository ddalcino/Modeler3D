#include "global_structs.h"

const char* PrimTypes::primTypeNames[NUM_PRIMITIVE_TYPES]  = {"Cube", "Cylinder", "Cone", "Sphere",
                                        "Grid", "Line Arrow"};

GlData::GlData() : translation(0,0,0), scale(1,1,1), rotation() {}

QString GlData::toString() const {
    QString str; // = QString();
    QTextStream ts(&str); //, QIODevice::ReadWrite);
    ts << "Trans=(" << translation.x() << "," << translation.y() << ","
       << translation.z()
       << "), Scale=(" << scale.x() << "," << scale.y() << "," << scale.z()
       << "), Rot=(" << rotation.scalar() << "," << rotation.x() << ","
       << rotation.y() << "," << rotation.z() << ")";
    return str;
}

void GlData::apply(const GlData* other) {
    if (other) {
        rotation = other->rotation * rotation;
        rotation.normalize();
        translation += other->translation;
        for (int i = 0; i < 3; ++i) {
            scale[i] *= other->scale[i];
        }
    }
}


const char *PrimTypes::toString(PrimTypes::Types t) {
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

QString PrimTypes::toQString(PrimTypes::Types t) {
    return QString(toString(t));
}

bool PrimTypes::isInPrimTypeNames(QString name) {
    for (int i = 0; i < NUM_PRIMITIVE_TYPES; ++i) {
        if (name == primTypeNames[i]) { return true; }
    }
    return false;
}
