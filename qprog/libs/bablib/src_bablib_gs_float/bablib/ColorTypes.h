#ifndef __COLOR_TYPES__
#define __COLOR_TYPES__

//#include <OpenGL.h>
#include <GL/gl.h>
#include <QColor>

BABLIB_NAMESPACE_BEGIN

//////////////////////////////////////////////////////////////////////////////////////////
//#define AUTOMATIC_GL_VEC_TYPES
#ifdef AUTOMATIC_GL_VEC_TYPES
    #include <MacroTools.h>
    
    // struct TYPE_PARAM :
    #define GLTYPE_PARAM_byte     (GLbyte     , Byte  , b , GL_BYTE           )
    #define GLTYPE_PARAM_short    (GLshort    , Short , s , GL_SHORT          )
    #define GLTYPE_PARAM_int      (GLint      , Int   , i , GL_INT            )
    #define GLTYPE_PARAM_float    (GLfloat    , Float , f , GL_FLOAT          )
    #define GLTYPE_PARAM_double   (GLdouble   , Double, d , GL_DOUBLE         )
    #define GLTYPE_PARAM_ubyte    (GLubyte    , UByte , ub, GL_UNSIGNED_BYTE  )
    #define GLTYPE_PARAM_ushort   (GLushort   , UShort, us, GL_UNSIGNED_SHORT )
    #define GLTYPE_PARAM_uint     (GLuint     , UInt  , ui, GL_UNSIGNED_INT   )
    #define GLTYPE_PARAM_bool     (GLboolean  , Bool  ,   ,                   )
    
    /*
    // pareil avec les types c++ correspondant aux types GL :
    #define GLTYPE_PARAM_byte     (signed char    , Byte  , b , GL_BYTE           )
    #define GLTYPE_PARAM_short    (short          , Short , s , GL_SHORT          )
    #define GLTYPE_PARAM_int      (int            , Int   , i , GL_INT            )
    #define GLTYPE_PARAM_float    (float          , Float , f , GL_FLOAT          )
    #define GLTYPE_PARAM_double   (double         , Double, d , GL_DOUBLE         )
    #define GLTYPE_PARAM_ubyte    (unsigned char  , UByte , ub, GL_UNSIGNED_BYTE  )
    #define GLTYPE_PARAM_ushort   (unsigned short , UShort, us, GL_UNSIGNED_SHORT )
    #define GLTYPE_PARAM_uint     (unsigned int   , UInt  , ui, GL_UNSIGNED_INT   )
    #define GLTYPE_PARAM_bool     (unsigned char  , Bool  ,   ,                   )
    */
    
    #define GLTYPE_PARAM(type, i) BOOST_PP_TUPLE_ELEM(4, i, GLTYPE_PARAM_ ## type)
    #define GLTYPE_GL(type)       GLTYPE_PARAM(type, 0)
    #define GLTYPE_MAJ(type)      GLTYPE_PARAM(type, 1)
    #define GLTYPE_SUFFIX(type)   GLTYPE_PARAM(type, 2)
    #define GLTYPE_GLENUM(type)   GLTYPE_PARAM(type, 3)
    
    #define COMPOUNED_GLTYPE_NAME(type, channels)      CONCAT2(GLTYPE_MAJ(type), channels)
    #define COMPOUNED_GLTYPE_SUFFIX(type, channels)    CONCAT2(channels, GLTYPE_SUFFIX(type))
    
    #define AFFECT_ARRAY_V(i)     this->v[i] = v[i];
    
    #define MULTI_CHANNEL_LIST_XYZW(channels) MULTI_CHANNEL_LIST_XYZW_ ## channels
    #define MULTI_CHANNEL_LIST_XYZW_1 x
    #define MULTI_CHANNEL_LIST_XYZW_2 x,y
    #define MULTI_CHANNEL_LIST_XYZW_3 x,y,z
    #define MULTI_CHANNEL_LIST_XYZW_4 x,y,z,w
    
    #define MULTI_CHANNEL_LIST_RGBA(channels) MULTI_CHANNEL_LIST_RGBA_ ## channels
    #define MULTI_CHANNEL_LIST_RGBA_1 r
    #define MULTI_CHANNEL_LIST_RGBA_2 r,g
    #define MULTI_CHANNEL_LIST_RGBA_3 r,g,b
    #define MULTI_CHANNEL_LIST_RGBA_4 r,g,b,a
    
    #define MULTI_CHANNEL_GLENUM(channels) MULTI_CHANNEL_GLENUM_ ## channels
    #define MULTI_CHANNEL_GLENUM_1 GL_LUMINANCE
    #define MULTI_CHANNEL_GLENUM_2 GL_LUMINANCE_ALPHA
    #define MULTI_CHANNEL_GLENUM_3 GL_RGB
    #define MULTI_CHANNEL_GLENUM_4 GL_RGBA
    
    #define MULTI_CHANNEL_EXPAND_COMMA(channels, macro, params...) MULTI_CHANNEL_EXPAND_COMMA_ ## channels(macro, params)
    #define MULTI_CHANNEL_EXPAND_COMMA_1(macro, params...) macro(x, params)
    #define MULTI_CHANNEL_EXPAND_COMMA_2(macro, params...) macro(x, params), macro(y, params)
    #define MULTI_CHANNEL_EXPAND_COMMA_3(macro, params...) macro(x, params), macro(y, params), macro(z, params)
    #define MULTI_CHANNEL_EXPAND_COMMA_4(macro, params...) macro(x, params), macro(y, params), macro(z, params), macro(w, params)
    
    #define COMPOUNED_GLTYPE_MACRO_DECL(c, type) type c
    #define COMPOUNED_GLTYPE_MACRO_INIT(c, _) c(c)
    //#define COMPOUNED_GLTYPE_MACRO_APPLY_OP(c, op) a.c op b.c
    #define COMPOUNED_GLTYPE_MACRO_APPLY_OP(c, op_a, op, op_b) op_a.c op op_b.c
    
    #define COMPOUNED_GLTYPE_BINARY_OPERATOR_NAME(type, channels, op) \
        COMPOUNED_GLTYPE_NAME(type, channels) operator op (const COMPOUNED_GLTYPE_NAME(type, channels) &a, const COMPOUNED_GLTYPE_NAME(type, channels) &b)
    
    
    #define COMPOUNED_GLTYPE_CLASS(type, channels)                                      \
        class COMPOUNED_GLTYPE_NAME(type, channels) {                                   \
            public:                                                                     \
                COMPOUNED_GLTYPE_NAME(type, channels)(GLTYPE_GL(type) *v) {             \
                    FOR_INT(0, channels, AFFECT_ARRAY_V)                                \
                    }                                                                   \
                COMPOUNED_GLTYPE_NAME(type, channels)(MULTI_CHANNEL_EXPAND_COMMA(channels, COMPOUNED_GLTYPE_MACRO_DECL, GLTYPE_GL(type)))    \
                    : MULTI_CHANNEL_EXPAND_COMMA(channels, COMPOUNED_GLTYPE_MACRO_INIT) {           \
                    }                                                                   \
                                                                                        \
                /** opérateurs arithmétiques binaires (membres à membres) : **/         \
                friend COMPOUNED_GLTYPE_BINARY_OPERATOR_NAME(type, channels, +);        \
                friend COMPOUNED_GLTYPE_BINARY_OPERATOR_NAME(type, channels, -);        \
                friend COMPOUNED_GLTYPE_BINARY_OPERATOR_NAME(type, channels, *);        \
                friend COMPOUNED_GLTYPE_BINARY_OPERATOR_NAME(type, channels, /);        \
                                                                                        \
                void glColor() const { CONCAT3(glColor, COMPOUNED_GLTYPE_SUFFIX(type, channels), v)(v); }     \
            public:                                                                     \
                union {                                                                 \
                    struct { GLTYPE_GL(type) MULTI_CHANNEL_LIST_XYZW(channels); };      \
                    struct { GLTYPE_GL(type) MULTI_CHANNEL_LIST_RGBA(channels); };      \
                    GLTYPE_GL(type) v[channels];                                        \
                    };                                                                  \
                static const int SIZE = channels;                                       \
                static const GLint  TEX_FORMAT = MULTI_CHANNEL_GLENUM(channels);        \
                static const GLenum DATA_FORMAT = MULTI_CHANNEL_GLENUM(channels);       \
                static const GLenum DATA_TYPE = GLTYPE_GLENUM(type);                    \
            };
    
    #define COMPOUNED_GLTYPE_DEFINE_BINARY_OPERATOR(type, channels, op)                                                                     \
        COMPOUNED_GLTYPE_BINARY_OPERATOR_NAME(type, channels, op) {                                                                         \
            return COMPOUNED_GLTYPE_NAME(type, channels) (MULTI_CHANNEL_EXPAND_COMMA(channels, COMPOUNED_GLTYPE_MACRO_APPLY_OP, a, op, b)); \
            }
        
    //{return COMPOUNED_GLTYPE_NAME(type, channels) (MULTI_CHANNEL_EXPAND_COMMA(channels, COMPOUNED_GLTYPE_MACRO_APPLY_OP, +)); }
    #define COMPOUNED_GLTYPE_CLASS_DEFINITION(type, channels)       \
        COMPOUNED_GLTYPE_DEFINE_BINARY_OPERATOR(type, channels, +)  \
        COMPOUNED_GLTYPE_DEFINE_BINARY_OPERATOR(type, channels, -)  \
        COMPOUNED_GLTYPE_DEFINE_BINARY_OPERATOR(type, channels, *)  \
        COMPOUNED_GLTYPE_DEFINE_BINARY_OPERATOR(type, channels, /)
    
    
        
    //COMPOUNED_GLTYPE_CLASS(double, 1)
    //COMPOUNED_GLTYPE_CLASS(double, 2)
    COMPOUNED_GLTYPE_CLASS(double, 3)
    COMPOUNED_GLTYPE_CLASS(double, 4)
    
    COMPOUNED_GLTYPE_CLASS(int, 4)
    COMPOUNED_GLTYPE_CLASS(ushort, 4)
    
    COMPOUNED_GLTYPE_CLASS_DEFINITION(double, 4)
    COMPOUNED_GLTYPE_CLASS_DEFINITION(int, 4)
    COMPOUNED_GLTYPE_CLASS_DEFINITION(ushort, 4)

