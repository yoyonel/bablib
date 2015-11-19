#include <OpenGL.h>
#include "ProgGLSL.h"
#include "Uniform.h"
#include "UniformGLSL.h"
#include "Message.h"
#include "Params.h"
#include <QFile>
#include <QInputDialog>
BABLIB_NAMESPACE_USE
/**************************************************************************************/

namespace {
    QString toQS(ShaderGLSL::Type type) {
        return (type == ShaderGLSL::VERTEX) ? "vertex" : (type == ShaderGLSL::FRAGMENT) ? "fragment" : "geometry";
        }
    
    ProgGLSL::Locator createDefaultLocator() {
        ProgGLSL::Locator locator(".vert", ".frag", ".geom");
        locator.addMapping("shaders", GlobalParams::get<QString>("bablib.shaders.default.dir"));
        locator.addMapping("bablib",  GlobalParams::getPath("bablib.shaders.lib.dir"));
        return locator;
        }
    }

/**************************************************************************************/

ProgGLSL::Locator ProgGLSL::locator = createDefaultLocator();

/**************************************************************************************/

void ProgGLSL::bindShaderFile(QString fileName, ShaderGLSL::Type type) {
    ShaderGLSL shader(fileName, type, true);  //@@ faire verif sur l'existence du fichier (retourner un booleen)
    shader.compile();
    attach(shader);
    }

void ProgGLSL::bindShaderSource(QString source, ShaderGLSL::Type type) {
    ShaderGLSL shader(source, type, false);
    shader.compile();
    attach(shader);
    }

void ProgGLSL::bindShader(Source source) {
    if (source.empty()) return;
    ShaderGLSL shader(source.source, source.type, false, source.getName());
    //
    if (source.type == ShaderGLSL::GEOMETRY) {
	setGeometryInputType( glInputType );
	setGeometryOutputType( glOutputType );
	setGeometryVerticesOut( verticesOut );
	}
    //
    shader.compile();
    attach(shader);
    }

void ProgGLSL::bindShaders(SourceSet sources) {
    foreach (Source s, sources) bindShader(s);
    }
        
void ProgGLSL::create() {
    if (!created) {
        id = glCreateProgram();
        created = true;
        }
    }

void ProgGLSL::attach(ShaderGLSL shader) { glAttachShader(id, shader.id); shaders.append(shader); }
void ProgGLSL::detach(ShaderGLSL shader) { glDetachShader(id, shader.id); shaders.removeAll(shader); }

void ProgGLSL::link() {
    glLinkProgram(id);
    
    if (getVariable(GL_LINK_STATUS) == GL_FALSE)
        Message::error(QString("- erreur : l'edition de liens a echoue :\n%1").arg(getInfoLog()), false);
    else
        linked = true;
    }

/**************************************************************************************/

void ProgGLSL::validate() {
    glValidateProgram(id);
    
    if (getVariable(GL_VALIDATE_STATUS) == GL_FALSE)
        Message::info(QString("> programme non valide :\n%1").arg(getInfoLog()));
    else
        Message::info("> programme valide.");
    }

/**************************************************************************************/

GLint ProgGLSL::getVariable(GLenum paramName) const { int res; glGetProgramiv(id, paramName, &res); return res; }

QString ProgGLSL::getInfoLog() const {
    int logLength = getVariable(GL_INFO_LOG_LENGTH);
    char *infoLog = new char[logLength];
    glGetProgramInfoLog(id, logLength, NULL, infoLog);
    QString res(infoLog);
    delete infoLog;
    return res;
    }

void ProgGLSL::activate() {
    glUseProgram(id);
    updateVariables();
    textures.activate();
    }

void ProgGLSL::deactivate() {
    glUseProgram(0);
    textures.deactivate();
    }

void ProgGLSL::destroy() {
    if (created) {
        foreach (ShaderGLSL s, shaders) { detach(s); s.destroy(); }
        glDeleteProgram(id);
        clearParameters();
        created = false;
        }
    }

