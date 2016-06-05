#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <vector>

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

#include <QString>
#include <QTextStream>

//#include "../View/primitivedefinition.h"
#include "../global_structs.h"

#ifndef NULL
#define NULL 0
#endif

typedef QString PrimDefinition;

//struct DrawDirections {
//    QMatrix4x4 mat;
//    PrimDefinition def;
//    DrawDirections() : mat(QMatrix4x4()), def() {}
//};

//struct GlData {
//    QVector3D translation, scale;
//    QQuaternion rotation;
//    GlData() : translation(0,0,0), scale(1,1,1), rotation() {}
//    QString toString() const {
//        QString str; // = QString();
//        QTextStream ts(&str); //, QIODevice::ReadWrite);
//        ts << "Trans=(" << translation.x() << "," << translation.y() << ","
//                        << translation.z()
//           << "), Scale=(" << scale.x() << "," << scale.y() << "," << scale.z()
//           << "), Rot=(" << rotation.scalar() << "," << rotation.x() << ","
//                         << rotation.y() << "," << rotation.z() << ")";
//        return str;
//    }
//};



class GlObject {
protected:
    std::vector<GlObject*> children;
    GlObject* parent;
    GlData glData;
//    QVector3D translation, scale;
//    QQuaternion rotation;
    bool _isPrimitive;


    bool _isSelected;

    QString name;
public:


    GlObject(QString name="Default", GlObject* parent = NULL, bool isPrimitive=false)
        : parent(parent), glData(), //translation(0,0,0), scale(1,1,1), rotation(),
          _isPrimitive(isPrimitive), name(name) {}
    GlObject(const GlObject& other);

    virtual ~GlObject();

    void setParent(GlObject* newParent);
    void addChild(GlObject* newChild);
    void removeChild(GlObject* childToRemove, bool deleteChild=true);
    bool removeChild(size_t childIndex, bool deleteChild=true);
    void clearChildren();
    void reparentChildren();

    // required for use in TreeModel
    int getNumChildren() const {return children.size();}
    const QString& getName() const {return name;}
    bool setName(const QString &newName);
    GlObject *child(size_t index) { return (index >= children.size()) ?
                    NULL : children[index]; }
    GlObject *getParent() {return parent;}
    int getChildNumber() const {return (parent) ? parent->getIndexOfChild(this) : 0;}
    int getIndexOfChild(const GlObject *child) const;
//    bool insertChildren(int position, int rowCount);
//    bool removeChildren(int position, int rowCount);
    bool hasAncestor(const GlObject *other) const;


    void setTranslation(const QVector3D& t) {glData.translation=t;}
    void setScale(const QVector3D& s) {glData.scale=s;}
    void setRotation(const QQuaternion& r) {glData.rotation=r;}

    const QVector3D& getTranslation() const {return glData.translation;}
    const QVector3D& getScale() const {return glData.scale;}
    const QQuaternion& getRotation() const {return glData.rotation;}
    const GlData *getGlData() const {
//        qDebug() << "GlObject::glData is: " << glData.toString();
        return &glData;
    }
    bool isPrimitive() const {return _isPrimitive;}
    const std::vector<GlObject*>& getChildren() const { return children; }
    std::vector<GlObject*>& getChildren() { return children; }
    //const GlObject* getParent() { return parent; }

    virtual void getDrawingDirections(std::vector<DrawDirections>& dir,
                                      const DrawDirections& next,
                                      const GlObject *selected) const;

    DrawDirections makeDrawDirection(const DrawDirections& next,
                            const GlObject *selected) const;

};

class GlPrimitiveObject : public GlObject {
    PrimDefinition definition;
public:
    GlPrimitiveObject(const PrimDefinition def = "",
                      GlObject* parent = NULL)
        : GlObject(def, parent, true), definition(def) {}
    virtual ~GlPrimitiveObject() {}
    void setDef(PrimDefinition def) {definition=def;}

    const PrimDefinition getDefinition() const {return definition;}
    virtual void getDrawingDirections(std::vector<DrawDirections>& dir,
                                      const DrawDirections& next,
                                      const GlObject *selected) const;
};

#endif // GLOBJECT_H