#endif
////////////////////////////////////////////////////////////////////////////


template<class Color>
class GenericColor {
    public:
        static Color create(float r, float g, float b, float a) { Color res; res.reset(r,g,b,a); return res; }
        static Color create(float c) { return create(c,c,c,c); }
        static Color BLACK() { return create(0); }
        static Color WHITE() { return create(1); }
        
        GenericColor(Color &color) : color(color) {}
        
        float norm2() const {
            float e = 0.0f;
            for (int c=0; c<Color::SIZE; c++) e += color[c] * color[c];
            return e;
            }

        float norm() const { return sqrt(norm2()); }
        float normRMS() const { return sqrt(norm2() / Color::SIZE); }
        
    private:
        Color &color;
    };

class Float1 {
    public:
        Float1(float x = 0);
        Float1(const float *c);   // tableau de 1 valeur
        Float1(QRgb c);
    
        // opérateurs de conversion :
        inline operator const float* () const {return c;}     // cast du Float1 en const float*
        inline operator       float* ()       {return c;}     // cast du Float1 en float*
        //inline operator const float  () const {return r;}     // cast du Float1 en const float
        inline operator       float  ()       {return r;}     // cast du Float1 en float
        QRgb toQRgb() const;
        QString toQString() const { return QString("(%1)").arg(r); }