/**************************************************************************************/

int ProgGLSL::getAttribID(QString name) const {
    if (!isLinked()) {
        Message::error("le programme doit être linké avant de pouvoir accéder à ses attributs");
        return -1;
        }
    GLint attribID = glGetAttribLocation(id, name.toAscii().constData());
    if (attribID == -1)
        Message::error(QString("attribut invalide : %1").arg(name));
    return static_cast<int>(attribID);
    }

/**************************************************************************************/

void ProgGLSL::printInfo() const {
    Message::info("> attributs actifs :");
    printActiveAttribs();
    Message::info("> uniforms actifs :");
    printActiveUniforms();
    }
    
void ProgGLSL::printActiveAttribs() const {
    int numUniforms = getVariable(GL_ACTIVE_ATTRIBUTES);
    int maxLength   = getVariable(GL_ACTIVE_ATTRIBUTE_MAX_LENGTH);
    char *name = new char[maxLength];
    for (int i=0; i<numUniforms; i++) {
        int length, size;
        GLenum type;
        glGetActiveAttrib(id, i, maxLength, &length, &size, &type, name);
        QString t = typeEnumString(type);
        if (size > 1) t += QString("[%1]").arg(size);
        Message::info(QString("  - %1 %2").arg(t).arg(name));
        }
    delete[] name;
    }

void ProgGLSL::printActiveUniforms() const {
    int numUniforms = getVariable(GL_ACTIVE_UNIFORMS);
    int maxLength   = getVariable(GL_ACTIVE_UNIFORM_MAX_LENGTH);
    char *name = new char[maxLength];
    for (int i=0; i<numUniforms; i++) {
        int length, size;
        GLenum type;
        glGetActiveUniform(id, i, maxLength, &length, &size, &type, name);
        QString t = typeEnumString(type);
        if (size > 1) t += QString("[%1]").arg(size);
        Message::info(QString("  - %1 %2").arg(t).arg(name));
        }
    delete[] name;
    }

QString ProgGLSL::typeEnumString(GLenum type) {
    #define _(e) if (type == e) return #e;
    switch (type) {
        case GL_FLOAT       : return "float";
        case GL_FLOAT_VEC2  : return "vec2";
        case GL_FLOAT_VEC3  : return "vec3";
        case GL_FLOAT_VEC4  : return "vec4";
        case GL_INT         : return "int";
        case GL_INT_VEC2    : return "ivec2";
        case GL_INT_VEC3    : return "ivec3";
        case GL_INT_VEC4    : return "ivec4";
        case GL_BOOL        : return "bool";
        case GL_BOOL_VEC2   : return "bvec2";
        case GL_BOOL_VEC3   : return "bvec3";
        case GL_BOOL_VEC4   : return "bvec4";
        case GL_FLOAT_MAT2  : return "mat2";
        case GL_FLOAT_MAT3  : return "mat3";
        case GL_FLOAT_MAT4  : return "mat4";
        case GL_SAMPLER_1D  : return "sampler1D";
        case GL_SAMPLER_2D  : return "sampler2D";
        case GL_SAMPLER_3D  : return "sampler3D";          
        case GL_SAMPLER_CUBE        : return "samplerCube";
        case GL_SAMPLER_1D_SHADOW   : return "sampler1DShadow";
        case GL_SAMPLER_2D_SHADOW   : return "sampler2DShadow";
        default : return "<type inconnu>";
        }
    }

/**************************************************************************************/

void ProgGLSL::clearParameters() {
    textures.clear();
    while (!parameters.isEmpty()) delete parameters.takeFirst();
    }

void ProgGLSL::updateVariables()  const { foreach (Variable *param, parameters) param->update(); }
void ProgGLSL::activateTextures() const { textures.activate(); }

bool ProgGLSL::isLinked() const { return linked; }


/**************************************************************************************/

