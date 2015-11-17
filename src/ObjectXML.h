#ifndef __OBJECT_XML__
#define __OBJECT_XML__

#include <QString>
#include <QDomElement>
#include <QDomDocument>

BABLIB_NAMESPACE_BEGIN

#define OBJXML_METHODS  \
    QDomElement domElement(const QString &name, QDomDocument &document) const;  \
    void initFromDOMElement(const QDomElement &element);

// class with practical functions to read/write XML file for objects impementing the OBJXML_METHODS
class ObjectXML {
    public:
        // class <Object> must implement OBJXML_METHODS
        template<class Object> static void save(const Object &obj, QString fileName);
        template<class Object> static void load(      Object &obj, QString fileName);
        
    // useful methods to parse XML files:
        static int   getIntAttrib(const QDomElement &e,   const QString &name, int   defaultValue = 0);
        static float getFloatAttrib(const QDomElement &e, const QString &name, float defaultValue = 0);
        
        /*
        template<class Object> static Object* getArray(const QDomElement &e);
        template<class Object> static QDomElement arrayElement(const Object* obj, int size, const QString &name, QDomDocument &doc);
        */
    };

BABLIB_NAMESPACE_END

#include "ObjectXML_impl.h"

#endif

