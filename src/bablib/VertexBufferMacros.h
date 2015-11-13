#ifndef __VERTEX_BUFFER_MACROS__
#define __VERTEX_BUFFER_MACROS__

#include "GLVec.h"
#include "MacroTools.h"

#define VERTEX_ARRAY_INTERLEAVED
// usually more efficient interleaved if one same buffer is used to store all attribs

// this abstract class is not really necessary
//@ maybe add the GLVec* type as template param to move most methods up here..
class AbstractVertexAttrib {
    public:
        void setOffset(unsigned int o) { offset = o; }
        virtual void setBuffer(void *b) = 0;
        virtual int size() = 0;
        virtual void setPointer(void *) = 0;
    public:
        unsigned int offset;
    };

// struct ATTRIB_PARAM :
// (member name, GLenum, glSet function type (MULTI/UNARY), glPointer args number (2/3/4/6), meta type (GEN/TEX/VA))
#define ATTRIB_STRUCT_Vertex          (vertex        , GL_VERTEX_ARRAY           , MULTI, 4, GEN)
#define ATTRIB_STRUCT_Normal          (normal        , GL_NORMAL_ARRAY           , MULTI, 3, GEN)
#define ATTRIB_STRUCT_Color           (color         , GL_COLOR_ARRAY            , MULTI, 4, GEN)
#define ATTRIB_STRUCT_SecondaryColor  (secondaryColor, GL_SECONDARY_COLOR_ARRAY  , MULTI, 4, GEN)
#define ATTRIB_STRUCT_Index           (index         , GL_INDEX_ARRAY            , UNARY, 3, GEN)
#define ATTRIB_STRUCT_FogCoord        (fogCoord      , GL_FOG_COORD_ARRAY        , UNARY, 3, GEN)
#define ATTRIB_STRUCT_EdgeFlag        (edgeFlag      , GL_EDGE_FLAG_ARRAY        , UNARY, 2, GEN)
#define ATTRIB_STRUCT_TexCoord        (texCoord      , GL_TEXTURE_COORD_ARRAY    , MULTI, 4, TEX)
#define ATTRIB_STRUCT_VertexAttrib    (vertexAttrib  ,                           , MULTI, 6, VA )

#define ATTRIB_MEMBER(attrib, i) BOOST_PP_TUPLE_ELEM(5, i, ATTRIB_STRUCT_ ## attrib)
#define ATTRIB_NAME(attrib)           ATTRIB_MEMBER(attrib, 0)
#define ATTRIB_GLENUM(attrib)         ATTRIB_MEMBER(attrib, 1)
#define ATTRIB_SET_FUNC_TYPE(attrib)  ATTRIB_MEMBER(attrib, 2)
#define ATTRIB_PTR_FUNC_ARGS(attrib)  ATTRIB_MEMBER(attrib, 3)
#define ATTRIB_META_TYPE(attrib)      ATTRIB_MEMBER(attrib, 4)

#define ATTRIB_SET_FUNC_MULTI(attrib, type, channels) CONCAT4(gl, attrib, GLVEC_SUFFIX(type,channels), v)
#define ATTRIB_SET_FUNC_UNARY(attrib, type, channels) CONCAT4(gl, attrib, GLATOM_SUFFIX(type), v)

#define ATTRIB_SET_FUNC(attrib, type, channels) \
    CONCAT2(ATTRIB_SET_FUNC_, ATTRIB_SET_FUNC_TYPE(attrib))(attrib, type, channels)

#define ATTRIB_PTR_FUNC_NAME(attrib) CONCAT3(gl, attrib, Pointer)

#ifdef  VERTEX_ARRAY_INTERLEAVED
    #define ATTRIB_PTR_FUNC_ARGS_4(attrib, type, channels)   (channels, GLATOM_GLENUM(type), stride, ptr)
    #define ATTRIB_PTR_FUNC_ARGS_3(attrib, type, channels)   (GLATOM_GLENUM(type), stride, ptr)                              // <channels> implicite
    #define ATTRIB_PTR_FUNC_ARGS_2(attrib, type, channels)   (stride, ptr)                                                   // <channels> et <type> implicites
    #define ATTRIB_PTR_FUNC_ARGS_6(attrib, type, channels)   (attribID, channels, GLATOM_GLENUM(type), false, stride, ptr)   // arguments de glVertexAttribPointer
#else
    #define ATTRIB_PTR_FUNC_ARGS_4(attrib, type, channels)   (channels, GLATOM_GLENUM(type), 0, ptr)
    #define ATTRIB_PTR_FUNC_ARGS_3(attrib, type, channels)   (GLATOM_GLENUM(type), 0, ptr)                              // <channels> implicite
    #define ATTRIB_PTR_FUNC_ARGS_2(attrib, type, channels)   (0, ptr)                                                   // <channels> et <type> implicites
    #define ATTRIB_PTR_FUNC_ARGS_6(attrib, type, channels)   (attribID, channels, GLATOM_GLENUM(type), false, 0, ptr)   // arguments de glVertexAttribPointer