UniformGLSL ProgGLSL::createUniformGLSL(QString name, bool warn) const {
    UniformGLSL var(name, id);
    if (!var.exists() && warn)
        Message::error(QString("<%1> n'est pas une variable uniform active du programme").arg(name));
    return var;
    }

void ProgGLSL::addUniform(Uniform *uniform, bool warn, bool ownUniform) {
    UniformGLSL var = createUniformGLSL(uniform->name, warn);
    if (var.exists()) {
        Variable *p = new Variable(uniform, var, ownUniform);
        parameters.append(p);
        }
    }

void ProgGLSL::addUniform(QString name, const double *value, bool warn)    { addUniform(new UniformDouble(name, value), warn, true); }
void ProgGLSL::addUniform(QString name, const float *value, bool warn)     { addUniform(new UniformFloat(name,  value), warn, true); }
void ProgGLSL::addUniform(QString name, const bool *value, bool warn)      { addUniform(new UniformBool(name,   value), warn, true); }
void ProgGLSL::addUniform(QString name, const int *value, bool warn)       { addUniform(new UniformInt(name,    value), warn, true); }
void ProgGLSL::addUniformVec2(QString name, const float *value, bool warn) { addUniform(new UniformVec2(name,   value), warn, true); }
void ProgGLSL::addUniformVec3(QString name, const float *value, bool warn) { addUniform(new UniformVec3(name,   value), warn, true); }
void ProgGLSL::addUniformVec4(QString name, const float *value, bool warn) { addUniform(new UniformVec4(name,   value), warn, true); }
void ProgGLSL::addUniformIVec2(QString name, const int *value, bool warn)  { addUniform(new UniformIVec2(name,  value), warn, true); }
void ProgGLSL::addUniformIVec3(QString name, const int *value, bool warn)  { addUniform(new UniformIVec3(name,  value), warn, true); }
void ProgGLSL::addUniformIVec4(QString name, const int *value, bool warn)  { addUniform(new UniformIVec4(name,  value), warn, true); }
void ProgGLSL::addUniformBVec2(QString name, const bool *value, bool warn) { addUniform(new UniformBVec2(name,  value), warn, true); }
void ProgGLSL::addUniformBVec3(QString name, const bool *value, bool warn) { addUniform(new UniformBVec3(name,  value), warn, true); }
void ProgGLSL::addUniformBVec4(QString name, const bool *value, bool warn) { addUniform(new UniformBVec4(name,  value), warn, true); }
void ProgGLSL::addUniformMat2(QString name, const float *value, bool warn) { addUniform(new UniformMat2(name,   value), warn, true); }
void ProgGLSL::addUniformMat3(QString name, const float *value, bool warn) { addUniform(new UniformMat3(name,   value), warn, true); }
void ProgGLSL::addUniformMat4(QString name, const float *value, bool warn) { addUniform(new UniformMat4(name,   value), warn, true); }

/**************************************************************************************/

void ProgGLSL::addTexture(QString name, Texture *texture, bool warn) {
    int texNum = textures.add(texture);
    addUniform(new UniformSampler(name, texture->getID(), texNum), warn, true);
    }

/**************************************************************************************/

#define CREATE_PARAM_AND_CALL(functionCall) {           \
    UniformGLSL param = createUniformGLSL(name, warn);  \
    if (param.exists()) param.functionCall;             \
    }
    
void ProgGLSL::setUniform(QString name, float value, bool warn)                                  CREATE_PARAM_AND_CALL(setUniform1f(value))
void ProgGLSL::setUniform(QString name, int value, bool warn)                                    CREATE_PARAM_AND_CALL(setUniform1i(value))
void ProgGLSL::setUniform(QString name, bool value, bool warn)                                   CREATE_PARAM_AND_CALL(setUniform1b(value))

