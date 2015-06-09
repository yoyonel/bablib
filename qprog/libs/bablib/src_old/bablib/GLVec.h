#ifndef __GL_VEC__
#define __GL_VEC__

#include "MacroTools.h"

// struct GLATOM_PARAM : types atomiques GL
#define GLATOM_STRUCT_byte     (GLbyte     , Byte  , b , GL_BYTE           )
#define GLATOM_STRUCT_short    (GLshort    , Short , s , GL_SHORT          )
#define GLATOM_STRUCT_int      (GLint      , Int   , i , GL_INT            )
#define GLATOM_STRUCT_float    (GLfloat    , Float , f , GL_FLOAT          )
#define GLATOM_STRUCT_double   (GLdouble   , Double, d , GL_DOUBLE         )
#define GLATOM_STRUCT_ubyte    (GLubyte    , UByte , ub, GL_UNSIGNED_BYTE  )
#define GLATOM_STRUCT_ushort   (GLushort   , UShort, us, GL_UNSIGNED_SHORT )
#define GLATOM_STRUCT_uint     (GLuint     , UInt  , ui, GL_UNSIGNED_INT   )
#define GLATOM_STRUCT_bool     (GLboolean  , Bool  ,   ,                   )

/*
// pareil avec les types c++ correspondant aux types GL :
#define GLATOM_STRUCT_byte     (signed char    , Byte  , b , GL_BYTE           )
#define GLATOM_STRUCT_short    (short          , Short , s , GL_SHORT          )
#define GLATOM_STRUCT_int      (int            , Int   , i , GL_INT            )
#define GLATOM_STRUCT_float    (float          , Float , f , GL_FLOAT          )
#define GLATOM_STRUCT_double   (double         , Double, d , GL_DOUBLE         )
#define GLATOM_STRUCT_ubyte    (unsigned char  , UByte , ub, GL_UNSIGNED_BYTE  )
#define GLATOM_STRUCT_ushort   (unsigned short , UShort, us, GL_UNSIGNED_SHORT )
#define GLATOM_STRUCT_uint     (unsigned int   , UInt  , ui, GL_UNSIGNED_INT   )
#define GLATOM_STRUCT_bool     (unsigned char  , Bool  ,   ,                   )
*/

#define GLATOM_MEMBER(type, i) BOOST_PP_TUPLE_ELEM(4, i, GLATOM_STRUCT_ ## type)
#define GLATOM_TYPE(type)     GLATOM_MEMBER(type, 0)
#define GLATOM_MAJ(type)      GLATOM_MEMBER(type, 1)
#define GLATOM_SUFFIX(type)   GLATOM_MEMBER(type, 2)
#define GLATOM_GLENUM(type)   GLATOM_MEMBER(type, 3)

//@ pas top, à voir
#define GLVEC_NAME(type, channels)      CONCAT3(Vec, GLATOM_MAJ(type), channels)
//#define GLVEC_NAME(type, channels)      CONCAT3(Vec, channels, GLATOM_SUFFIX(type))
//#define GLVEC_NAME(type, channels)      CONCAT2(GLATOM_MAJ(type), channels)
#define GLVEC_SUFFIX(type, channels)    CONCAT2(channels, GLATOM_SUFFIX(type))

#define GLVEC_AFFECT_ARRAY(i)     this->v[i] = v[i];

#define GLVEC_CLASS(type, channels)                             \
    class GLVEC_NAME(type, channels) {                          \
        public:                                                 \
            GLVEC_NAME(type, channels)(GLATOM_TYPE(type) *v) {  \
                FOR_INT(0, channels, GLVEC_AFFECT_ARRAY)        \
                }                                               \
        public:                                                 \
            GLATOM_TYPE(type) v[channels];                      \
        };


// declaration of the set of all possible compouned types :
//---------------------------------------------------------
    // t = atomic type
    #define GLVEC_CLASS_MULTI(type) \
        GLVEC_CLASS(type, 1)        \
        GLVEC_CLASS(type, 2)        \
        GLVEC_CLASS(type, 3)        \
        GLVEC_CLASS(type, 4)
    
    #define GLVEC_CLASS_MULTI_MACRO(r,data,t) GLVEC_CLASS_MULTI(t)

    BOOST_PP_SEQ_FOR_EACH(GLVEC_CLASS_MULTI_MACRO, ~,
        (byte)(short)(int)(float)(double)(ubyte)(ushort)(uint)(bool)
        )

        
#endif

