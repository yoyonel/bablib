#ifndef __TRI_MESH__
#define __TRI_MESH__

#include "TriSoup.h"
#include <Vec3.h>
#include <QList>

class TriMesh {
    public:
        struct Edge {
            int v0, v1;     // v0 < v1
            int priL, priR;
            Edge(int v0, int v1) : priL(-1), priR(-1) {
                if (flip(v0,v1)) {
                    this->v0 = v1;
                    this->v1 = v0;
                    }
                else {
                    this->v0 = v0;
                    this->v1 = v1;
                    }
                }
            static bool flip(int v0, int v1) { return v1 < v0; }
            };
        
        struct Pri {
            int vert[3];
            int edge[3];
            bool onLeft[3]; // onLeft[i] == true iff primitive on left of edge[i]
            //Float3 color;
            Vec3 color;
            //Pri(int vert0, int vert1, int vert2, Vec3 color) : color(color) { vert[0] = vert0; vert[1] = vert1; vert[2] = vert2; }
            };
    
    public:
        TriMesh(const TriSoup *ts);
        
        void render() const;
        void renderPri(int i) const;
        void renderPri(int i, Vec3 color) const;
        
        void clear() {
            vertex.clear();
            pri.clear();
            }
            
        Vec3 normal(int i) const;
        int neighbor(int i, int j) const;

    public:
        QList<Vec3> vertex;
        QList<Edge> edge;
        QList<Pri> pri;
    };

#endif

