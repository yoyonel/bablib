#include "SceneOBJ.h"
#include "Face.h"
#include "AABox.h"
#include <Message.h>
#include <Tab1D.h>
#include <glide/glarraybuffers.h>
#include <QList>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <GL/gl.h>

using namespace glide;

struct VertexOBJ {
    VertexOBJ(int iv, int it, int in) : iv(iv), it(it), in(in) {}
    int iv;
    int it;
    int in;
    };

SceneOBJ::SceneOBJ(QString fileName) : Scene3D(fileName), geometry(NULL), displist(0) {
    nv = nf = 0;
    
    if (fileName.isEmpty()) {
        Message::error(QString("le nom de fichier spécifié est vide"));
        return;
        }
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        
        #define FORMAT_ERROR(message) { \
            Message::error(QString("erreur de format du fichier '%1' ligne %2 : %3").arg(fileName).arg(num).arg(message)); \
            file.close();   \
            return; }
        #define UNSUPPORTED(message) \
            Message::error(QString("balise non supportee : fichier '%1' ligne %2 : %3").arg(fileName).arg(num).arg(message));
        #define READ_NEXT_LINE  \
            if (stream.atEnd()) FORMAT_ERROR("fin de fichier inattendue")    \
            else { line = stream.readLine(); num++; }
        #define END_FILE (stream.atEnd())
        //#define SPLIT_LINE word = line.split(" ", QString::SkipEmptyParts);
        #define SPLIT_LINE word = line.section("#", 0, 0).simplified().split(" ", QString::SkipEmptyParts);

        int num = 0;
        QString line;
        QStringList word;

        QList<Vec3> v;      //@ voir : Vec4 possible..
        QList<Vec3> vt;
        QList<Vec3> vn;
        QList< QList<VertexOBJ> > f;
        
        #define FLOAT_REQ(ok, val, i)           bool ok = (i < word.size());    float val = ok                ? word[i].toFloat(&ok) : 0.0f;
        #define FLOAT_OPT(ok, val, i, def)      bool ok = true;                 float val = (i < word.size()) ? word[i].toFloat(&ok) : def;
        #define INT_OPT(ok, val, tab, i, def)   bool ok = true;                 int   val = (i >= tab.size() || tab[i].isEmpty()) ? def : tab[i].toInt(&ok);
        
        while (!END_FILE) {
            READ_NEXT_LINE
            SPLIT_LINE
            
            if (word.size() >= 1) {
                QString token = word[0].toLower();
                if (token == "v") {
                    FLOAT_REQ(ok1, x, 1)
                    FLOAT_REQ(ok2, y, 2)
                    FLOAT_REQ(ok3, z, 3)
                    //FLOAT_OPT(ok4, w, 4, 1.0f)
                    if (word.size() > 4) UNSUPPORTED("les vertex doivent etre au format x y z")
                    if (!(ok1 && ok2 && ok3)) FORMAT_ERROR("ligne pas au format vertex : 'v x y z [w]'");
                    v << Vec3(x,y,z);
                    }
                else if (token == "vt") {
                    FLOAT_REQ(ok1, u, 1)
                    FLOAT_REQ(ok2, v, 2)
                    FLOAT_OPT(ok3, w, 3, 0.0f)
                    if (!(ok1 && ok2 && ok3)) FORMAT_ERROR("ligne pas au format coordonnées de texture : 'vt u v [w]'");
                    vt << Vec3(u,v,w);
                    }
                else if (token == "vn") {
                    FLOAT_REQ(ok1, x, 1)
                    FLOAT_REQ(ok2, y, 2)
                    FLOAT_REQ(ok3, z, 3)
                    if (!(ok1 && ok2 && ok3)) FORMAT_ERROR("ligne pas au format normale : 'vn x y z'");
                    vn << Vec3(x,y,z);
                    }
                else if (token == "f") {
                    if (word.size() < 4)
                        FORMAT_ERROR("ligne pas au format face : 'f v1 v2 v3 [v4 ...]'");
                    QList<VertexOBJ> newFace;
                    for (int i=1; i<word.size(); i++) {
                        QStringList fi = word[i].split("/");
                        INT_OPT(ok1, iv, fi, 0, 0);
                        INT_OPT(ok2, it, fi, 1, 0);
                        INT_OPT(ok3, in, fi, 2, 0);
                        if (!(ok1 && ok2 && ok3)) FORMAT_ERROR("token pas au format triplet d'indices : 'iv/it/in'");
                        newFace << VertexOBJ(iv-1, it-1, in-1);
                        }
                    f << newFace;
                    }
                else
                    UNSUPPORTED(token)
                }
            }
        
        file.close();
        
        #undef FORMAT_ERROR
        #undef READ_NEXT_LINE
        #undef UNSUPPORTED
        #undef END_FILE
        #undef SPLIT_LINE
        #undef FLOAT_REQ
        #undef FLOAT_OPT
        #undef INT_OPT
        
        nv = v.size();
        nf = f.size();
        vertex = new Vec3[nv];
        normal = new Vec3[nv];
        face = new Face[nf];
                
        // post-process : calcul bbox et normales :
        //-----------------------------------------
        int* vertexFaces = new int[nv];
        AABox box;
        for (int i=0; i<nv; i++) {
            vertex[i] = v[i];
            box.add(vertex[i]);
            normal[i] = Vec3(0,0,0);
            vertexFaces[i] = 0;
            }
        center = box.center();
        radius = box.radius();
        
        for (int i=0; i<nf; i++) {
            face[i].nv = f[i].size();
            face[i].index = new int[face[i].nv];
            for (int j=0; j<face[i].nv; j++)
                face[i].index[j] = f[i][j].iv;      //@ voir : utiliser aussi in, it
            
            int* index = face[i].index;
            face[i].normal = triangleNormal(vertex[index[0]], vertex[index[1]], vertex[index[2]]);
            for (int j=0; j<face[i].nv; j++) {
                normal[index[j]] += face[i].normal;
                vertexFaces[index[j]] ++;
                }
            }
        
        for (int i=0; i<nv; i++) {
            normal[i] /= vertexFaces[i];
            normal[i].normalize();
            }
        delete[] vertexFaces;
        }
    else
        Message::error(QString("impossible d'ouvrir le fichier '%1' : %2").arg(fileName).arg(file.errorString()));
    }