#endif

#define ATTRIB_PTR_FUNC(attrib, type, channels) \
    ATTRIB_PTR_FUNC_NAME(attrib)CONCAT2(ATTRIB_PTR_FUNC_ARGS_, ATTRIB_PTR_FUNC_ARGS(attrib))(attrib, type, channels)

#define VA_CLASS_NAME(attrib, type, channels) CONCAT3(VA, attrib, GLVEC_NAME(type, channels))

// sous-parties generiques de la macro DEF_VERTEX_ATTRIB_TYPE, dÃÂ©pendant du mÃÂ©ta type d'<attrib> :
#define VA_DECL_SET_GL(attrib, type, channels)  CONCAT2(VA_DECL_SET_GL_, ATTRIB_META_TYPE(attrib))(attrib, type, channels)
#define VA_DECL_CONSTR(attrib, type, channels)  CONCAT2(VA_DECL_CONSTR_, ATTRIB_META_TYPE(attrib))(attrib, type, channels)
#define VA_DECL_MEMBER(attrib)      CONCAT2(VA_DECL_MEMBER_, ATTRIB_META_TYPE(attrib))(attrib)
#define VA_ENABLE_PTR(attrib)       CONCAT2(VA_ENABLE_PTR_,  ATTRIB_META_TYPE(attrib))(attrib)
#define VA_DECL_SET_PROG(attrib)    CONCAT2(VA_DECL_SET_PROG_, ATTRIB_META_TYPE(attrib))(attrib)

// corps de la fonction setGL
#define VA_DECL_SET_GL_GEN(attrib, type, channels)  ATTRIB_SET_FUNC(attrib, type, channels)(v)
#define VA_DECL_SET_GL_TEX(attrib, type, channels)  CONCAT4(gl, MultiTexCoord, GLVEC_SUFFIX(type,channels), v)(tex, v)
#define VA_DECL_SET_GL_VA(attrib, type, channels)   ATTRIB_SET_FUNC(attrib, type, channels)(attribID, v)

// dÃÂ©claration ÃÂ©ventuelle d'un constructeur
#define VA_DECL_CONSTR_GEN(attrib, type, channels)  
#define VA_DECL_CONSTR_TEX(attrib, type, channels)  VA_CLASS_NAME(TexCoord, type, channels)(int tex=0) : tex(GL_TEXTURE0 + tex) {}
#define VA_DECL_CONSTR_VA(attrib, type, channels)   VA_CLASS_NAME(VertexAttrib, type, channels)(GLuint attribID=0) : attribID(attribID) {}

// dÃÂ©claration ÃÂ©ventuelle de membres
#define VA_DECL_MEMBER_GEN(attrib)  
#define VA_DECL_MEMBER_TEX(attrib)  GLenum tex;
#define VA_DECL_MEMBER_VA(attrib)   GLuint attribID;

// enables GL avant le gl***Pointer
#define VA_ENABLE_PTR_GEN(attrib)   glEnableClientState(ATTRIB_GLENUM(attrib))
#define VA_ENABLE_PTR_TEX(attrib)   glClientActiveTexture(tex); glEnableClientState(GL_TEXTURE_COORD_ARRAY)
#define VA_ENABLE_PTR_VA(attrib)    glEnableVertexAttribArray(attribID)

// declaration de la mÃÂ©thode setProg
#define VA_DECL_SET_PROG_GEN(attrib)    void setProg(const ProgGLSL&, const char*) {}
#define VA_DECL_SET_PROG_TEX(attrib)    void setProg(const ProgGLSL&, const char*) {}
#define VA_DECL_SET_PROG_VA(attrib)     void setProg(const ProgGLSL& prog, const char* name) { attribID = prog.getAttribID(name); }