void ProgGLSL::setUniformVec2(QString name, float v0, float v1, bool warn)                       CREATE_PARAM_AND_CALL(setUniform2f(v0, v1))
void ProgGLSL::setUniformVec3(QString name, float v0, float v1, float v2, bool warn)             CREATE_PARAM_AND_CALL(setUniform3f(v0, v1, v2))
void ProgGLSL::setUniformVec4(QString name, float v0, float v1, float v2, float v3, bool warn)   CREATE_PARAM_AND_CALL(setUniform4f(v0, v1, v2, v3))
void ProgGLSL::setUniformVec2(QString name, const float *value, bool warn)                       CREATE_PARAM_AND_CALL(setUniform2fv(value))
void ProgGLSL::setUniformVec3(QString name, const float *value, bool warn)                       CREATE_PARAM_AND_CALL(setUniform3fv(value))
void ProgGLSL::setUniformVec4(QString name, const float *value, bool warn)                       CREATE_PARAM_AND_CALL(setUniform4fv(value))

void ProgGLSL::setUniformIVec2(QString name, int v0, int v1, bool warn)                          CREATE_PARAM_AND_CALL(setUniform2i(v0, v1))
void ProgGLSL::setUniformIVec3(QString name, int v0, int v1, int v2, bool warn)                  CREATE_PARAM_AND_CALL(setUniform3i(v0, v1, v2))
void ProgGLSL::setUniformIVec4(QString name, int v0, int v1, int v2, int v3, bool warn)          CREATE_PARAM_AND_CALL(setUniform4i(v0, v1, v2, v3))
void ProgGLSL::setUniformIVec2(QString name, const int *value, bool warn)                        CREATE_PARAM_AND_CALL(setUniform2iv(value))
void ProgGLSL::setUniformIVec3(QString name, const int *value, bool warn)                        CREATE_PARAM_AND_CALL(setUniform3iv(value))
void ProgGLSL::setUniformIVec4(QString name, const int *value, bool warn)                        CREATE_PARAM_AND_CALL(setUniform4iv(value))

void ProgGLSL::setUniformBVec2(QString name, bool v0, bool v1, bool warn)                        CREATE_PARAM_AND_CALL(setUniform2b(v0, v1))
void ProgGLSL::setUniformBVec3(QString name, bool v0, bool v1, bool v2, bool warn)               CREATE_PARAM_AND_CALL(setUniform3b(v0, v1, v2))
void ProgGLSL::setUniformBVec4(QString name, bool v0, bool v1, bool v2, bool v3, bool warn)      CREATE_PARAM_AND_CALL(setUniform4b(v0, v1, v2, v3))
void ProgGLSL::setUniformBVec2(QString name, const bool *value, bool warn)                       CREATE_PARAM_AND_CALL(setUniform2bv(value))
void ProgGLSL::setUniformBVec3(QString name, const bool *value, bool warn)                       CREATE_PARAM_AND_CALL(setUniform3bv(value))
void ProgGLSL::setUniformBVec4(QString name, const bool *value, bool warn)                       CREATE_PARAM_AND_CALL(setUniform4bv(value))

void ProgGLSL::setUniformMat2(QString name, const float *value, bool warn)                       CREATE_PARAM_AND_CALL(setUniformMatrix2fv(value))
void ProgGLSL::setUniformMat3(QString name, const float *value, bool warn)                       CREATE_PARAM_AND_CALL(setUniformMatrix3fv(value))
void ProgGLSL::setUniformMat4(QString name, const float *value, bool warn)                       CREATE_PARAM_AND_CALL(setUniformMatrix4fv(value))

/**************************************************************************************/
// methodes pour configurer le geometry shader :

void ProgGLSL::setGeometryInputType( GLenum glInputType ) {
	glProgramParameteriEXT( id, GL_GEOMETRY_INPUT_TYPE_EXT, glInputType );
}

void ProgGLSL::setGeometryOutputType( GLenum glOutputType ) {
	glProgramParameteriEXT( id, GL_GEOMETRY_OUTPUT_TYPE_EXT, glOutputType );
}

