#ifndef __PROG_GPU__
#define __PROG_GPU__

#include "TextureSet.h"
#include "ShaderGLSL.h"
#include "UniformGLSL.h"
#include "Uniform.h"
#include "Dir.h"
#include <QString>
#include <QList>
#include <QString>
#include <QStringList>
#include <QHash>

BABLIB_NAMESPACE_BEGIN

//@@ gestion uniforms:
//@ structure définie par macro avec un membre par uniform + sets
//@ ou hash-map avec clés=noms uniforms, + création auto par listage des uniforms avec fonction GL [+ pareil pour attribs]
//@ --> classe ProgState + vérifs auto prog activate...
//@ --> par contre enlever la possibilité de définir des Uniform indépendant d'un prog (inutile)
//@@ a voir : passage de valeurs d'uniforms sans accès au prog.. (ou accès au prog mais pas au activate/deactivate)


class ProgGLSL {
    //public:
    //    typedef enum { VERTEX, FRAGMENT } ShaderType;
    public:
        ProgGLSL() : linked(false), created(false) {}
        virtual ~ProgGLSL() {}
        
        class Source {  // provided filenames are preprocessed by ProgGLSL::locator.sourceName()
            public:
                Source(ShaderGLSL::Type type);      //@ possibilite deduire type des noms de fichers inclus / verif
                Source(ShaderGLSL::Type type, QString source, bool isFileName=true);
            
                //@@ gestion associations numérotations messages d'erreur / source
                //  --> associer noms aux portions de codes, defines..
                
                void addFile(QString fileName, bool forceName=false);       // resolves potential includes
                void addSource(QString code);                               // this should not contain local includes
                void addDefine(QString def, QString val=QString::null);
                
                QString getName() const;
                ShaderGLSL::Type getType() const { return type; }
                
                bool empty() const;
                bool hasFileName() const;
                
            private:
                static QString readSourceFile(QString fileName);
                static QString separatorCode();
                void appendCode(QString code);
                QString preprocess(QString code) const;
                
            private:
                QString source;
                ShaderGLSL::Type type;
                QString fileName;           // information used at display of compilation / linking error messages
                friend class ProgGLSL;
            };
        
        typedef QList<Source> SourceSet;
        
    // phases de chargement d'un programme (a appeler dans cet ordre) :
    //-----------------------------------------------------------------
        void create();
        void bindShaderFile(QString fileName, ShaderGLSL::Type type);
        void bindShaderSource(QString source, ShaderGLSL::Type type);
        void bindShader(Source source);
        void bindShaders(SourceSet sources);
        void link();
//        void activate();        // active le programme et actualise les variables liées
//        void deactivate();
        void activate() const;        // active le programme et actualise les variables liées
        void deactivate() const;
        void destroy();         // supprime le programme

        bool isLinked() const;          // true ssi le prog a ete linke correctement
        void activateTextures() const;  // doit être appelé pendant que le programme est activé, si les textures attachées ont été changées
        
    // méthodes de chargment automatique :
    //------------------------------------
        void load(SourceSet sources);                   // creates, compiles and links the GLSL program
	
//        void loadDir(QString dirName, bool sub=false, bool loadGeometryShader=false);  	// loads all GLSL sources found in directory <dirName>
											// + if <sub> is true, lets the user choose a subdir of <dirName> from a dialog window
	void loadDir(QString dirName, bool sub=false, bool bLoadGeometryShader = false, GLenum _glInputType = GL_TRIANGLES, GLenum _glOutputType = GL_TRIANGLE_STRIP, GLint _verticesOut = 3);
	
        void load(QString name, bool loadGeometryShader=false);		// charge les fichiers <name>.vert et <name>.frag
        void load(QString vertName, QString fragName);      		// charge le vertex shader <vertName> et le fragment shader <fragName> (avec extensions)
	void load(QString vertName, QString fragName, QString geomName);// charge le vertex shader <vertName>, le fragment shader <fragName> et le geometry shader <geomName> (avec extensions)
 
        void loadAuto(QString fileName, ShaderGLSL::Type type);
        void loadAuto(Source source);   // source must have a reference file name

        void load(Source s1, Source s2);                // convenient method to load a program consisting in 2 sources
        void load(Source s1, Source s2, Source s3);     // convenient method to load a program consisting in 3 sources
        
        static void addLocation(QString name, QString dir);     // adds a mapping for filenames with prefix [<name>] to directory <dir>
                
