#include "Message.h"
#include <QFile>
#include <QTextStream>

BABLIB_NAMESPACE_BEGIN

#define XML_ELEMENT_NAME "Object"

template<class Object>
void ObjectXML::save(const Object &obj, QString fileName) {
    QDomDocument doc("ObjectXML");
    QDomElement e = obj.domElement(XML_ELEMENT_NAME, doc);
    doc.appendChild(e);
    
    QFile dataFile(fileName);
    if (dataFile.open(QIODevice::WriteOnly)) {
        QTextStream dataStream(&dataFile);
        doc.save(dataStream, 2);
        dataFile.flush();
        dataFile.close();
        }
    else
        MSG_ERROR("cannot write file '%1' : %2", (fileName) (dataFile.errorString()));
    }

template<class Object>
void ObjectXML::load(Object &obj, QString fileName) {
    QFile dataFile(fileName);
    QDomDocument doc;
    if (dataFile.open(QIODevice::ReadOnly) == true) {
        doc.setContent(&dataFile);
        dataFile.close();
        }
    else {
        MSG_ERROR("cannot read file '%1' : %2", (fileName) (dataFile.errorString()));
        return;
        }
    
    // lecture des attributs :
    QDomElement e = doc.documentElement();
    if (e.tagName() == XML_ELEMENT_NAME)
        obj.initFromDOMElement(e);
    else
        MSG_ERROR("no node '%1' dans le fichier XML", (XML_ELEMENT_NAME));
    }

/*
template<class Object> Object* ObjectXML::getArray(const QDomElement &e) {
    const int size = getIntAttrib(e, "size", 0);
    Object* res = new Object[n];
    for (int i=0; i<size; i++) res[i].initFromDOMElement(QString::number(i));  //@ format a voir
    }

template<class Object> QDomElement ObjectXML::arrayElement(const Object* obj, int size, const QString &name, QDomDocument &doc) {
    QDomElement res = doc.createElement(name);
    res.setAttribute("size", size);
    for (int i=0; i<size; i++) res.appendChild(obj[i].domElement(QString::number(i), doc));  //@ format a voir
    }
*/

BABLIB_NAMESPACE_END