SceneOBJ::~SceneOBJ() {
    delete geometry;
    delete[] vertex;
    delete[] normal;
    for (int i=0; i<nf; i++) face[i].destroy();
    delete[] face;
    if (displist != 0) glDeleteLists(displist, 1);
    }

void SceneOBJ::init() {
    /*
    GLArrayBufferCPU<AL_2(Vertex3f,Normal3f)> vertexArray;

    for (int i=0; i<nv; i++) {
        vertexArray.glNormal3fv(normal[i]);
        vertexArray.glVertex3fv(vertex[i]);
        }
    
    GLElementArrayBufferCPU indexes;
    for (int i=0; i<nf; i++) {
        if (face[i].nv == 3) {
            indexes += face[i].index[0];
            indexes += face[i].index[1];
            indexes += face[i].index[2];
            }
        else
            Message::info(QString("- attention : le modele contient des polygones à %1 cotés (pas supporté pour l'instant)").arg(face[i].nv));
        }

    geometry = new GLAsset::Mesh(GL_TRIANGLES, vertexArray.mappedToGPU(), indexes.mappedToGPU());
    geometry->mapToGPU();
    */
    
    
    displist = glGenLists(1);
    glNewList(displist, GL_COMPILE);
        glBegin(GL_TRIANGLES);
        for (int i=0; i<nf; i++) {
            for (int j=0; j<3; j++) {
                int k = face[i].index[j];
                glNormal3fv(normal[k]);
                glVertex3fv(vertex[k]);
                }
            }
        glEnd();
    glEndList();
    
    initialized = true;
    }
        
void SceneOBJ::render() const {
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glColor3f(0.8f, 0.8f, 0.8f);
    
    //if (geometry != NULL) geometry->render();
    if (displist != 0) glCallList(displist);
    }

Vec3  SceneOBJ::getCenter() const { return center; }
float SceneOBJ::getRadius() const { return radius; }