    // méthodes d'actualisation de variables (a faire apres le linkage et avant la premiere activation) :
    //---------------------------------------------------------------------------------------------------
        // les variables seront automatiquement actualisées à l'activation du programme en utilisant la valeur pointée juste avant l'activation
        // si warn==true et la variable n'existe pas, un message de warning est affiché (utiliser warn=false pour les variables optionnelles)
        void addUniform(Uniform *uniform, bool warn=true, bool ownUniform=true);    // si ownUniform==true, la suppression de <uniform> est prise en charge
        void addUniform(QString name, const double *value, bool warn=true);
        void addUniform(QString name, const float  *value, bool warn=true);
        void addUniform(QString name, const int    *value, bool warn=true);
        void addUniform(QString name, const bool   *value, bool warn=true);
        void addUniformVec2(QString name, const float *value, bool warn=true);
        void addUniformVec3(QString name, const float *value, bool warn=true);
        void addUniformVec4(QString name, const float *value, bool warn=true);
        void addUniformIVec2(QString name, const int *value, bool warn=true);
        void addUniformIVec3(QString name, const int *value, bool warn=true);
        void addUniformIVec4(QString name, const int *value, bool warn=true);
        void addUniformBVec2(QString name, const bool *value, bool warn=true);
        void addUniformBVec3(QString name, const bool *value, bool warn=true);
        void addUniformBVec4(QString name, const bool *value, bool warn=true);
        void addUniformMat2(QString name, const float *value, bool warn=true);      // les coefficients de la matrice comme succession de colonnes (column-major order)
        void addUniformMat3(QString name, const float *value, bool warn=true);      // les coefficients de la matrice comme succession de colonnes (column-major order)
        void addUniformMat4(QString name, const float *value, bool warn=true);      // les coefficients de la matrice comme succession de colonnes (column-major order)
        void addTexture(QString name, Texture *texture, bool warn=true);
        
    // méthodes pour changer la valeur d'une variable (a faire quand le programme est actif) :
    //----------------------------------------------------------------------------------------
        // si warn==true et la variable n'existe pas, un message de warning est affiché (utiliser warn=false pour les variables optionnelles)
        void setUniform(QString name, int   value, bool warn=true);
        void setUniform(QString name, float value, bool warn=true);
        void setUniform(QString name, bool value, bool warn=true);
        void setUniformVec2(QString name, float v0, float v1, bool warn=true);
        void setUniformVec3(QString name, float v0, float v1, float v2, bool warn=true);
        void setUniformVec4(QString name, float v0, float v1, float v2, float v3, bool warn=true);
        void setUniformVec2(QString name, const float *value, bool warn=true);
        void setUniformVec3(QString name, const float *value, bool warn=true);
        void setUniformVec4(QString name, const float *value, bool warn=true);
        void setUniformIVec2(QString name, int v0, int v1, bool warn=true);
        void setUniformIVec3(QString name, int v0, int v1, int v2, bool warn=true);
        void setUniformIVec4(QString name, int v0, int v1, int v2, int v3, bool warn=true);
        void setUniformIVec2(QString name, const int *value, bool warn=true);
        void setUniformIVec3(QString name, const int *value, bool warn=true);
        void setUniformIVec4(QString name, const int *value, bool warn=true);
        void setUniformBVec2(QString name, bool v0, bool v1, bool warn=true);
        void setUniformBVec3(QString name, bool v0, bool v1, bool v2, bool warn=true);
        void setUniformBVec4(QString name, bool v0, bool v1, bool v2, bool v3, bool warn=true);
        void setUniformBVec2(QString name, const bool *value, bool warn=true);
        void setUniformBVec3(QString name, const bool *value, bool warn=true);
        void setUniformBVec4(QString name, const bool *value, bool warn=true);
        void setUniformMat2(QString name, const float *value, bool warn=true);    // les coefficients de la matrice comme succession de colonnes (column-major order)
        void setUniformMat3(QString name, const float *value, bool warn=true);    // les coefficients de la matrice comme succession de colonnes (column-major order)
        void setUniformMat4(QString name, const float *value, bool warn=true);    // les coefficients de la matrice comme succession de colonnes (column-major order)
        
    // autres méthodes pratiques :
    //----------------------------
        // la liste des sources de type <shaderType> (noms de fichiers relatifs au répertoire) :
        static QStringList sourcesList(QString dirName, ShaderGLSL::Type shaderType);
        static QStringList sourcesList(QString dirName, ShaderGLSL::Type type, bool addPrefix = false, bool addExtension = false);

    private:
        void updateVariables() const;
        void clearParameters();
    private:
        UniformGLSL createUniformGLSL(QString name, bool warn) const;   // retourne NULL si la variable <name> n'existe pas
        void bindShaderDir(QString dirName, ShaderGLSL::Type shaderType);     // <dirName> relatif au répertoire de base