void ProgGLSL::setGeometryVerticesOut( GLint verticesOut ) {
	glProgramParameteriEXT( id, GL_GEOMETRY_VERTICES_OUT_EXT, verticesOut );
}

/**************************************************************************************/
// methodes de chargement de sources :

void ProgGLSL::load(SourceSet sources) {
    if (sources.empty()) {
        Message::error("no selected shader to bind");
        return;
        }
    create();
    bindShaders(sources);
    link();
    }

void ProgGLSL::loadDir(QString dirName, bool sub, bool loadGeometryShader, GLenum _glInputType, GLenum _glOutputType, GLint _verticesOut) {
	if (loadGeometryShader) {
		glInputType = _glInputType;
		glOutputType = _glOutputType;
		verticesOut = _verticesOut;
		}
	load(locator.sourcesDir(dirName, sub, loadGeometryShader)); 
	}

void ProgGLSL::load(QString name, bool loadGeometryShader)          		{ load(locator.sources(name, loadGeometryShader)); }
void ProgGLSL::load(QString vertName, QString fragName)             		{ load(locator.sources(vertName, fragName)); }
void ProgGLSL::load(QString vertName, QString fragName, QString geomName)       { load(locator.sources(vertName, fragName, geomName)); }
void ProgGLSL::loadAuto(QString fileName, ShaderGLSL::Type type)    		{ load(locator.sourcesAuto(fileName, type)); }
void ProgGLSL::loadAuto(Source source)                              		{ load(locator.sourcesAuto(source)); }
void ProgGLSL::load(Source s1, Source s2)                           		{ load(SourceSet() << s1 << s2); }
void ProgGLSL::load(Source s1, Source s2, Source s3)                		{ load(SourceSet() << s1 << s2 << s3); }

/**************************************************************************************/

QStringList ProgGLSL::sourcesList(QString dirName, ShaderGLSL::Type type, bool addPrefix, bool addExtension) {
    return locator.sourcesList(dirName, type, addPrefix, addExtension);
    }

QStringList ProgGLSL::sourcesList(QString dirName, ShaderGLSL::Type shaderType) {
    return ProgGLSL::sourcesList(dirName, shaderType, true, true);
    //@ extension : a voir
    }

void ProgGLSL::addLocation(QString name, QString dir) {
    locator.addMapping(name, dir);
    }
        
/**************************************************************************************/
// implementation of ProgGLSL::Source

//#include <Random.h>
ProgGLSL::Source::Source(ShaderGLSL::Type type) : type(type) {
    //this->source = QString("#line 0 %1\n").arg(Random::integer(100));
    }
                
ProgGLSL::Source::Source(ShaderGLSL::Type type, QString source, bool isFileName) : type(type) {
    //this->source = QString("#line 0 %1\n").arg(Random::integer(100));
    if (isFileName) addFile(source);
    else addSource(source);
    }

void ProgGLSL::Source::addFile(QString fileName, bool forceName) {
    fileName = ProgGLSL::locator.sourceName(fileName);      // to parse potential directory prefixes
    //@ dependency on ProgGLSL::locator.. --> maybe merge Locator and Source ?
    if (this->fileName.isEmpty() || forceName)
        this->fileName = fileName;
    QString sourceCode = readSourceFile(fileName);
    appendCode(sourceCode);
    }

void ProgGLSL::Source::addSource(QString code) {
    appendCode(code);
    }

void ProgGLSL::Source::addDefine(QString def, QString val) {
    appendCode(QString("#define %1 %2\n").arg(def).arg(val));
    }

QString ProgGLSL::Source::readSourceFile(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Message::error(QString("impossible d'ouvrir le fichier '%1'").arg(fileName));
        return QString::null;
        }
    QString res = QTextStream(&file).readAll();
    file.close();
    return res;
    }