        //opérateurs arithmétiques unaires :
        friend Float1 operator-(const Float1 &a);
    
        // opérateurs arithmétiques binaires (membres à membres) :
        friend Float1 operator+(const Float1 &a, const Float1 &b);
        friend Float1 operator-(const Float1 &a, const Float1 &b);
        friend Float1 operator*(const Float1 &a, const Float1 &b);
        friend Float1 operator/(const Float1 &a, const Float1 &b);
            
        // opérations avec un scalaire :
        friend Float1 operator*(float s, const Float1 &a);
        friend Float1 operator*(const Float1 &a, float s);
        friend Float1 operator/(float s, const Float1 &a);
        friend Float1 operator/(const Float1 &a, float s);
        
        // opérateurs de modification :
        Float1 operator+=(const Float1 &a);
        Float1 operator-=(const Float1 &a);
        Float1 operator*=(const Float1 &a);
        Float1 operator/=(const Float1 &a);
        Float1 operator*=(float s);
        Float1 operator/=(float s);

        void reset(float r=0, float g=0, float b=0, float a=0);
        void setGL() const;
        
    public:
        union {
            struct { float r; };
            struct { float x; };
            float c[1];
            };
        static const int SIZE = 1;
        static const GLint  TEX_FORMAT = 1;  // = GL_LUMINANCE
        static const GLenum DATA_FORMAT = GL_LUMINANCE;
        static const GLenum DATA_TYPE = GL_FLOAT;
    };