    public:
        // méthodes spécifiques :
        void validate();            // valide et affiche des infos de validation
        void printInfo() const;     // affiche la liste d'attributs et uniforms actifs
        void printActiveAttribs() const;    // affiche la liste d'attributs actifs
        void printActiveUniforms() const;   // affiche la liste d'uniforms actifs
        
        int getAttribID(QString name) const;    // retourne -1 si attribut invalide (gl_* ou inexistant)
        
    private:
        void attach(ShaderGLSL shader);
        void detach(ShaderGLSL shader);
        QString getInfoLog() const;
        GLint getVariable(GLenum paramName) const;
        static QString typeEnumString(GLenum type);     // convertit en QString une enum représentant un type GLSL

    public:
        class Locator {
            public:
                Locator(QString vertExt=".vert", QString fragExt=".frag", QString geomExt=".geom");
                
                // convenient methods to load a set of GLSL sources :
                //---------------------------------------------------
                    SourceSet sources(QString fileNamePrefix, bool loadGeometryShader = false) const;                      // files <fileNamePrefix>.vert and <fileNamePrefix>.frag
                    SourceSet sources(QString vertName, QString fragName) const;
		    SourceSet sources(QString vertName, QString fragName, QString geomName) const;
                    SourceSet sourcesDir(QString dirName, bool sub=false, bool loadGeometryShader = false) const;
                    SourceSet sourcesDir(ShaderGLSL::Type type, QString dirName, bool sub=false) const;   // same but restricted to a certain shader type
                    SourceSet sourcesAuto(QString fileName, ShaderGLSL::Type type) const;  // automatically gets sources from the other type(s) in the same directory
                    SourceSet sourcesAuto(Source source) const;  // automatically gets sources from the other type(s) in the same directory (<source> must have a reference fileName)
        
                    Source source(ShaderGLSL::Type type, QString fileName, bool addExt=false) const;
                    
                // other useful methods :
                //-----------------------
                    QStringList sourcesList(ShaderGLSL::Type type, bool addPrefix = false, bool addExt = false) const;
                    QStringList sourcesList(QString subDir, ShaderGLSL::Type type, bool addPrefix = false, bool addExt = false) const;
                
                    QString sourceName(QString fileName) const;                             // returns the absolute file name
                    QString sourceName(QString fileName, ShaderGLSL::Type type) const;      // the same but with the extension added as suffix

                    void addMapping(QString name, QString dir);     // adds a mapping for filenames with prefix [<name>] to directory <dir>
                    
            private:
                QString dirName(QString fileName) const;
                QString sameDirFileName(QString refFileName, QString localFileName) const;
                QString suffix(ShaderGLSL::Type type) const;
                QString withSuffix(QString fileName, ShaderGLSL::Type type) const;
                
                QStringList subDirs(QString dirName) const;
                QString getSubDir(QString dirName) const;   // ouvre une boîte de dialogue pour choisir un sous-répertoire de <dirName>
            
                bool fileExists(QString fileName) const;
                
            private:
                const QString vertExt;
                const QString fragExt;
		const QString geomExt;
                QHash<QString, QString> map;
                
            friend class Source;
            };
        
    private:
        // cette classe sert à représenter un couple (Uniform, UniformGLSL) :
        class Variable {
            public:
                // la suppression de <value> est prise en charge si et seulement si <ownValue> == true
                Variable(Uniform *value, UniformGLSL param, bool ownValue) : value(value), param(param), ownValue(ownValue) {}
                ~Variable() { if (ownValue) delete value; }
                
                void update() const { value->update(param); }
                
            private:
                Uniform *value;
                UniformGLSL param;
                const bool ownValue;
            };
            
    private:
        QList<Variable*> parameters;    // la liste des variables attachées au programme
        TextureSet textures;            // la liste des textures attachées au programme
        bool linked;
    private:
        GLuint id;
        bool created;
        QList<ShaderGLSL> shaders;      // gardés pour la destruction
    public:
        static Locator locator;
	
    private:
	// méthodes pour configurer le(s) geometry shader(s) :
	//----------------------------------------------------------------------------------------
	void setGeometryInputType( GLenum glInputType );
	void setGeometryOutputType( GLenum glOutputType );
	void setGeometryVerticesOut( GLint verticesOut );	
    private:
	GLenum glInputType;
	GLenum glOutputType;
	GLenum verticesOut;	
    };

BABLIB_NAMESPACE_END

#endif