#ifdef  VERTEX_ARRAY_INTERLEAVED
    // attribs prÃÂ©dÃÂ©finis d'opengl : Vertex, Normal, Color, SecondaryColor, Index, [EdgeFlag], FogCoord
    #define DEF_VERTEX_ATTRIB_TYPE(attrib, type, channels)                                                  \
        class VA_CLASS_NAME(attrib, type, channels) : public AbstractVertexAttrib {                         \
            public:                                                                                         \
                VA_DECL_CONSTR(attrib, type, channels)                                                      \
                virtual int size() { return channels * sizeof(GLATOM_TYPE(type)); }                         \
                void setGL(GLATOM_TYPE(type) *v) { VA_DECL_SET_GL(attrib, type, channels); }                \
                virtual void setPointer(void *ptr) {                                                        \
                    VA_ENABLE_PTR(attrib);                                                                  \
                    ATTRIB_PTR_FUNC(attrib, type, channels);                                                \
                    }                                                                                       \
                void set(int i, GLATOM_TYPE(type) *v) { reinterpret_cast<GLVEC_NAME(type,channels)*>(mem + stride * i)[0] = GLVEC_NAME(type,channels)(v); }            \
                virtual void setBuffer(void *b) { mem =  reinterpret_cast<char*>(b); }                      \
                VA_DECL_SET_PROG(attrib)                                                                    \
                void setStride(int stride) { this->stride = stride; }                                       \
            public:                                                                                         \
                VA_DECL_MEMBER(attrib)                                                                      \
                char *mem;                                                                                  \
                int stride;     \
            };
#else
    // attribs prÃÂ©dÃÂ©finis d'opengl : Vertex, Normal, Color, SecondaryColor, Index, [EdgeFlag], FogCoord
    #define DEF_VERTEX_ATTRIB_TYPE(attrib, type, channels)                                                  \
        class VA_CLASS_NAME(attrib, type, channels) : public AbstractVertexAttrib {                         \
            public:                                                                                         \
                VA_DECL_CONSTR(attrib, type, channels)                                                      \
                virtual int size() { return channels * sizeof(GLATOM_TYPE(type)); }                         \
                void setGL(GLATOM_TYPE(type) *v) { VA_DECL_SET_GL(attrib, type, channels); }                \
                virtual void setPointer(void *ptr) {                                                        \
                    VA_ENABLE_PTR(attrib);                                                                  \
                    ATTRIB_PTR_FUNC(attrib, type, channels);                                                \
                    }                                                                                       \
                void set(int i, GLATOM_TYPE(type) *v) { mem[i] = GLVEC_NAME(type,channels)(v); }            \
                virtual void setBuffer(void *b) { mem = reinterpret_cast<GLVEC_NAME(type,channels)*>(b); }  \
                VA_DECL_SET_PROG(attrib)                                                                    \
            public:                                                                                         \
                VA_DECL_MEMBER(attrib)                                                                      \
                GLVEC_NAME(type, channels) *mem;                                                            \
            };
#endif
        
// declaration of the set of all possible vertex attributes, with all possible associated types :
//-----------------------------------------------------------------------------------------------

    // product = (attrib)(type)(channels)
    #define DEF_VA_MULTI_MACRO(r, product) DEF_VERTEX_ATTRIB_TYPE(BOOST_PP_SEQ_ELEM(0, product), BOOST_PP_SEQ_ELEM(1, product), BOOST_PP_SEQ_ELEM(2, product))
    #define DEF_VERTEX_ATTRIB_TYPES(attrib, seqChannels, seqTypes) \
        BOOST_PP_SEQ_FOR_EACH_PRODUCT(DEF_VA_MULTI_MACRO, ((attrib))(seqTypes)(seqChannels))
    
    DEF_VERTEX_ATTRIB_TYPES(Vertex          , (2)(3)(4)     , (short)(int)(float)(double)                           )
    DEF_VERTEX_ATTRIB_TYPES(Normal          , (3)           , (byte)(short)(int)(float)(double)                     )
    DEF_VERTEX_ATTRIB_TYPES(Color           , (3)(4)        , (byte)(ubyte)(short)(ushort)(int)(uint)(float)(double))
    DEF_VERTEX_ATTRIB_TYPES(SecondaryColor  , (3)           , (byte)(ubyte)(short)(ushort)(int)(uint)(float)(double))
    DEF_VERTEX_ATTRIB_TYPES(Index           , (1)           , (ubyte)(short)(int)(float)(double)                    )
    DEF_VERTEX_ATTRIB_TYPES(FogCoord        , (1)           , (float)(double)                                       )
    DEF_VERTEX_ATTRIB_TYPES(EdgeFlag        , (1)           , (bool)                                                )
    DEF_VERTEX_ATTRIB_TYPES(TexCoord        , (1)(2)(3)(4)  , (short)(int)(float)(double)                           )
    DEF_VERTEX_ATTRIB_TYPES(VertexAttrib    , (1)(2)(3)     , (short)(float)(double)                                )
    DEF_VERTEX_ATTRIB_TYPES(VertexAttrib    , (4)           , (byte)(ubyte)(short)(ushort)(int)(uint)(float)(double))
    
    // sÃÂ©paration en 2 car glVertexAttrib{123}{bi ubusui} n'existent pas
    // a voir : limite les possibilites de vertex attribs (si setGL pas utilisÃÂ©) --> definir des macros glVertexAttrib2i, etc. ?