class Float2 {
    public:
        Float2(float x = 0, float y = 0);
        Float2(const float *c);   // tableau de 2 valeurs
        Float2(QRgb c);
    
        // opérateurs de conversion :
        inline operator const float* () const {return c;}     // cast du Float2 en const float*
        inline operator       float* ()       {return c;}     // cast du Float2 en float*
        QRgb toQRgb() const;
        QString toQString() const { return QString("(%1, %2)").arg(r).arg(g); }

        //opérateurs arithmétiques unaires :
        friend Float2 operator-(const Float2 &a);
    
        // opérateurs arithmétiques binaires (membres à membres) :
        friend Float2 operator+(const Float2 &a, const Float2 &b);
        friend Float2 operator-(const Float2 &a, const Float2 &b);
        friend Float2 operator*(const Float2 &a, const Float2 &b);
        friend Float2 operator/(const Float2 &a, const Float2 &b);
            
        // opérations avec un scalaire :
        friend Float2 operator*(float s, const Float2 &a);
        friend Float2 operator*(const Float2 &a, float s);
        friend Float2 operator/(float s, const Float2 &a);
        friend Float2 operator/(const Float2 &a, float s);
        
        // opérateurs de modification :
        Float2 operator+=(const Float2 &a);
        Float2 operator-=(const Float2 &a);
        Float2 operator*=(const Float2 &a);
        Float2 operator/=(const Float2 &a);
        Float2 operator*=(float s);
        Float2 operator/=(float s);

        void reset(float r=0, float g=0, float b=0, float a=0);
        void setGL() const;
        
    public:
        union {
            struct { float r, g; };
            struct { float x, y; };
            float c[2];
            };
        static const int SIZE = 2;
        static const GLint  TEX_FORMAT = 2;  // = GL_LUMINANCE_ALPHA
        static const GLenum DATA_FORMAT = GL_LUMINANCE_ALPHA;
        static const GLenum DATA_TYPE = GL_FLOAT;
    };

class Float3 {
    public:
        Float3(float x = 0, float y = 0, float z = 0);
        Float3(const float *c);   // tableau de 3 valeurs
        Float3(QRgb c);
    
        // opérateurs de conversion :
        inline operator const float* () const {return c;}     // cast du Float3 en const float*
        inline operator       float* ()       {return c;}     // cast du Float3 en float*
        QRgb toQRgb() const;
        QString toQString() const { return QString("(%1, %2, %3)").arg(r).arg(g).arg(b); }

        //opérateurs arithmétiques unaires :
        friend Float3 operator-(const Float3 &a);
    
        // opérateurs arithmétiques binaires (membres à membres) :
        friend Float3 operator+(const Float3 &a, const Float3 &b);
        friend Float3 operator-(const Float3 &a, const Float3 &b);
        friend Float3 operator*(const Float3 &a, const Float3 &b);
        friend Float3 operator/(const Float3 &a, const Float3 &b);
            
        // opérations avec un scalaire :
        friend Float3 operator*(float s, const Float3 &a);
        friend Float3 operator*(const Float3 &a, float s);
        friend Float3 operator/(float s, const Float3 &a);
        friend Float3 operator/(const Float3 &a, float s);
        