QString ProgGLSL::Source::separatorCode() {
    return "\n#line 0\n";   // reinitializes the line counter
    //@@ utiliser la version #line <line> <source-string-number>
    //@@ pour gerer les includes + messages d'erreur..
    }

void ProgGLSL::Source::appendCode(QString code) {
    if (code.isEmpty()) return;
    if (!source.isEmpty()) source += separatorCode();
    source += preprocess(code);
    }

QString ProgGLSL::Source::preprocess(QString code) const {
    QStringList lines = code.split('\n');   //@ not file format independent? --> use QTextStream(code)?
    QString newCode = QString::null;
    bool in = true;
    int n =  0;
    foreach (QString line, lines) {
        QString s = line.trimmed();
        if (in) {
            const QString prefix = "#bablib_include";
            if (s.startsWith(prefix)) {
                QString p = s.remove(0, prefix.size()).trimmed();
                
                #define SYNTAX_ERROR(message) { error = true; MSG_ERROR("syntax error for bablib_include on line %1 : %2", (n)(message)); }

                bool global = false;
                bool error = false;
                int j = -1;
                if (p.startsWith('"')) { global = false;    j = p.indexOf('"', 1); } else
                if (p.startsWith('<')) { global = true;     j = p.indexOf('>', 1); } else SYNTAX_ERROR("no file name provided")
                
                QString include = QString::null;
                QString lineEnd = QString::null;
                if (j != -1) {
                    if (j > 1)
                        include = p.mid(1,j-1);
                    else
                        SYNTAX_ERROR("empty file name")
                    lineEnd = p.remove(0, j+1).trimmed();
                    }
                else if (!error)
                    SYNTAX_ERROR("missing closing bracket")
                
                while (!lineEnd.isEmpty()) {
                    if (lineEnd.startsWith("//")) lineEnd = QString::null;
                    else if (lineEnd.startsWith("/*")) {
                        int k = lineEnd.indexOf("*/");
                        if (k == -1) in = false;
                        else lineEnd = lineEnd.remove(0, k+2);
                        }
                    else {
                        SYNTAX_ERROR("nothing else than comments should appear on an include line")
                        break;
                        }
                    }
                
                if (!error) {
                    QString includeFileName = include;
                    // by default check if a file exists with name <include> in the same directory as the current source:
                    if (!global && hasFileName()) {
                        QString relFileName = locator.sameDirFileName(fileName, include);       //@ ! check that fileName belongs to locator
                        if (locator.fileExists(relFileName)) includeFileName = relFileName;
                        }
                    if (locator.fileExists(includeFileName)) {
                        QString includeCode = readSourceFile(locator.sourceName(includeFileName));
                        //@ keep track of current source / included source with compiler indices mechanism
                        //  i.e. #line 0 <source-string-number>
                        //  with <source-string-number> = arbitrary number identifying the source
                        newCode += "#line 0\n" + includeCode + "\n";  // switch to included source's line numbering
                        if (!in) newCode += "/*\n";
                        //newCode += lineEnd + "\n";
                        newCode += QString("#line %1\n").arg(n+1);    // back to current source's line numbering
                        }
                    else
                        MSG_ERROR("included file not found : '%1'", (includeFileName))
                    }
                
                if (error)
                    newCode += line + "\n";
                }
            else {
                newCode += line + "\n";
                int i = s.indexOf("/*");
                while (i != -1) {
                    in = !in;
                    i = s.indexOf(in ? "/*" : "*/", i+2);
                    }
                }
            }
        else {
            newCode += line + "\n";
            int i = s.indexOf("*/");
            while (i != -1) {
                in = !in;
                i = s.indexOf(in ? "/*" : "*/", i+2);
                }
            }
        n++;
        }
    
    return newCode;
    }

QString ProgGLSL::Source::getName() const {
    return fileName.isEmpty() ? "<unknown>" : fileName;
    }

bool ProgGLSL::Source::empty() const {
    return source.isEmpty();
    }

