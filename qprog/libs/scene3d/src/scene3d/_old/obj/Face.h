#ifndef __FACE__
#define __FACE__

#include <Vec3.h>

//voir : template..
class Face {
    public:
        Face() : nv(0), index(NULL) {}   //@ perrave --> faire template
        //Face(int nv) : nv(nv), index(new int[nv]) {}
        Face(int nv, int *index) : nv(nv), index(index) {}
        Face(int nv, int *index, Vec3 normal) : nv(nv), index(index), normal(normal) {}
        //~Face() { if (index != NULL) delete[] index; }    //@ perrave

        void destroy() { if (index != NULL) delete[] index; }    //@ perrave
        
    public:
    //private:
        int nv;
        int *index;
        Vec3 normal;
    };

#endif

