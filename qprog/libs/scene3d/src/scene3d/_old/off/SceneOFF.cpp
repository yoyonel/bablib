#include "SceneOFF.h"
#include "Face.h"
#include "AABox.h"
#include <Message.h>
#include <Tab1D.h>
#include <glide/glarraybuffers.h>
#include <QFile>
#include <QStringList>
#include <QTextStream>
#include <GL/gl.h>

using namespace glide;

SceneOFF::SceneOFF(QString fileName) : Scene3D(fileName), geometry(NULL) {
    nv = 0;
    
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
        #define READ_NEXT_LINE  \
            if (stream.atEnd()) FORMAT_ERROR("fin de fichier inattendue")    \
            else { line = stream.readLine(); num++; }
        #define SPLIT_LINE word = line.split(" ", QString::SkipEmptyParts);

        int num = 0;
        QString line;
        QStringList word;
        READ_NEXT_LINE
        
        QStringList words = line.section("#", 0, 0).simplified().split(" ", QString::SkipEmptyParts);
        if (words.size() < 1 || words[0] != "OFF") FORMAT_ERROR("le premier mot de la premiere ligne doit etre 'OFF'")
            
        if (words.size() > 1) {
            file.close();
            if (words[1] != "BINARY") FORMAT_ERROR("le deuxième mot (optionnel) doit être BINARY");
            if (words.size() > 2)     FORMAT_ERROR("mots inconnus présents après BINARY");

            // BINARY mode
            //------------
            QFile datafile(fileName);
            if (datafile.open(QIODevice::ReadOnly)) {
                QDataStream datastream(&datafile);
                
                //int nSkip = line.length();
                char c = '\0';
                while (c != '\n') datastream.readRawData(&c, 1); 
            
                int ne;     // nombre d'arêtes, pas utilisé
                datastream >> nv;
                datastream >> nf;
                datastream >> ne;
                vertex = new Vec3[nv];
                normal = new Vec3[nv];
                face   = new Face[nf];
                datastream.readRawData(reinterpret_cast<char*>(vertex), sizeof(Vec3) * nv);
                
                for (int i=0; i<nf; i++) {
                    int n;
                    datastream >> n;
                    int *index = new int[n];
                    datastream.readRawData(reinterpret_cast<char*>(index), sizeof(int) * n);
                    face[i] = Face(n, index);
                    }
                                
                datafile.close();
                }
            }
        else {
            // ASCII mode
            //-----------
            READ_NEXT_LINE
            SPLIT_LINE
            bool ok1; nv = word[0].toInt(&ok1);
            bool ok2; nf = word[1].toInt(&ok2);
            if (!(ok1 && ok2)) FORMAT_ERROR("problème de lecture des entiers sur la deuxième ligne");
    
            vertex = new Vec3[nv];
            normal = new Vec3[nv];
            face = new Face[nf];
            
            do { READ_NEXT_LINE; } while (line.section("#", 0, 0).simplified().isEmpty());
            
            for (int i=0; i<nv; i++) {
                if (i > 0) READ_NEXT_LINE
                SPLIT_LINE
                bool ok1; float x = word[0].toFloat(&ok1);
                bool ok2; float y = word[1].toFloat(&ok2);
                bool ok3; float z = word[2].toFloat(&ok3);
                if (!(ok1 && ok2 && ok3)) FORMAT_ERROR("ligne pas au format vertex : 'x y z'");
                vertex[i] = Vec3(x, y, z);
                }
    
            do { READ_NEXT_LINE; } while (line.section("#", 0, 0).simplified().isEmpty());
            
            for (int i=0; i<nf; i++) {
                if (i > 0) READ_NEXT_LINE
                SPLIT_LINE
                bool ok; int n = word[0].toInt(&ok); if (!ok) FORMAT_ERROR("probleme format face");
                if (n < 3) FORMAT_ERROR("face a moins de 3 aretes");
                int *index = new int[n];
                for (int j=0; j<n; j++) { index[j] = word[j+1].toInt(&ok); if (!ok) FORMAT_ERROR("probleme format face"); }
                face[i] = Face(n, index);
                }

            file.close();
            }
        
        //#undef FORMAT_ERROR
        //#undef READ_NEXT_LINE
        
        // post-process : calcul bbox et normales :
        //-----------------------------------------
        int* vertexFaces = new int[nv];
        AABox box;
        for (int i=0; i<nv; i++) {
            box.add(vertex[i]);
            normal[i] = Vec3(0,0,0);
            vertexFaces[i] = 0;
            }
        center = box.center();
        radius = box.radius();
        
        for (int i=0; i<nf; i++) {
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

SceneOFF::~SceneOFF() {
    delete geometry;
    delete[] vertex;
    delete[] normal;
    for (int i=0; i<nf; i++) face[i].destroy();
    delete[] face;
    }

void SceneOFF::init() {
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
    initialized = true;
    }
        
void SceneOFF::render() const {
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glColor3f(0.8f, 0.8f, 0.8f);
    
    if (geometry != NULL) geometry->render();
    }

Vec3  SceneOFF::getCenter() const { return center; }
float SceneOFF::getRadius() const { return radius; }