bool ProgGLSL::Source::hasFileName() const {
    return !fileName.isEmpty();
    }

/**************************************************************************************/
// implementation of ProgGLSL::Locator

ProgGLSL::Locator::Locator(QString vertExt, QString fragExt, QString geomExt) :
    vertExt(vertExt), fragExt(fragExt), geomExt(geomExt) {
    }
    
void ProgGLSL::Locator::addMapping(QString name, QString dir) {
    map.insert(name, dir);
    }
                
ProgGLSL::SourceSet ProgGLSL::Locator::sources(QString fileNamePrefix, bool loadGeometryShader) const {
    SourceSet res;
    res << Source(ShaderGLSL::VERTEX,   sourceName(fileNamePrefix, ShaderGLSL::VERTEX));
    res << Source(ShaderGLSL::FRAGMENT, sourceName(fileNamePrefix, ShaderGLSL::FRAGMENT));
    if (loadGeometryShader)
	res << Source(ShaderGLSL::GEOMETRY, sourceName(fileNamePrefix, ShaderGLSL::GEOMETRY));
    return res;
    }

ProgGLSL::SourceSet ProgGLSL::Locator::sources(QString vertName, QString fragName) const {
    SourceSet res;
    res << Source(ShaderGLSL::VERTEX  , sourceName(vertName));
    res << Source(ShaderGLSL::FRAGMENT, sourceName(fragName));    
    return res;
    }

ProgGLSL::SourceSet ProgGLSL::Locator::sources(QString vertName, QString fragName, QString geomName) const {
    SourceSet res = sources(vertName, fragName);
    res << Source(ShaderGLSL::GEOMETRY, sourceName(geomName));
    return res;
    }

ProgGLSL::SourceSet ProgGLSL::Locator::sourcesDir(QString dirName, bool sub, bool loadGeometryShader) const {
    if (sub) {
        QString subDirName = getSubDir(dirName.isEmpty() ? "." : dirName);
        if (subDirName.isEmpty()) return SourceSet();
        if (!dirName.endsWith("/")) dirName += "/";
        dirName += subDirName;
        }
    SourceSet res = sourcesDir(ShaderGLSL::VERTEX, dirName) + sourcesDir(ShaderGLSL::FRAGMENT, dirName);
    if (loadGeometryShader) res += sourcesDir(ShaderGLSL::GEOMETRY, dirName);		
    return res;
    }

ProgGLSL::SourceSet ProgGLSL::Locator::sourcesDir(ShaderGLSL::Type type, QString dirName, bool sub) const {
    if (sub) {
        QString subDirName = getSubDir(dirName.isEmpty() ? "." : dirName);
        if (subDirName.isEmpty()) return SourceSet();
        if (!dirName.endsWith("/")) dirName += "/";
        dirName += subDirName;
        }
    QStringList list = sourcesList(dirName, type, true, true);
    SourceSet res;
    if (list.isEmpty())
        Message::error(QString("pas de %1 shader dans le repertoire '%2'").arg(toQS(type)).arg(dirName));
    else foreach (QString s, list)
        res << Source(type, sourceName(s));
    return res;
    }

ProgGLSL::SourceSet ProgGLSL::Locator::sourcesAuto(QString fileName, ShaderGLSL::Type type) const {
    ShaderGLSL::Type typeAuto = (type == ShaderGLSL::VERTEX) ? ShaderGLSL::FRAGMENT : ShaderGLSL::VERTEX;
    
    QString subDirName = dirName(fileName);
    QStringList list = sourcesList(subDirName, typeAuto, true, true);
    
    SourceSet res;
    if (list.size() == 1) {
        res << Source(type    , sourceName(fileName));
        res << Source(typeAuto, sourceName(list.first()));
        }
    else
        Message::error(QString("le repertoire '%1' doit contenir un unique %2 shader").arg(subDirName).arg(toQS(typeAuto)));
    return res;
    }

