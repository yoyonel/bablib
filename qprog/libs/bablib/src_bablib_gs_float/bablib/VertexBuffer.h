#ifndef __VERTEX_BUFFER__
#define __VERTEX_BUFFER__

#include <OpenGL.h>
#include "VertexBufferMacros.h"
#include "BufferGL.h"

class IndexBufferGL : public BufferGL {
    public:
        //@ verifs..
        IndexBufferGL(int n, GLenum usage = GL_STATIC_DRAW) : BufferGL(GL_ELEMENT_ARRAY_BUFFER, usage), n(n) {}
        virtual ~IndexBufferGL() {}

        int size() const { return n; }
        
        void load() { BufferGL::load(dataPtr(), n * dataSize()); }
        
        virtual void set(int i, int v) = 0;
        
        virtual GLenum dataType() const = 0;
        virtual int    dataSize() const = 0;    // in bytes
        virtual void*  dataPtr()  const = 0;
        
    protected:
        int n;
    };


//@ superclasse abstraite pour methodes communes
//@ classe (IndexBufferGL, VertexDataBufferGL)
template<class Index, GLenum enumGL>
class IndexBuffer : public IndexBufferGL {
    public:
        //@ verifs..
        //@ notamment GL_MAX_ELEMENTS_INDICES
       IndexBuffer(int n) : IndexBufferGL(n), index(new Index[n]) {}
        ~IndexBuffer() { delete[] index; destroy(); }

        //inline int byteSize() const { return n * sizeof(Index); }

        inline       Index& elem(int i)             { return index[i]; }
        inline const Index& elem(int i)       const { return index[i]; }
        inline       Index& operator()(int i)       { return index[i]; }
        inline const Index& operator()(int i) const { return index[i]; }
        
        inline virtual void set(int i, int v) { index[i] = static_cast<Index>(v); }
        inline virtual GLenum dataType() const { return enumGL; }
        inline virtual int    dataSize() const { return sizeof(Index); }
        inline virtual void*  dataPtr()  const { return index; }
        
        //static const GLenum DATA_TYPE = enumGL;
        
    private:
        Index *index;
    };

typedef IndexBuffer<GLubyte,  GL_UNSIGNED_BYTE>   IndexBufferUB;
typedef IndexBuffer<GLushort, GL_UNSIGNED_SHORT>  IndexBufferUS;
typedef IndexBuffer<GLuint,   GL_UNSIGNED_INT>    IndexBufferUI;
//IndexBufferUI test(4);

class VertexData {
    public:
        virtual int unitSize() const = 0;
        virtual void setBuffers(char *data, int size) = 0;
        virtual void setPointers() = 0;
		
		virtual void setProg(const ProgGLSL &prog) = 0;
    };
                
class VertexDataBufferGL : public BufferGL {
    public:
        //@ verifs..
        //@ notamment GL_MAX_ELEMENTS_VERTICES
        //@@ param <usage>
        VertexDataBufferGL(VertexData *attrib, int n, GLenum usage = GL_STATIC_DRAW) :
                BufferGL(GL_ARRAY_BUFFER, usage), n(n), attrib(attrib), data(new char[n * attrib->unitSize()]) {
            //@ verifs
            attrib->setBuffers(data, n);
            }
        ~VertexDataBufferGL() { delete[] data; destroy(); }

        void load() {
            BufferGL::load(data, n * attrib->unitSize());
            }
        
        void render(GLenum mode) {
            bind();
            attrib->setPointers();
            glDrawArrays(mode, 0, n);
            unbind();
            }

        void render(GLenum mode, IndexBufferGL *elements) {
            bind();
            attrib->setPointers();
            elements->bind();
            glDrawElements(mode, elements->size(), elements->dataType(), (void*)0);
            //glDrawRangeElements(mode, 0, size()-1, elements->size(), elements->dataType(), (void*)0);
            //glDrawRangeElements(mode, 0, size()/2-1, elements->size(), elements->dataType(), (void*)0);
            //glDrawRangeElements(mode, 100, 200, elements->size(), elements->dataType(), (void*)0);
            //glDrawRangeElements(mode, 0, 3000000, elements->size(), elements->dataType(), (void*)0);
            elements->unbind();
            //@@ desactiver les pointeurs d'attribs..
            unbind();
            
            //@@ mieux : faire fonctions begin/end par attrib
            glClientActiveTexture(GL_TEXTURE0); // modifié par classes VBO, pas sauvegardé par glPushAttrib (mais ok si glPushClientAttrib)
            }
        
        int size() const { return n; }
        
		//
		void setProg( const ProgGLSL& prog) { attrib->setProg( prog ); }

    private:
        int n;
        VertexData *attrib;
        char *data;
    };

#endif