        // opérateurs de modification :
        Float3 operator+=(const Float3 &a);
        Float3 operator-=(const Float3 &a);
        Float3 operator*=(const Float3 &a);
        Float3 operator/=(const Float3 &a);
        Float3 operator*=(float s);
        Float3 operator/=(float s);

        void reset(float r=0, float g=0, float b=0, float a=0);
        void setGL() const;
        
    public:
        union {
            struct { float r, g, b; };
            struct { float x, y, z; };
            float c[3];
            };
        static const int SIZE = 3;
        static const GLint  TEX_FORMAT = 3;  // = GL_RGB
        static const GLenum DATA_FORMAT = GL_RGB;
        static const GLenum DATA_TYPE = GL_FLOAT;
    };

class Float4 {
    public:
        Float4(float x = 0, float y = 0, float z = 0, float w = 1);
        Float4(const float *c);   // tableau de 4 valeurs
        Float4(const float *xy, const float *zw);   // 2 tableaux de 2 valeurs
        Float4(QRgb c);
        
        // opérateurs de conversion :
        inline operator const float* () const {return c;}     // cast du Float4 en const float*
        inline operator       float* ()       {return c;}     // cast du Float4 en float*
        QRgb toQRgb() const;
        QString toQString() const { return QString("(%1, %2, %3, %4)").arg(r).arg(g).arg(b).arg(a); }

        //opérateurs arithmétiques unaires :
        friend Float4 operator-(const Float4 &a);
    
        // opérateurs arithmétiques binaires (membres à membres) :
        friend Float4 operator+(const Float4 &a, const Float4 &b);
        friend Float4 operator-(const Float4 &a, const Float4 &b);
        friend Float4 operator*(const Float4 &a, const Float4 &b);
        friend Float4 operator/(const Float4 &a, const Float4 &b);
            
        // opérations avec un scalaire :
        friend Float4 operator*(float s, const Float4 &a);
        friend Float4 operator*(const Float4 &a, float s);
        friend Float4 operator/(float s, const Float4 &a);
        friend Float4 operator/(const Float4 &a, float s);
        
        // opérateurs de modification :
        Float4 operator+=(const Float4 &a);
        Float4 operator-=(const Float4 &a);
        Float4 operator*=(const Float4 &a);
        Float4 operator/=(const Float4 &a);
        Float4 operator*=(float s);
        Float4 operator/=(float s);

        void reset(float r=0, float g=0, float b=0, float a=0);
        void setAlpha(float a);
        void setGL() const;
        
    public:
        union {
            struct { float r, g, b, a; };
            struct { float x, y, z, w; };
            float c[4];
            };
        static const int SIZE = 4;
        static const GLint  TEX_FORMAT = 4;  // = GL_RGBA
        static const GLenum DATA_FORMAT = GL_RGBA;
        static const GLenum DATA_TYPE = GL_FLOAT;
    };

class UByte3;

class UInt1 {
    public:
        UInt1() : x(0) {}
        UInt1(GLuint x);
        UInt1(const GLuint *c);   // tableau de 1 valeur
        /*UInt1(QRgb c);*/
    
        // opérateurs de conversion :
        inline operator const GLuint* () const {return c;}     // cast du UInt1 en const GLuint*
        inline operator       GLuint* ()       {return c;}     // cast du UInt1 en GLuint*
        QRgb toQRgb() const;
        QString toQString() const { return QString("(%1)").arg(r); }

        // opérateurs arithmétiques binaires (membres à membres) :
        friend UInt1 operator+(const UInt1 &a, const UInt1 &b);
        friend UInt1 operator-(const UInt1 &a, const UInt1 &b);
        friend UInt1 operator*(const UInt1 &a, const UInt1 &b);
        friend UInt1 operator/(const UInt1 &a, const UInt1 &b);
            
        // opérations avec un scalaire :
        friend UInt1 operator*(float s, const UInt1 &a);
        friend UInt1 operator*(const UInt1 &a, float s);
        friend UInt1 operator/(float s, const UInt1 &a);
        friend UInt1 operator/(const UInt1 &a, float s);
        
