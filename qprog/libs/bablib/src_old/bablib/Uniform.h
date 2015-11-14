#ifndef __UNIFORM__
#define __UNIFORM__

#include <OpenGL.h>
#include "UniformGLSL.h"
#include <QString>

BABLIB_NAMESPACE_BEGIN

class Uniform {
    public:
        Uniform(QString name) : name(name) {}
        virtual ~Uniform() {}
        virtual void update(const UniformGLSL &u) const = 0;
    public:
        const QString name;
    };

///////////////////////////////////////////////////////////////////////////////////

class UniformSampler : public Uniform {
    public:
        UniformSampler(QString name, GLuint texID, int texNum) : Uniform(name), texID(texID), texNum(texNum) {}
        virtual void update(const UniformGLSL &u) const { u.setSampler(texID, texNum); }    // attache la texture a la variable sampler2D correspondante du shader
    private:
        GLuint texID;   // l'identifiant de la texture
        int texNum;     // numero de texture (correspondant au GL_TEXTURE<i>)
    };

///////////////////////////////////////////////////////////////////////////////////

template<class baseType, void (UniformGLSL::*setUniform)(const baseType*) const>
class UniformGen : public Uniform {
    public:
        UniformGen(QString name, const baseType *value) : Uniform(name), value(value) {}    // précondition : value != NULL
        virtual void update(const UniformGLSL &u) const { (u.*setUniform)(value); }         // précondition : u != NULL
    private:
        const baseType *value;
    };

typedef UniformGen<int,   &UniformGLSL::setUniform1iv>        UniformInt;
typedef UniformGen<int,   &UniformGLSL::setUniform2iv>        UniformIVec2;
typedef UniformGen<int,   &UniformGLSL::setUniform3iv>        UniformIVec3;
typedef UniformGen<int,   &UniformGLSL::setUniform4iv>        UniformIVec4;

typedef UniformGen<bool,   &UniformGLSL::setUniform1bv>       UniformBool;
typedef UniformGen<bool,   &UniformGLSL::setUniform2bv>       UniformBVec2;
typedef UniformGen<bool,   &UniformGLSL::setUniform3bv>       UniformBVec3;
typedef UniformGen<bool,   &UniformGLSL::setUniform4bv>       UniformBVec4;

typedef UniformGen<float, &UniformGLSL::setUniform1fv>        UniformFloat;
typedef UniformGen<float, &UniformGLSL::setUniform2fv>        UniformVec2;
typedef UniformGen<float, &UniformGLSL::setUniform3fv>        UniformVec3;
typedef UniformGen<float, &UniformGLSL::setUniform4fv>        UniformVec4;

typedef UniformGen<double, &UniformGLSL::setUniform1dv>       UniformDouble;

typedef UniformGen<float, &UniformGLSL::setUniformMatrix2fv>  UniformMat2;
typedef UniformGen<float, &UniformGLSL::setUniformMatrix3fv>  UniformMat3;
typedef UniformGen<float, &UniformGLSL::setUniformMatrix4fv>  UniformMat4;

///////////////////////////////////////////////////////////////////////////////////

/*
// version allouant de la mémoire :
template<class baseType, int N, void (UniformGLSL::*setUniform)(const baseType*) const>
class UniformGenMem : public Uniform {
    public:
        UniformGenMem(QString name) : Uniform(name), value(new baseType[N]) {}
        ~UniformGenMem() { delete[] value; }
        virtual void update(const UniformGLSL *u) const { (u->*setUniform)(value); }   // précondition : u != NULL
        baseType* operator*() { return value; }
    private:
        baseType *value;
    };

typedef UniformGenMem<int,    1, &UniformGLSL::setUniform1iv>        UnifInt;
typedef UniformGenMem<float,  1, &UniformGLSL::setUniform1fv>        UnifFloat;
typedef UniformGenMem<float,  2, &UniformGLSL::setUniform2fv>        UnifVec2;
typedef UniformGenMem<float,  3, &UniformGLSL::setUniform3fv>        UnifVec3;
typedef UniformGenMem<float,  4, &UniformGLSL::setUniform4fv>        UnifVec4;
typedef UniformGenMem<float,  4, &UniformGLSL::setUniformMatrix2fv>  UnifMat2;
typedef UniformGenMem<float,  9, &UniformGLSL::setUniformMatrix3fv>  UnifMat3;
typedef UniformGenMem<float, 16, &UniformGLSL::setUniformMatrix4fv>  UnifMat4;
*/

BABLIB_NAMESPACE_END

#endif

