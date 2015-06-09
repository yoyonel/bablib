#include <OpenGL.h>
#include "TriMesh.h"
#include "Tri.h"
#include <Message.h>
#include <QSet>
#include <QHash>
#include <float.h>

/*********************************************************************************************/

namespace {
    struct IndexPair {
        int i0, i1;
        int k;  // edge index
        IndexPair(int i0, int i1, int k=-1) : k(k) {
            if (TriMesh::Edge::flip(i0,i1)) {
                this->i0 = i1;
                this->i1 = i0;
                }
            else {
                this->i0 = i0;
                this->i1 = i1;
                }
            }
        bool operator==(const IndexPair &e) const { return i0 == e.i0 && i1 == e.i1; }
        friend uint qHash(const IndexPair& e) {
            return qHash(QByteArray(reinterpret_cast<const char*>(&e), 8));
            }
        
        static bool noFlip(int i0, int i1) { return i0 < i1; }
        };
    }

/*********************************************************************************************/

TriMesh::TriMesh(const TriSoup *ts) {
    QSet<IndexPair> indexPairSet;
    
    for(int i=0; i<ts->nv; i++)
        vertex << ts->vertex[i];
    
    for(int i=0; i<ts->nt; i++) {
        const Tri& t = ts->tri[i];
        Pri p;
        p.color = t.color;
        for (int j=0; j<3; j++) {
            const int i0 = t.index[j];
            const int i1 = t.index[(j+1)%3];
            
            QSet<IndexPair>::const_iterator ki = indexPairSet.find(IndexPair(i0, i1));
            int k;
            if (ki == indexPairSet.end()) {
                k = edge.size();
                indexPairSet << IndexPair(i0, i1, k);
                edge << Edge(i0, i1);
                }
            else {
                k = ki->k;
                }
            
            Edge &e = edge[k];
            const bool onLeft = !Edge::flip(i0,i1);   // true iff triangle on left of e
            if (onLeft) {
                if (e.priL != -1) Message::error("inconsistent mesh : double edge");
                e.priL = i;
                }
            else {
                if (e.priR != -1) Message::error("inconsistent mesh : double edge");
                e.priR = i;
                }
            
            p.vert[j] = i0;
            p.edge[j] = k;
            p.onLeft[j] = onLeft;
            }
        pri << p;
        }
    }

/*********************************************************************************************/

int TriMesh::neighbor(int i, int j) const {
    const Edge &e = edge[pri[i].edge[j]];
    return pri[i].onLeft[j] ? e.priR : e.priL;
    }

Vec3 TriMesh::normal(int i) const {
    return triangleNormal(
        vertex[pri[i].vert[0]],
        vertex[pri[i].vert[1]],
        vertex[pri[i].vert[2]]
        );
    }
        
/*********************************************************************************************/

void TriMesh::render() const {
    glBegin(GL_TRIANGLES);
        foreach (const Pri &p, pri) {
            glColor3fv(p.color);
            vertex[p.vert[0]].glVertex();
            vertex[p.vert[1]].glVertex();
            vertex[p.vert[2]].glVertex();
            }
    glEnd();
    }

void TriMesh::renderPri(int i) const {
    renderPri(i, pri[i].color);
    }

void TriMesh::renderPri(int i, Vec3 color) const {
    const Pri &p = pri[i];
    glColor3fv(color);
    glBegin(GL_TRIANGLES);
        vertex[p.vert[0]].glVertex();
        vertex[p.vert[1]].glVertex();
        vertex[p.vert[2]].glVertex();
    glEnd();
    }