        // opérateurs de modification :
        UInt1 operator+=(const UInt1 &a);
        UInt1 operator-=(const UInt1 &a);
        UInt1 operator*=(const UInt1 &a);
        UInt1 operator/=(const UInt1 &a);
        UInt1 operator*=(float s);
        UInt1 operator/=(float s);

        void reset(float r=0, float g=0, float b=0, float a=0);
        void setGL() const;
        
        //UByte1 toUByte1() const;
        
    public:
        union {
            struct { GLuint r; };
            struct { GLuint x; };
            GLuint c[1];
            };
        static const int SIZE = 1;
        static const GLint  TEX_FORMAT = 1;  // = GL_LUMINANCE
        //static const GLenum DATA_FORMAT = GL_LUMINANCE;
        static const GLenum DATA_FORMAT = GL_LUMINANCE_INTEGER_EXT;     // pour utiliser EXT_texture_integer (usampler dans shaders GLSL)
        //@@@ définir DATA_FORMAT et DATA_FORMAT_INTEGER, puis dans Image*.loadToGPU, tester si l'internal format est un format entier
        static const GLenum DATA_TYPE = GL_UNSIGNED_INT;
    };

class UInt2 {
    public:
        UInt2() : x(0), y(0) {}
        UInt2(GLuint x, GLuint y);
        UInt2(const GLuint *c);   // tableau de 2 valeurs
        UInt2(QRgb c);
    
        // opérateurs de conversion :
        inline operator const GLuint* () const {return c;}     // cast du UInt2 en const GLuint*
        inline operator       GLuint* ()       {return c;}     // cast du UInt2 en GLuint*
        QRgb toQRgb() const;
        QString toQString() const { return QString("(%1, %2)").arg(r).arg(g); }

        // opérateurs arithmétiques binaires (membres à membres) :
        friend UInt2 operator+(const UInt2 &a, const UInt2 &b);
        friend UInt2 operator-(const UInt2 &a, const UInt2 &b);
        friend UInt2 operator*(const UInt2 &a, const UInt2 &b);
        friend UInt2 operator/(const UInt2 &a, const UInt2 &b);
            
        // opérations avec un scalaire :
        friend UInt2 operator*(float s, const UInt2 &a);
        friend UInt2 operator*(const UInt2 &a, float s);
        friend UInt2 operator/(float s, const UInt2 &a);
        friend UInt2 operator/(const UInt2 &a, float s);
        
        // opérateurs de modification :
        UInt2 operator+=(const UInt2 &a);
        UInt2 operator-=(const UInt2 &a);
        UInt2 operator*=(const UInt2 &a);
        UInt2 operator/=(const UInt2 &a);
        UInt2 operator*=(float s);
        UInt2 operator/=(float s);

        void reset(float r=0, float g=0, float b=0, float a=0);
        void setGL() const;
        
    public:
        union {
            struct { GLuint r, g; };
            struct { GLuint x, y; };
            GLuint c[2];
            };
        static const int SIZE = 2;
        static const GLint  TEX_FORMAT = 2;  // = GL_LUMINANCE_ALPHA
        //static const GLenum DATA_FORMAT = GL_LUMINANCE_ALPHA;
        static const GLenum DATA_FORMAT = GL_LUMINANCE_ALPHA_INTEGER_EXT;       //@@ a voir
        static const GLenum DATA_TYPE = GL_UNSIGNED_INT;
    };

class UInt3 {
    public:
        UInt3() : x(0), y(0), z(0) {}
        UInt3(GLuint x, GLuint y, GLuint z);
        UInt3(const GLuint *c);   // tableau de 3 valeurs
        UInt3(QRgb c);
    
        // opérateurs de conversion :
        inline operator const GLuint* () const {return c;}     // cast du UInt3 en const GLuint*
        inline operator       GLuint* ()       {return c;}     // cast du UInt3 en GLuint*
        QRgb toQRgb() const;
        QString toQString() const { return QString("(%1, %2, %3)").arg(r).arg(g).arg(b); }

