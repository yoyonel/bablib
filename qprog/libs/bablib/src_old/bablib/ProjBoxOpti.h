#ifndef __PROJ_BOX_OPTI__
#define __PROJ_BOX_OPTI__

#include <Vec3.h>
#include <Vec4.h>
#include <ProjTransfo.h>
#include <QList>

BABLIB_NAMESPACE_BEGIN

class ProjBoxOpti {
    public:
        ProjBoxOpti() {}
        
        void addMapping(Vec4 p, Vec4 q, float w=1.0f);
        void addMapping(Vec3 p, Vec3 q, float w=1.0f);
        
        ProjTransfo calcTransfo() const;    // returns transfo T such that T * pi = qi
        
    private:
        struct Mapping {
            Mapping(Vec4 p, Vec4 q, float w) : p(p), q(q), w(w) {}
            Vec4 p;     // source
            Vec4 q;     // destination
            float w;    // weight
            };
        QList<Mapping> map;
    };

BABLIB_NAMESPACE_END

#endif

