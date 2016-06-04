#ifndef SCENEXMLHANDLER_H
#define SCENEXMLHANDLER_H

#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class GlObject;
class QVector3D;
class QQuaternion;


/**
 * @brief The SceneXMLHandler class
 * Borrows code from Chapter 15 of "C++ Gui Programming with Qt, 4th ed"
 * by jasmin blanchette and mark summerfeld
 *
 * This class is used to convert between a tree of GlObjects and SceneXML
 * files. It can save a GlObject tree structure to an XML file, and load
 * a GlObject tree structure from an XML file.
 */
class SceneGraphXMLHandler
{
public:
    SceneGraphXMLHandler(GlObject *root);

    bool readFile(const QString &fileName);
    GlObject *getRoot() const;

    static bool writeXml(const QString& fileName, const GlObject *root);
    static void writeGlObject(QXmlStreamWriter *xmlWriter, const GlObject *node);


    static const char* ELEMENT_SCENEGRAPH_ROOT; // = "SceneGraphXMLRoot";
    static const char* ELEMENT_GLOBJECT; // = "GlObject";
    static const char* ATTR_IS_PRIMITIVE; // = "isPrim";
    static const char* ATTR_NAME; // = "name";
    static const char* ATTR_TRANSLATION; // = "pos";
    static const char* ATTR_SCALE; // = "scale";
    static const char* ATTR_ROTATION; // = "rot";
    static const char* ATTR_DEFINITION; // = "def";

    static const char* VAL_TRUE; // = "true";
    static const char* VAL_FALSE; // = "false";

private:

    void readSceneGraphRootElement();
    void readGlObjectElement(GlObject *parent);

//    void readBookindexElement();
//    void readEntryElement(QTreeWidgetItem *parent);
//    void readPageElement(QTreeWidgetItem *parent);
    void skipUnknownElement();

    static QVector3D stringToQVec3(const QString &str, bool *ok);
    static QQuaternion stringToQQuat(const QString&str, bool *ok);
    static QString qVec3ToString(const QVector3D&v);
    static QString qQuatToString(const QQuaternion&);

    //QTreeWidget *treeWidget;
    GlObject *root;
    QXmlStreamReader reader;
};

#endif // SCENEXMLHANDLER_H
