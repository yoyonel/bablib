#ifndef __SHADER_GLSL__
#define __SHADER_GLSL__

#include <OpenGL.h>
//#include "ProgGLSL.h"
#include <QString>

BABLIB_NAMESPACE_BEGIN

class ShaderGLSL {
    public:
        typedef enum { VERTEX, FRAGMENT } Type;     //@ ajouter GEOMETRY
    public:
        // constructeur à appeler quand un contexte OpenGL a été initialisé
        // <source> contient le code source si <loadFromFile>==false, le nom du fichier source sinon
        // if <loadFromFile>==false, <name> is used as the displayed shader name for compilation error messages
        ShaderGLSL(QString source, Type type, bool loadFromFile=true, QString name = "<sans nom>");
    
        void compile();
        void destroy();
        
        // comparaison de deux shaders à partir des id :
        bool operator==(const ShaderGLSL& s) { return id == s.id; }
        
    private:
        QString getInfoLog() const;
        
    private:
        QString fileName;   // juste pour les eventuels messages d'erreurs
        GLuint id;
        
    friend class ProgGLSL;     // pour permettre à la classe ProgrammeGLSL d'accéder à la variable <id>
    };

BABLIB_NAMESPACE_END

#endif