        // opérateurs arithmétiques binaires (membres à membres) :
        friend UInt3 operator+(const UInt3 &a, const UInt3 &b);
        friend UInt3 operator-(const UInt3 &a, const UInt3 &b);
        friend UInt3 operator*(const UInt3 &a, const UInt3 &b);
        friend UInt3 operator/(const UInt3 &a, const UInt3 &b);
            
        // opérations avec un scalaire :
        friend UInt3 operator*(float s, const UInt3 &a);
        friend UInt3 operator*(const UInt3 &a, float s);
        friend UInt3 operator/(float s, const UInt3 &a);
        friend UInt3 operator/(const UInt3 &a, float s);
        
        // opérateurs de modification :
        UInt3 operator+=(const UInt3 &a);
        UInt3 operator-=(const UInt3 &a);
        UInt3 operator*=(const UInt3 &a);
        UInt3 operator/=(const UInt3 &a);
        UInt3 operator*=(float s);
        UInt3 operator/=(float s);

        void reset(float r=0, float g=0, float b=0, float a=0);
        void setGL() const;
        
        UByte3 toUByte3() const;
        
    public:
        union {
            struct { GLuint r, g, b; };
            struct { GLuint x, y, z; };
            GLuint c[3];
            };
        static const int SIZE = 3;
        static const GLint  TEX_FORMAT = 3;  // = GL_RGB
        //static const GLenum DATA_FORMAT = GL_RGB;
        static const GLenum DATA_FORMAT = GL_RGB_INTEGER_EXT;       //@@ a voir
        static const GLenum DATA_TYPE = GL_UNSIGNED_INT;
    };

class UByte4;

class UInt4 {
    public:
        UInt4() : x(0), y(0), z(0), w(255) {}
        UInt4(GLuint x, GLuint y, GLuint z, GLuint w);
        UInt4(const GLuint *c);   // tableau de 4 valeurs
        UInt4(QRgb c);
    
        // opérateurs de conversion :
        inline operator const GLuint* () const {return c;}     // cast du UInt4 en const GLuint*
        inline operator       GLuint* ()       {return c;}     // cast du UInt4 en GLuint*
        QRgb toQRgb() const;
        QString toQString() const { return QString("(%1, %2, %3, %4)").arg(r).arg(g).arg(b).arg(a); }

        // opérateurs arithmétiques binaires (membres à membres) :
        friend UInt4 operator+(const UInt4 &a, const UInt4 &b);
        friend UInt4 operator-(const UInt4 &a, const UInt4 &b);
        friend UInt4 operator*(const UInt4 &a, const UInt4 &b);
        friend UInt4 operator/(const UInt4 &a, const UInt4 &b);
            
        // opérations avec un scalaire :
        friend UInt4 operator*(float s, const UInt4 &a);
        friend UInt4 operator*(const UInt4 &a, float s);
        friend UInt4 operator/(float s, const UInt4 &a);
        friend UInt4 operator/(const UInt4 &a, float s);
        
        // opérateurs de modification :
        UInt4 operator+=(const UInt4 &a);
        UInt4 operator-=(const UInt4 &a);
        UInt4 operator*=(const UInt4 &a);
        UInt4 operator/=(const UInt4 &a);
        UInt4 operator*=(float s);
        UInt4 operator/=(float s);

        void reset(float r=0, float g=0, float b=0, float a=0);
        void setAlpha(float a);
        void setGL() const;
        
        UByte4 toUByte4() const;
        
    public:
        union {
            struct { GLuint r, g, b, a; };
            struct { GLuint x, y, z, w; };
            GLuint c[4];
            };
        static const int SIZE = 4;
        static const GLint  TEX_FORMAT = 4;  // = GL_RGBA
        //static const GLenum DATA_FORMAT = GL_RGBA;
        static const GLenum DATA_FORMAT = GL_RGBA_INTEGER_EXT;      //@@ a voir
        static const GLenum DATA_TYPE = GL_UNSIGNED_INT;
    };

class UByte3 {
    public:
        UByte3(GLubyte x = 0, GLubyte y = 0, GLubyte z = 0);
        UByte3(const GLubyte *c);   // tableau de 3 valeurs
        UByte3(QRgb c);
    
