#include "TriSoup.h"
#include "Tri.h"
#include "AABox.h"
#include <Message.h>
#include <Tab1D.h>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>

/**************************************************************************************************/
// partie ÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ  modifier ÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ  chaque ajout de nouveau loader :

#include "obj/LoaderOBJ.h"
#include "off/LoaderOFF.h"
#ifdef PLY_SUPPORT
    #include "ply/LoaderPLY.h"
#endif

QList<TriSoupLoader*> TriSoup::loaders = QList<TriSoupLoader*>()
#ifdef PLY_SUPPORT
    << new LoaderPLY()
#endif
    << new LoaderOBJ()
    << new LoaderOFF();

/**************************************************************************************************/

TriSoup* TriSoup::load(QString fileName) {
    foreach(TriSoupLoader *loader, loaders) {
        if (fileName.endsWith("." + loader->suffix(), Qt::CaseInsensitive))
            return loader->load(fileName);
        /*
        if (fileName.endsWith("." + loader->suffix(), Qt::CaseInsensitive)) {
            TriSoup *res = loader->load(fileName);
            if (res != NULL) res->postProcess();
            return res;
            }
        */
        }
    Message::error(QString("format de fichier non supportÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ© : %1").arg(fileName));
    return NULL;
    }

TriSoup::TriSoup(int nv, int nt, QString fileName) : nv(nv), nt(nt), fileName(fileName) {
    vertex = new Vec3[nv];
    normal = new Vec3[nv];
    tri    = new Tri[nt];
    }

void TriSoup::postProcess() {
    // post-process : calcul bbox et normales :
    //-----------------------------------------
    //int* vertexFaces = new int[nv];
    //AABox box;
    for (int i=0; i<nv; i++) {
        //vertex[i] += Vec3::random(-0.2,0.2);
        box.add(vertex[i]);
        normal[i] = Vec3(0,0,0);
        //vertexFaces[i] = 0;
        }
    center = box.center();
    radius = box.radius();
    
    for (int i=0; i<nt; i++) {
        tri[i].color = Vec3::random(0.2, 0.8);
        int* index = tri[i].index;

        const Vec3 &a = vertex[index[0]];
        const Vec3 &b = vertex[index[1]];
        const Vec3 &c = vertex[index[2]];
        tri[i].normal = (b - a) ^ (c - a);
        const float nNorm = tri[i].normal.norm();
        if (nNorm != 0.0f) tri[i].normal /= nNorm;     // for robustness
        //tri[i].normal = triangleNormal(vertex[index[0]], vertex[index[1]], vertex[index[2]]);
        
        for (int j=0; j<3; j++) {
            normal[index[j]] += tri[i].normal;
            //vertexFaces[index[j]] ++;
            }
        }
    
    for (int i=0; i<nv; i++) {
        //MSG_DEBUG_VAR(i);
        //MSG_DEBUG_VAR(normal[i].toQString());
        //normal[i] /= vertexFaces[i];
        normal[i].normalize();
        }
    //delete[] vertexFaces;
    
    Message::info(QString("> mesh loaded : %1 vertices, %2 triangles.").arg(nv).arg(nt));
    }

TriSoup::~TriSoup() {
    delete[] vertex;
    delete[] normal;
    delete[] tri;
    }

Vec3  TriSoup::getCenter() const { return center; }
float TriSoup::getRadius() const { return radius; }

QStringList TriSoup::infos() const {
    return QStringList()
        << QString("%1 vertices").arg(nv)
        << QString("%1 faces").arg(nt);
    }

/**************************************************************************************************/

TriSoup* TriSoupLoader::load(QString fileName) const {
    TriSoup *res = loadRaw(fileName);
    if (res != NULL) res->postProcess();
    return res;
    }

