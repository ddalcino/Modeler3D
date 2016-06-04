#include <QFile>
#include <QVector3D>
#include <QQuaternion>
#include <QStringRef>
#include <QTextStream>

#include "scenexmlhandler.h"
#include "globject.h"
#include "../global_structs.h"

const char* SceneGraphXMLHandler::ELEMENT_SCENEGRAPH_ROOT = "SceneGraphXMLRoot";
const char* SceneGraphXMLHandler::ELEMENT_GLOBJECT = "GlObject";
const char* SceneGraphXMLHandler::ATTR_IS_PRIMITIVE = "isPrim";
const char* SceneGraphXMLHandler::ATTR_NAME = "name";
const char* SceneGraphXMLHandler::ATTR_TRANSLATION = "pos";
const char* SceneGraphXMLHandler::ATTR_SCALE = "scale";
const char* SceneGraphXMLHandler::ATTR_ROTATION = "rot";
const char* SceneGraphXMLHandler::ATTR_DEFINITION = "def";

const char* SceneGraphXMLHandler::VAL_TRUE = "true";
const char* SceneGraphXMLHandler::VAL_FALSE = "false";


SceneGraphXMLHandler::SceneGraphXMLHandler(GlObject *root)
    : root(root) {
    if (!root) {
        root = new GlObject("Root", NULL, false);
//        throw "SceneGraphXMLHandler(): ctor needs a root!";
    }
}

bool SceneGraphXMLHandler::readFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Error: Cannot read file " << fileName << ": "
                 << file.errorString();
        return false;
    }
    reader.setDevice(&file);

    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isStartElement()) {
            if (reader.name() == ELEMENT_SCENEGRAPH_ROOT) {
                readSceneGraphRootElement();
            } else {
                reader.raiseError(QObject::tr("Not a SceneGraphXML file"));
            }
        } else {
            reader.readNext();
        }
    }

    file.close();
    if (reader.hasError()) {
        qDebug() << "Error: Failed to parse file " << fileName << ": "
                 << reader.errorString();
        return false;
    } else if (file.error() != QFile::NoError) {
        qDebug() << "Error: Cannot read file " << fileName << ": "
                 << file.errorString();
        return false;
    }
    return true;
}

GlObject *SceneGraphXMLHandler::getRoot() const {
    return root;
}

bool SceneGraphXMLHandler::writeXml(const QString &fileName,
                                    const GlObject *root) {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "Error: Cannot write file " << fileName << ": "
                 << file.errorString();
        return false;
    }

    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement(ELEMENT_SCENEGRAPH_ROOT);

    for (const GlObject* child : root->getChildren()) {
        writeGlObject(&xmlWriter, child);
    }
    xmlWriter.writeEndDocument(); // closes any open tags
    file.close();
    if (file.error()) {
        qDebug() << "Error: Cannot write file " << fileName << ": "
                 << file.errorString();
        return false;
    }
    return true;
}

void SceneGraphXMLHandler::writeGlObject(QXmlStreamWriter *xmlWriter,
                                         const GlObject *node) {
    xmlWriter->writeStartElement(ELEMENT_GLOBJECT);
    xmlWriter->writeAttribute(ATTR_NAME, node->getName());
    xmlWriter->writeAttribute(ATTR_IS_PRIMITIVE,
                              node->isPrimitive() ? VAL_TRUE : VAL_FALSE );
    if (node->isPrimitive()) {
        xmlWriter->writeAttribute(ATTR_DEFINITION,
                                  ((GlPrimitiveObject*)node)->getDefinition());
    }
    xmlWriter->writeAttribute(ATTR_TRANSLATION,
                              qVec3ToString(node->getGlData()->translation) );
    xmlWriter->writeAttribute(ATTR_SCALE,
                              qVec3ToString(node->getGlData()->scale) );
    xmlWriter->writeAttribute(ATTR_ROTATION,
                              qQuatToString(node->getGlData()->rotation) );

    for (const GlObject* child : node->getChildren()) {
        writeGlObject(xmlWriter, child);
    }
    xmlWriter->writeEndElement();
}

void SceneGraphXMLHandler::readSceneGraphRootElement() {
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartDocument()) {
            if (reader.name() == ELEMENT_GLOBJECT) {
                readGlObjectElement(root);
            } else {
                skipUnknownElement();
            }
        } else {
            reader.readNext();
        }
    }
}

