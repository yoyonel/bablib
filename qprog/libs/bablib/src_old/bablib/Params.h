#ifndef __PARAMS__
#define __PARAMS__

#include "Vec2.h"
#include "Vec3.h"
#include <QString>
#include <QStringList>
#include <QHash>

BABLIB_NAMESPACE_BEGIN

// format du fichier lu par ParamSet :
//------------------------------------
// cas de lignes possibles :
//    | <vide>
//    | // commentaires
//    | # commentaires
//    | <name> = <value>
//    | <name> = <value>  // commentaires
//    | <name> = <value>  # commentaires
// <name> et <value> ne contiennent ni espaces, ni la chaîne '//' ou '#'
// <name> contient au moins un caractère


// types supportés :
//------------------
//  int, float, double, bool,
//  std::string, QString,
//  Vec2, Vec3, Float3, UByte3, Float4,
//  GLenum,
//  QList<int>, QList<float>, QList<double>, QList<bool>, QList<QString>
        
// cette classe permet de charger un ensemble de paramètres à partir d'un fichier :
class ParamSet {
    public:
        void load(QString fileName);
        void reload();
        
        bool save(QString fileName) const;
        
        bool defined()              const { return !fileName.isEmpty(); }
        bool defines(QString name)  const { return table.contains(name); }
        
        QString getFileName() const { return fileName; }
        
        // méthodes pour obtenir la valeur d'un paramètre :
        template<class Object> Object get(QString name, Object def, int pos, bool warn = false) const;
        template<class Object> Object get(QString name, Object def, bool warn = false) const { return get<Object>(name, def, 0, warn); }
        template<class Object> Object get(QString name, int pos = 0) const { return get<Object>(name, defaultValue<Object>(), pos, true); }
        
        void add(QString name, QString value, bool overwrite=true);
        template<class Object> void add(QString name, Object value, bool overwrite=true);
        //@ make other methods taking non-qstring values
        
        void overwrite(const ParamSet &params);    // overwrites only common params by values defined in <params>
        void insert(const ParamSet &params, bool overwrite=true);
        
        void parseShaderLocations() const;      // convenient method interpreting every param beginning with 'shaders.location.' as a shader directory mapping
        
    private:
        template<class Object> static Object defaultValue();
        void errorFormat(QString name, QString format) const;
        void errorNotDef(QString name) const;
        QString getComponent(QString name, int pos = 0, int size = 1) const;    // returns the whole value iff pos == -1 (size has no effect in that case)
        QStringList getComponents(QString name, int size = 1) const;

    private:
        QHash<QString, QString> table;
        QString fileName;
    };

// cette classe permet d'accéder à un ensemble de paramètres globaux :
class Params {
    public:
        static void load(QString fileName = defaultFileName());
        static void reload();   // précondition : load a déja été appelé
        
        // the whole value is returned iff pos == -1
        template<class Object> static Object get(QString name, Object def, int pos, bool warn = false) { checkLoaded(); return params().get<Object>(name, def, pos, warn); }
        template<class Object> static Object get(QString name, Object def, bool warn = false)          { checkLoaded(); return params().get<Object>(name, def, warn); }
        template<class Object> static Object get(QString name, int pos = 0)                            { checkLoaded(); return params().get<Object>(name, pos); }
        
        bool defines(QString name)  const { checkLoaded(); return params().defines(name); }
        
        static QString getFileName() { checkLoaded(); return params().getFileName(); }
        static bool fileExists();
        
        // convenient method to automatically add directory mappings to ProgGLSL :
        // > parses params beginning by 'shaders.location.'
        static void parseShaderLocations();
        
    private:
        static void checkLoaded();

    private:
        static QString defaultFileName();
        static ParamSet& params();
        friend class GlobalParams;
    };

// cette classe permet d'accéder aux paramètres spécifiques aux classes bablib :
class GlobalParams {
    public:
        template<class Object> static Object get(QString name, Object def, int pos, bool warn = false) { checkLoaded(); return params().get<Object>(name, def, pos, warn); }
        template<class Object> static Object get(QString name, Object def, bool warn = false)          { checkLoaded(); return params().get<Object>(name, def, warn); }
        template<class Object> static Object get(QString name, int pos = 0)                            { checkLoaded(); return params().get<Object>(name, pos); }
        
        static QString getPath(QString name, bool warn = true);
        // reads a path potentially prefixed by <data> or <qprog>
        
    private:
        static void checkLoaded();
        static ParamSet& params();
    };

// macros pratiques pour recupérer des paramètres :
//-------------------------------------------------
#define PARAM_DEF(type, name)               type name = Params::get<type>(#name)
#define PARAM_SET(type, name)               name = Params::get<type>(#name)
#define PARAM_CALL2(function, type, name)   { type temp = Params::get<type>(#name); function(temp[0], temp[1]); }
#define PARAM_CALL3(function, type, name)   { type temp = Params::get<type>(#name); function(temp[0], temp[1], temp[2]); }
#define PARAM_CALL4(function, type, name)   { type temp = Params::get<type>(#name); function(temp[0], temp[1], temp[2], temp[3]); }
#define PARAM(type, name)                   Params::get<type>(#name)
#define PARAMi(type, name, i)               Params::get<type>(#name, i)
#define PARAM2(type, name)                  PARAMi(type,name,0), PARAMi(type,name,1)
#define PARAM3(type, name)                  PARAMi(type,name,0), PARAMi(type,name,1), PARAMi(type,name,2)
#define PARAM4(type, name)                  PARAMi(type,name,0), PARAMi(type,name,1), PARAMi(type,name,2), PARAMi(type,name,3)
#define PARAM5(type, name)                  PARAMi(type,name,0), PARAMi(type,name,1), PARAMi(type,name,2), PARAMi(type,name,3), PARAMi(type,name,4)
#define PARAM6(type, name)                  PARAMi(type,name,0), PARAMi(type,name,1), PARAMi(type,name,2), PARAMi(type,name,3), PARAMi(type,name,4), PARAMi(type,name,5)
#define glColor3f_PARAM(name)               PARAM_CALL3(glColor3f, Float3, name)
#define glColor4f_PARAM(name)               PARAM_CALL4(glColor4f, Float4, name)
#define PARAMi2(type, name, i)              Params::get<type>(#name,2*i+0), Params::get<type>(#name,2*i+1)
#define glEnable_PARAM(name, nameGL)        (PARAM(bool, name) ? glEnable : glDisable)(nameGL)
#define PARAM_ENUM(Enum, name)              Enum##Loader::get(PARAM(QString, name))
#define PARAM_ENUM_DEF(Enum, name)          Enum name = Enum##Loader::get(PARAM(QString, name))
//#define PARAM_GLSL_LOCATION(location_name, param_name) Params::addShadersLocation(#location_name, #param_name);
#define PARAM_AUTO_SHADER_LOCATIONS Params::parseShaderLocations();
#define if_PARAM(name) if (PARAM(bool, name))

BABLIB_NAMESPACE_END

#endif