ProgGLSL::SourceSet ProgGLSL::Locator::sourcesAuto(Source source) const {
    SourceSet res;
    if (!source.hasFileName()) {
        Message::error("the provided source does not have a reference file name");
        return res;
        }
    ShaderGLSL::Type typeAuto = (source.getType() == ShaderGLSL::VERTEX) ? ShaderGLSL::FRAGMENT : ShaderGLSL::VERTEX;
    res << source;
    res << sourcesDir(typeAuto, dirName(source.getName()));
    return res;
    }
                    
ProgGLSL::Source ProgGLSL::Locator::source(ShaderGLSL::Type type, QString fileName, bool addExt) const {
    return Source(type, addExt ? sourceName(fileName, type) : sourceName(fileName));
    }

/**************************************************************************************/

QString ProgGLSL::Locator::sourceName(QString fileName) const {
    if (fileName.startsWith("[")) {
        int i = fileName.indexOf("]");
        if (i > 0) {
            QString key = fileName.mid(1, i-1);
            if (map.contains(key)) {
                return map[key] + fileName.mid(i+1);
                }
            else
                MSG_ERROR("unknown shaders directory shortcut : '%1'", (key))
            }
        else
            MSG_ERROR("unmatched opening bracket in shader file name : '%1'", (fileName))
        }
    return fileName;    //@@ maybe recursively repeat until no bracket prefix: to allow key names to be defined with other key names as prefixes
    }

QString ProgGLSL::Locator::sourceName(QString fileName, ShaderGLSL::Type type)  const { return withSuffix(sourceName(fileName), type); }
QString ProgGLSL::Locator::suffix(ShaderGLSL::Type type)                        const { return type == ShaderGLSL::VERTEX ? vertExt : type == ShaderGLSL::FRAGMENT ? fragExt : geomExt; }
QString ProgGLSL::Locator::withSuffix(QString fileName, ShaderGLSL::Type type)  const { return fileName + suffix(type); }

QString ProgGLSL::Locator::dirName(QString fileName) const {
    int i = fileName.lastIndexOf("/");
    return (i == -1 ? "." : fileName.left(i));
    }

QString ProgGLSL::Locator::sameDirFileName(QString refFileName, QString localFileName) const {
    QString d = dirName(refFileName);
    if (!d.isEmpty() && !d.endsWith("/")) d += "/";
    return d + localFileName;
    }

bool ProgGLSL::Locator::fileExists(QString fileName) const {
    return QFile(sourceName(fileName)).exists();
    }

QStringList ProgGLSL::Locator::sourcesList(QString subDirName, ShaderGLSL::Type type, bool addPrefix, bool addExt) const {
    Dir subDir(sourceName(subDirName));
    QString filter = "*" + suffix(type);
    QStringList fileList = subDir.getFileList(filter);
    
    QString prefix = sourceName(subDirName);
    if (!prefix.endsWith("/")) prefix += "/";
    
    if (addPrefix || !addExt) {
        const int n = suffix(type).length();
        for (int i=0; i<fileList.size(); i++) {
            if (addPrefix) fileList[i] = prefix + fileList[i];
            if (!addExt)   fileList[i].chop(n);
            }
        }
    
    return fileList;
    }

QStringList ProgGLSL::Locator::sourcesList(ShaderGLSL::Type type, bool addPrefix, bool addExt) const {
    return sourcesList(".", type, addPrefix, addExt);
    }

QStringList ProgGLSL::Locator::subDirs(QString dirName) const {
    return Dir(sourceName(dirName)).getDirList();
    }

QString ProgGLSL::Locator::getSubDir(QString dirName) const {
    QStringList list = subDirs(dirName);
    bool ok;
    const QString subDirName = QInputDialog::getItem(NULL, "Repertoires de shaders", "Choix", list, 0, false, &ok);
    return ok ? subDirName : QString::null;
    }

