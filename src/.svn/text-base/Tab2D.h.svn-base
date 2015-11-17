#ifndef __TAB_2D__
#define __TAB_2D__

#include "Tab1D.h"
#include "Message.h"
#include "Image2D.h"
#include <QString>
#include <QFile>
#include <QDataStream>

BABLIB_NAMESPACE_BEGIN

#define for2D(tab, index1, index2) for (int index1 = 0; index1 < tab.N1(); index1++)  for (int index2 = 0; index2 < tab.N2(); index2++)

template<class Object>
class Tab2D {
    public:
        Tab2D() : n1(-1), n2(-1), data(NULL) {}
        Tab2D(int n1, int n2) : n1(n1), n2(n2), data(new Object[n1 * n2]) {}
        Tab2D(int n1, int n2, Object *data) : n1(n1), n2(n2), data(data) {}
        Tab2D(Image2D<Object> image) : n1(image.width()), n2(image.height()), data(image.mem()) {}
        Tab2D(QString fileName) : n1(-1), n2(-1), data(NULL) { load(fileName); }

        void destroy() { if (data != NULL) { delete[] data; data = NULL; } }
        void fill(Object value) { for (int c=0; c<n1*n2; c++) data[c] = value; }
        bool empty() const { return data == NULL; }
        
        inline       Object&  value(int i, int j)          { return data[i + n1*j]; }
        inline const Object&  value(int i, int j)    const { return data[i + n1*j]; }
        inline       Object&  operator()(int i, int j)       { return value(i,j); }
        inline const Object&  operator()(int i, int j) const { return value(i,j); }
        
        inline Tab1D<Object> col(int j) { return Tab1D<Object>(n1, data + n1*j); }    // la jème colonne
        
        inline int N1() const { return n1; }
        inline int N2() const { return n2; }

        inline       Object* getData()       { return data; }
        inline const Object* getData() const { return data; }

        void save(QString fileName) const {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly)) {
                QDataStream stream(&file);
                stream << n1;
                stream << n2;
                file.write(reinterpret_cast<const char*>(data), sizeof(Object) * n1 * n2);
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
                stream >> n1;
                stream >> n2;
                data = new Object[n1 * n2];
                file.read(reinterpret_cast<char*>(data), sizeof(Object) * n1 * n2);
                file.close();
                }
            else
                Message::error(QString("probleme de lecture : '%1'").arg(fileName));
            }
        
        static Image2D<Object> loadImage(QString fileName) {
            Tab2D<Object> tab;
            tab.load(fileName);
            return Image2D<Object>(tab.n1, tab.n2, tab.data);
            }
                
    private:
        int n1, n2;
        Object *data;
    };

typedef Tab2D<int>    Int2D;
typedef Tab2D<bool>   Bool2D;
typedef Tab2D<float>  Float2D;
typedef Tab2D<double> Double2D;

BABLIB_NAMESPACE_END

#endif

