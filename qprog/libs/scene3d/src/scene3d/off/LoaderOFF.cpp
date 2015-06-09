#include "LoaderOFF.h"
#include "../Tri.h"
#include <Message.h>
#include <QFile>
#include <QStringList>
#include <QTextStream>

TriSoup* LoaderOFF::loadRaw(QString fileName) const {
    int nv = 0, nf = 0;
    
    if (fileName.isEmpty()) {
        Message::error(QString("le nom de fichier spécifié est vide"));
        return NULL;
        }
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Message::error(QString("impossible d'ouvrir le fichier '%1' : %2").arg(fileName).arg(file.errorString()));
        return NULL;
        }
    
    TriSoup *res = NULL;
    QTextStream stream(&file);
    
    #define FORMAT_ERROR(message) { \
        Message::error(QString("erreur de format du fichier '%1' ligne %2 : %3").arg(fileName).arg(num).arg(message)); \
        file.close();   \
        return NULL; }
    #define READ_NEXT_LINE { \
        if (stream.atEnd()) FORMAT_ERROR("fin de fichier inattendue")    \
        else { line = stream.readLine(); num++; }   \
        }
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
            res = new TriSoup(nv, nf, fileName);
            /*
            vertex = new Vec3[nv];
            normal = new Vec3[nv];
            face   = new Face[nf];
            */
            datastream.readRawData(reinterpret_cast<char*>(res->vertex), sizeof(Vec3) * nv);
            
            for (int i=0; i<nf; i++) {
                int n;
                datastream >> n;
                //int *index = new int[n];
                if (n != 3) {
                    Message::error("faces autres que triangles non supportées");
                    delete res;
                    file.close();
                    return NULL;
                    }
                datastream.readRawData(reinterpret_cast<char*>(res->tri[i].index), sizeof(int) * n);
                //datastream.readRawData(reinterpret_cast<char*>(index), sizeof(int) * n);
                //face[i] = Face(n, index);
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

        /*
        vertex = new Vec3[nv];
        normal = new Vec3[nv];
        face = new Face[nf];
        */
        res = new TriSoup(nv, nf, fileName);
        
        do { READ_NEXT_LINE; } while (line.section("#", 0, 0).simplified().isEmpty());
        
        for (int i=0; i<nv; i++) {
            if (i > 0) READ_NEXT_LINE
            SPLIT_LINE
            bool ok1; float x = word[0].toFloat(&ok1);
            bool ok2; float y = word[1].toFloat(&ok2);
            bool ok3; float z = word[2].toFloat(&ok3);
            if (!(ok1 && ok2 && ok3)) FORMAT_ERROR("ligne pas au format vertex : 'x y z'");
            res->vertex[i] = Vec3(x, y, z);
            }

        do { READ_NEXT_LINE; } while (line.section("#", 0, 0).simplified().isEmpty());
        
        for (int i=0; i<nf; i++) {
            if (i > 0) READ_NEXT_LINE
            SPLIT_LINE
            bool ok; int n = word[0].toInt(&ok); if (!ok) FORMAT_ERROR("probleme format face");
            if (n < 3) FORMAT_ERROR("face a moins de 3 aretes");
            if (n != 3) {
                Message::error("faces autres que triangles non supportées");
                delete res;
                file.close();
                return NULL;
                }
            
            //int *index = new int[n];
            for (int j=0; j<n; j++) { res->tri[i].index[j] = word[j+1].toInt(&ok); if (!ok) FORMAT_ERROR("probleme format face"); }
            //face[i] = Face(n, index);
            }

        file.close();
        }
    
    #undef FORMAT_ERROR
    #undef READ_NEXT_LINE
    
    return res;
    }

/**************************************************************************************************/

void LoaderOFF::save(const TriSoup *ts, QString fileName) const {
    MSG_INFO("- writing file '%1' ...", (fileName));
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);
        const char* header = "OFF BINARY\n";
        stream.writeRawData(header, 11);
        
        stream << ts->nv;
        stream << ts->nt;
        stream << (int)0;
        
        stream.writeRawData(reinterpret_cast<const char*>(ts->vertex), sizeof(Vec3) * ts->nv);

        for (int i=0; i<ts->nt; i++) {
            stream << 3;
            stream.writeRawData(reinterpret_cast<char*>(ts->tri[i].index), 3 * sizeof(int));
            }

        file.close();
        }
    else
        MSG_ERROR("impossible to write '%1'", (fileName));

    Message::info("> done.");
    }

/*
// generic method for *polygon*-soup (not just triangles)
void LoaderOFF::save(const TriSoup *ts, QString fileName) const {
    Message::debug(QString("- writing file '%1' ...").arg(fileName));
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);
        char* header = "OFF BINARY\n";
        stream.writeRawData(header, 11);
        
        stream << ts->nv;
        stream << ts->nf;
        stream << (int)0;
        
        stream.writeRawData(reinterpret_cast<const char*>(ts->vertex), sizeof(Vec3) * ts->nv);

        for (int i=0; i<ts->nf; i++) {
            stream << ts->face[i].nv;
            stream.writeRawData(reinterpret_cast<char*>(ts->face[i].index), sizeof(int) * ts->face[i].nv);
            }

        file.close();
        }
    else
        Message::error(QString("impossible to write '%1'").arg(fileName));

    Message::debug("> done.");
    }
*/


