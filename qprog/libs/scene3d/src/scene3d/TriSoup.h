#ifndef __TRI_SOUP__
#define __TRI_SOUP__

#include "AABox.h"
#include <Vec3.h>
#include <QString>
#include <QStringList>
#include <QList>
class Tri;
class TriSoupLoader;


class TriSoup {
    public:
        //TriSoup(QString fileName);
        TriSoup(int nv, int nt, QString fileName);
        ~TriSoup();

        Vec3  getCenter() const;
        float getRadius() const;

        AABox bbox() const { return box; }

        static TriSoup* load(QString fileName);

        QStringList infos() const;

    //
    protected:
        void postProcess();

    public:
        int nv;     // number of vertices
        int nt;     // number of faces
        Vec3 *vertex;
        Vec3 *normal;
        Tri *tri;
    private:
        Vec3 center;
        float radius;
        AABox box;

    protected:
        static QList<TriSoupLoader*> loaders;
    public:
        QString fileName;

    friend class TriSoupLoader;
    };


class TriSoupLoader {
    public:
        virtual ~TriSoupLoader() {}
        virtual TriSoup* loadRaw(QString fileName) const = 0;      // warning : does not perform post-process (bbox, normals computation, etc.)
        virtual QString suffix() const = 0;
        TriSoup* load(QString fileName) const;     // loads the tri-soup and performs post-process
    };

#endif
