#ifndef __TRI_SOUP_RENDERER__
#define __TRI_SOUP_RENDERER__

#include "TriSoup.h"
#include <ProgGLSL.h>
#include <DispListGL.h>

#define USE_VBO
//#define NO_ELEMENT_ARRAY

#ifdef USE_VBO
    class VertexDataBufferGL;
    class IndexBufferGL;
#endif

class TriSoupRenderer {
    public:
        TriSoupRenderer(const TriSoup *ts);
        virtual ~TriSoupRenderer();
        
        void render() const { begin(); renderRaw(); end(); }
        
        // if multiple renderings must be done sequentially :
        virtual void begin() const {}
        virtual void renderRaw() const;
        virtual void end() const {}
        
    protected:
        #ifdef USE_VBO
            VertexDataBufferGL *vertexBuffer;
            IndexBufferGL *indexBuffer;
        #else
            DispListGL *displist;
        #endif
        const TriSoup *ts;
    };

class ShaderTSR : public TriSoupRenderer {
    public:
        ShaderTSR(const TriSoup *ts, ProgGLSL shader) : TriSoupRenderer(ts), shader(shader) {}
        ShaderTSR(const TriSoup *ts, QString shaderName);
        virtual ~ShaderTSR() { shader.destroy(); }
        
        virtual void begin() const;
        virtual void end() const;
        
        //@ add possibility to setup uniform params of the shader
        
    private:
        mutable ProgGLSL shader;
    };

class GouraudTSR : public ShaderTSR {
    public:
        GouraudTSR(const TriSoup *ts) : ShaderTSR(ts, "[bablib]/gouraud") {}
    };

// to output the primitive id as an unsigned int
class PrimitiveIndexTSR : public ShaderTSR {
    public:
        PrimitiveIndexTSR(const TriSoup *ts) : ShaderTSR(ts, "[bablib]/primitive_id") {}
    };

class PrimitiveColorTSR : public TriSoupRenderer {
    public:
        PrimitiveColorTSR(const TriSoup *ts, bool debug=false) : TriSoupRenderer(ts), debug(debug) {}
        virtual void renderRaw() const;
        void setDebug(bool debug) { this->debug = debug; }
    private:
        bool debug;
    };

#endif