#define ATTRIB_SEQ_MACRO(r,data,t) data t
#define ATTRIB_SEQ_APPLY(macro, seq) BOOST_PP_SEQ_FOR_EACH(ATTRIB_SEQ_MACRO, macro, seq)

#ifdef VERTEX_ARRAY_INTERLEAVED
    #define DECL_ATTRIB(attrib_class, attrib_name, attrib_init)     attrib_class attrib_name;
    #define ADD_UNIT_SIZE(attrib_class,  attrib_name, attrib_init)  unit += attrib_name.size();
    #define INIT_ATTRIB(attrib_class, attrib_name, attrib_init)     attrib_name = attrib_class( attrib_init );
    #define LINK_ATTRIB(attrib_class, attrib_name, attrib_init)     attrib_name.setOffset(offset); attrib_name.setStride(unit); attrib_name.setBuffer(dataPtr + offset); offset += attrib_name.size();
    #define SET_POINTER(attrib_class, attrib_name, attrib_init)     attrib_name.setPointer(reinterpret_cast<void*>(attrib_name.offset));
    #define SET_PROG(attrib_class, attrib_name, attrib_init)        attrib_name.setProg(prog, #attrib_name);
#else
    #define DECL_ATTRIB(attrib_class, attrib_name, attrib_init)     attrib_class attrib_name;
    #define ADD_UNIT_SIZE(attrib_class,  attrib_name, attrib_init)  unit += attrib_name.size();
    #define INIT_ATTRIB(attrib_class, attrib_name, attrib_init)     attrib_name = attrib_class( attrib_init );
    #define LINK_ATTRIB(attrib_class, attrib_name, attrib_init)     attrib_name.setOffset(offset); attrib_name.setBuffer(dataPtr + offset * size); offset += attrib_name.size();
    #define SET_POINTER(attrib_class, attrib_name, attrib_init)     attrib_name.setPointer((void*)(attrib_name.offset * size));
    #define SET_PROG(attrib_class, attrib_name, attrib_init)        attrib_name.setProg(prog, #attrib_name);
#endif

//@ peut etre definir deux macros, ou une option selon prog / pas prog..
#define VERTEX_DATA_SUBCLASS(className, seq)                    \
    class className : public VertexData {                       \
        public:                                                 \
        /*className(ProgGLSL prog = ProgGLSL()) {*/                 \
        className() {                                           \
            unit = 0;                                           \
            ATTRIB_SEQ_APPLY(ADD_UNIT_SIZE, seq)                \
            ATTRIB_SEQ_APPLY(INIT_ATTRIB, seq)                  \
            }                                                   \
        int totalSize() { return size * unit; }                 \
        virtual int unitSize() const { return unit; }           \
        virtual void setBuffers(char *dataPtr, int size) {      \
            this->size = size;                                  \
            int offset = 0;                                     \
            ATTRIB_SEQ_APPLY(LINK_ATTRIB, seq)                  \
            }                                                   \
        virtual void setPointers() {                            \
            ATTRIB_SEQ_APPLY(SET_POINTER, seq)                  \
            }                                                   \
        virtual void setProg(const ProgGLSL &prog) {            \
            ATTRIB_SEQ_APPLY(SET_PROG, seq)                     \
            }                                                   \
        public:                                                 \
            ATTRIB_SEQ_APPLY(DECL_ATTRIB, seq)                  \
            int unit;                                           \
            int size;                                           \
        };
    
//@@ messages d'erreur :
//@ set pointers sans setProg
//@ setProg sans attribs generiques (inutiles)
    
// (class, name, params)

#define DEF_ATTRIB(attrib, type, channels)          ((VA_CLASS_NAME(attrib,type,channels),       ATTRIB_NAME(attrib),    ))
#define DEF_ATTRIB_TEXCOORD(num, type, channels)    ((VA_CLASS_NAME(TexCoord,type,channels),     CONCAT2(texCoord,num),     num))
//#define DEF_ATTRIB_GENERIC(name, type, channels)    ((VA_CLASS_NAME(VertexAttrib,type,channels), name,                      prog.getAttribID(#name)))
#define DEF_ATTRIB_GENERIC(name, type, channels)    ((VA_CLASS_NAME(VertexAttrib,type,channels), name,                      ))
//#define DEF_ATTRIB_GENERIC(name, type, channels)    ((VA_CLASS_NAME(VertexAttrib,type,channels), name,                      #name))
//#define DEF_ATTRIB_GENERIC(name, type, channels, prog) ((VA_CLASS_NAME(VertexAttrib,type,channels), name, name))
//@@ a voir : nom d'attrib : gen_... ou gl_... pour les non gÃÂ©nÃÂ©riques?
//#define DEF_ATTRIB(attrib, type, channels) ((attrib,type,channels))

#endif