        // opérateurs de conversion :
        inline operator const GLubyte* () const {return c;}     // cast du UByte3 en const GLubyte*
        inline operator       GLubyte* ()       {return c;}     // cast du UByte3 en GLubyte*
        QRgb toQRgb() const;
        QString toQString() const { return QString("(%1, %2, %3)").arg(r).arg(g).arg(b); }

        // opérateurs arithmétiques binaires (membres à membres) :
        friend UByte3 operator+(const UByte3 &a, const UByte3 &b);
        friend UByte3 operator-(const UByte3 &a, const UByte3 &b);
        friend UByte3 operator*(const UByte3 &a, const UByte3 &b);
        friend UByte3 operator/(const UByte3 &a, const UByte3 &b);
            
        // opérations avec un scalaire :
        friend UByte3 operator*(float s, const UByte3 &a);
        friend UByte3 operator*(const UByte3 &a, float s);
        friend UByte3 operator/(float s, const UByte3 &a);
        friend UByte3 operator/(const UByte3 &a, float s);
        
        // opérateurs de modification :
        UByte3 operator+=(const UByte3 &a);
        UByte3 operator-=(const UByte3 &a);
        UByte3 operator*=(const UByte3 &a);
        UByte3 operator/=(const UByte3 &a);
        UByte3 operator*=(float s);
        UByte3 operator/=(float s);

        void reset(float r=0, float g=0, float b=0, float a=0);
        void setGL() const;
        
        UInt3 toUInt3() const;
        
    public:
        union {
            struct { GLubyte r, g, b; };
            struct { GLubyte x, y, z; };
            GLubyte c[3];
            };
        static const int SIZE = 3;
        static const GLint  TEX_FORMAT = 3;  // = GL_RGB
        static const GLenum DATA_FORMAT = GL_RGB;
        static const GLenum DATA_TYPE = GL_UNSIGNED_BYTE;
    };

class UByte4 {
    public:
        UByte4(GLubyte x = 0, GLubyte y = 0, GLubyte z = 0, GLubyte w = 255);
        UByte4(const GLubyte *c);   // tableau de 4 valeurs
        UByte4(QRgb c);
    
        // opérateurs de conversion :
        inline operator const GLubyte* () const {return c;}     // cast du UByte4 en const GLubyte*
        inline operator       GLubyte* ()       {return c;}     // cast du UByte4 en GLubyte*
        QRgb toQRgb() const;
        QString toQString() const { return QString("(%1, %2, %3, %4)").arg(r).arg(g).arg(b).arg(a); }

        // opérateurs arithmétiques binaires (membres à membres) :
        friend UByte4 operator+(const UByte4 &a, const UByte4 &b);
        friend UByte4 operator-(const UByte4 &a, const UByte4 &b);
        friend UByte4 operator*(const UByte4 &a, const UByte4 &b);
        friend UByte4 operator/(const UByte4 &a, const UByte4 &b);
            
        // opérations avec un scalaire :
        friend UByte4 operator*(float s, const UByte4 &a);
        friend UByte4 operator*(const UByte4 &a, float s);
        friend UByte4 operator/(float s, const UByte4 &a);
        friend UByte4 operator/(const UByte4 &a, float s);
        
        // opérateurs de modification :
        UByte4 operator+=(const UByte4 &a);
        UByte4 operator-=(const UByte4 &a);
        UByte4 operator*=(const UByte4 &a);
        UByte4 operator/=(const UByte4 &a);
        UByte4 operator*=(float s);
        UByte4 operator/=(float s);

        void reset(float r=0, float g=0, float b=0, float a=0);
        void setAlpha(float a);
        void setGL() const;
        
        UInt4 toUInt4() const;
        
    public:
        union {
            struct { GLubyte r, g, b, a; };
            struct { GLubyte x, y, z, w; };
            GLubyte c[4];
            };
        static const int SIZE = 4;
        static const GLint  TEX_FORMAT = 4;  // = GL_RGBA
        static const GLenum DATA_FORMAT = GL_RGBA;
        static const GLenum DATA_TYPE = GL_UNSIGNED_BYTE;
    };

BABLIB_NAMESPACE_END

#endif