void SceneGraphXMLHandler::readGlObjectElement(GlObject *parent) {
    GlObject *child = NULL;

    ////////////////////////////////////////////////////////////////
    /// Read Attributes
    ///
    {
        // variables for creation of GlObject
        bool isPrimitive = false;
        QString name = "default";
        QString definition;
        QVector3D scale(1,1,1), pos(0,0,0);
        QQuaternion rot;

        QStringRef sRef;
        if (reader.attributes().hasAttribute(ATTR_NAME)) {
            sRef = reader.attributes().value(ATTR_NAME);
            name = sRef.toString();
        }

        if (reader.attributes().hasAttribute(ATTR_TRANSLATION)) {
            sRef = reader.attributes().value(ATTR_TRANSLATION);
            bool ok;
            QVector3D v = stringToQVec3(sRef.toString(), &ok);
            if (ok) {pos = v;}
        }

        if (reader.attributes().hasAttribute(ATTR_SCALE)) {
            sRef = reader.attributes().value(ATTR_SCALE);
            bool ok;
            QVector3D v = stringToQVec3(sRef.toString(), &ok);
            if (ok) {scale = v;}
        }

        if (reader.attributes().hasAttribute(ATTR_ROTATION)) {
            sRef = reader.attributes().value(ATTR_ROTATION);
            bool ok;
            QQuaternion q = stringToQQuat(sRef.toString(), &ok);
            if (ok) {rot = q;}
        }

        if (reader.attributes().hasAttribute(ATTR_IS_PRIMITIVE)) {
            sRef = reader.attributes().value(ATTR_IS_PRIMITIVE);
            if (*sRef.string() == VAL_TRUE) {
                isPrimitive = true;
                if (reader.attributes().hasAttribute(ATTR_DEFINITION)) {
                    sRef = reader.attributes().value(ATTR_DEFINITION);
                    if (PrimTypes::isInPrimTypeNames(sRef.toString())) {
                        definition = sRef.toString();
                        // It's a valid GlPrimitiveObject, we should make it now
                        child = new GlPrimitiveObject(definition, parent);
                    }
                }
            }
        }

        if (!child) {
            // if object hasn't been assigned yet, it's not a primitive
            child = new GlObject(name, parent, isPrimitive);
        }
        child->setRotation(rot);
        child->setScale(scale);
        child->setTranslation(pos);
    }
    ///////////////////////////////////////////////////////////////////////
    /// add the node's children
    ///
    {
        reader.readNext();
        while (!reader.atEnd()) {
            if (reader.isEndElement()) {
                reader.readNext();
                break;
            }

            if (reader.isStartElement()) {
                if (reader.name() == ELEMENT_GLOBJECT) {
                    readGlObjectElement(child);
                } else {
                    skipUnknownElement();
                }
            } else {
                reader.readNext();
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////
    /// decide whether to keep the node or throw it away
    ///
    if (child) {
        if (child->getNumChildren() > 0 && !child->isPrimitive()) {
            parent->addChild(child);
        } else if (child->getNumChildren() == 0 && child->isPrimitive()) {
            parent->addChild(child);
        } else {
            delete child;
            child = NULL;
        }
    }
}

void SceneGraphXMLHandler::skipUnknownElement() {
    reader.readNext();
    while (!reader.atEnd()) {
        if (reader.isEndElement()) {
            reader.readNext();
            break;
        }

        if (reader.isStartElement()) {
            skipUnknownElement();
        } else {
            reader.readNext();
        }
    }
}

QVector3D SceneGraphXMLHandler::stringToQVec3(const QString &str, bool *ok) {
    // should be in the form "#,#,#" //x,y,z
    QStringList list = str.split(',', QString::SkipEmptyParts);
    if (list.size() == 3) {
        bool ok0, ok1, ok2;
        QVector3D vec(list[0].toFloat(&ok0),
                list[1].toFloat(&ok1),
                list[2].toFloat(&ok2));
        *ok = ok0 && ok1 && ok2;
        return vec;
    } else {
        *ok = false;
        return QVector3D();
    }
}

QQuaternion SceneGraphXMLHandler::stringToQQuat(const QString &str, bool *ok) {
    // should be in the form "#,#,#,#" //w,x,y,z
    QStringList list = str.split(',', QString::SkipEmptyParts);
    if (list.size() == 4) {
        bool ok0, ok1, ok2, ok3;
        QQuaternion quat(list[0].toFloat(&ok0),
                list[1].toFloat(&ok1),
                list[2].toFloat(&ok2),
                list[3].toFloat(&ok3));
        *ok = ok0 && ok1 && ok2 && ok3;
        return quat;
    } else {
        *ok = false;
        return QQuaternion();
    }
}

QString SceneGraphXMLHandler::qVec3ToString(const QVector3D &v) {
    QString str;
    QTextStream ts(&str);
    ts << v.x() << "," << v.y() << "," << v.z();
    return str;
}

QString SceneGraphXMLHandler::qQuatToString(const QQuaternion &q) {
    QString str;
    QTextStream ts(&str);
    ts << q.scalar() << "," << q.x() << "," << q.y() << "," << q.z();
    return str;
}

