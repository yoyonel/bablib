#ifndef __TAB_4D__
#define __TAB_4D__

#include "Message.h"
#include <QString>
#include <QFile>
#include <QDataStream>

BABLIB_NAMESPACE_BEGIN

template<class Object>
class Tab4D {
    public:
        Tab4D() : n1(-1), n2(-1), n3(-1), n4(-1), data(NULL) {}
        Tab4D(int n1, int n2, int n3, int n4) : n1(n1), n2(n2), n3(n3), n4(n4), data(new Object[n1 * n2 * n3 * n4]) {}
        Tab4D(QString fileName) : n1(-1), n2(-1), n3(-1), n4(-1), data(NULL) { load(fileName); }

        void destroy() { if (data != NULL) { delete[] data; data = NULL; } }
        void fill(Object value) { for (int c=0; c<n1*n2*n3*n4; c++) data[c] = value; }
        bool empty() const { return data == NULL; }
        
        inline       Object&  value(int i, int j, int k, int l)          { return data[i + n1*(j + n2*(k + n3*l))]; }
        inline const Object&  value(int i, int j, int k, int l)    const { return data[i + n1*(j + n2*(k + n3*l))]; }
        inline       Object&  operator()(int i, int j, int k, int l)       { return value(i,j,k,l); }
        inline const Object&  operator()(int i, int j, int k, int l) const { return value(i,j,k,l); }

        void save(QString fileName) const {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly)) {
                QDataStream stream(&file);
                stream << n1;
                stream << n2;
                stream << n3;
                stream << n4;
                file.write(reinterpret_cast<const char*>(data), sizeof(Object) * n1 * n2 * n3 * n4);
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
                stream >> n3;
                stream >> n4;
                data = new Object[n1 * n2 * n3 * n4];
                file.read(reinterpret_cast<char*>(data), sizeof(Object) * n1 * n2 * n3 * n4);
                file.close();
                }
            else
                Message::error(QString("probleme de lecture : '%1'").arg(fileName));
            }
        
    private:
        int n1, n2, n3, n4;
        Object *data;
    };

typedef Tab4D<int>    Int4D;
typedef Tab4D<bool>   Bool4D;
typedef Tab4D<float>  Float4D;
typedef Tab4D<double> Double4D;

BABLIB_NAMESPACE_END

#endif

