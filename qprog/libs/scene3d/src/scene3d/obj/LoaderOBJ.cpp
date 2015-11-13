#include "LoaderOBJ.h"
#include "../Tri.h"
#include <Message.h>
#include <QList>
#include <QFile>
#include <QStringList>
#include <QTextStream>

struct VertexOBJ {
    VertexOBJ(int iv, int it, int in) : iv(iv), it(it), in(in) {}
    int iv;
    int it;
    int in;
    };

TriSoup* LoaderOBJ::loadRaw(QString fileName) const {
    if (fileName.isEmpty()) {
        Message::error(QString("le nom de fichier spÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ©cifiÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ© est vide"));
        return NULL;
        }
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Message::error(QString("impossible d'ouvrir le fichier '%1' : %2").arg(fileName).arg(file.errorString()));
        return NULL;
        }
    
    QTextStream stream(&file);
    
    #define FORMAT_ERROR(message) { \
        Message::error(QString("erreur de format du fichier '%1' ligne %2 : %3").arg(fileName).arg(num).arg(message)); \
        file.close();   \
        return NULL; }
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
                if (!(ok1 && ok2 && ok3)) FORMAT_ERROR("ligne pas au format coordonnÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ©es de texture : 'vt u v [w]'");
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
    
    const int nv = v.size();
    const int nf = f.size();
    
    TriSoup *res = new TriSoup(nv, nf, fileName);
    for (int i=0; i<nv; i++) res->vertex[i] = v[i];
    for (int i=0; i<nf; i++) {
        if (f[i].size() != 3) {
            Message::error("faces autres que triangles non supportÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ©es");
            delete res;
            return NULL;
            }
        for (int j=0; j<f[i].size(); j++)
            res->tri[i].index[j] = f[i][j].iv;      //@ voir : utiliser aussi in, it
        }
    
    return res;
    }


