#ifndef __TAB_1D__
#define __TAB_1D__

#include "Message.h"
#include "ObjectXML.h"
#include <QString>
#include <QFile>
#include <QDataStream>

BABLIB_NAMESPACE_BEGIN

#define for1D(tab, index) for (int index = 0; index < tab.N(); index++)

template<class Object>
class Tab1D {
    public:
        Tab1D() : n(-1), data(NULL) {}
        Tab1D(int n) : n(n), data(new Object[n]) {}
        Tab1D(int n, Object *data) : n(n), data(data) {}
        Tab1D(QString fileName) : n(-1), data(NULL) { load(fileName); }

        void destroy() { if (data != NULL) { delete[] data; data = NULL; } }
        void fill(Object value) { for (int i=0; i<n; i++) data[i] = value; }
        bool empty() const { return data == NULL; }
        
        inline       Object&  value(int i)          { return data[i]; }
        inline const Object&  value(int i)    const { return data[i]; }
        inline       Object&  operator()(int i)       { return value(i); }
        inline const Object&  operator()(int i) const { return value(i); }
        
        inline       Object* getData()       { return data; }
        inline const Object* getData() const { return data; }

        inline int N() const { return n; }
        inline int size() const { return n; }
        
        inline Object* clonedData() const { Object* newData = new Object[n]; for (int i=0; i<n; i++) newData[i] = data[i]; return newData; }
        Tab1D<Object>  clone() const { return Tab1D(n, clonedData()); }
        
        void save(QString fileName) const {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly)) {
                QDataStream stream(&file);
                stream << n;
                file.write(reinterpret_cast<const char*>(data), sizeof(Object) * n);
                file.close();
                }
            else
                Message::error(QString("probleme d'ecriture : '%1'").arg(fileName));
            }
        
        void load(QString fileName) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly)) {
                destroy();
                QDataStream stream(&file);
                stream >> n;
                data = new Object[n];
                file.read(reinterpret_cast<char*>(data), sizeof(Object) * n);
                file.close();
                }
            else
                Message::error(QString("probleme de lecture : '%1'").arg(fileName));
            }
        
        void write(QDataStream &stream) const {
            stream << n;
            stream.writeRawData(reinterpret_cast<const char*>(data), sizeof(Object) * n);
            }
        
        void read(QDataStream &stream) {
            stream >> n;
            data = new Object[n];
            stream.readRawData(reinterpret_cast<char*>(data), sizeof(Object) * n);
            }
        
        QDomElement domElement(const QString &name, QDomDocument &doc) const {
            QDomElement res = doc.createElement(name);
            res.setAttribute("n", n);
            for (int i=0; i<n; i++) res.appendChild(data[i].domElement("e" + QString::number(i), doc));  //@ format a voir
            return res;
            }
        
        void initFromDOMElement(const QDomElement &e) {
            n = ObjectXML::getIntAttrib(e, "n", 0);
            data = new Object[n];
            for (int i=0; i<n; i++) data[i].initFromDOMElement(e.firstChildElement("e" + QString::number(i)));  //@ format a voir
            }
        
        static Object* readDOMElement(const QDomElement e) {
            Tab1D<Object> tab;
            tab.initFromDOMElement(e);
            return tab.data;
            }
        
    private:
        int n;
        Object *data;
    };

template<class Object> Object* cloneTab1D(int n, Object *data) { return Tab1D<Object>(n, data).clonedData(); }

typedef Tab1D<int>    Int1D;
typedef Tab1D<bool>   Bool1D;
typedef Tab1D<float>  Float1D;
typedef Tab1D<double> Double1D;

BABLIB_NAMESPACE_END

#endif

