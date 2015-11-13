#include "TriSoupRenderer.h"
#include "Tri.h"
#include <Params.h>
#include <Message.h>
#ifdef USE_VBO
    #include <VertexBuffer.h>
#endif

// most efficient rendering :
// --> interleaved VBO + element array + optimised shader (per-vertex lighting computations + only color as varying = gouraud shading)
// --> probably faster with separated vbos for each attrib
// --> gouraud shader not necessary : what was (very) costly was glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1)


TriSoupRenderer::TriSoupRenderer(const TriSoup *ts) : ts(ts) {
    #ifdef USE_VBO
        VERTEX_DATA_SUBCLASS(VertexDataFormat,
            DEF_ATTRIB(Vertex, float, 3)
            DEF_ATTRIB(Normal, float, 3)
            )
            
        VertexDataFormat *vd = new VertexDataFormat();
        #ifdef NO_ELEMENT_ARRAY
            vertexBuffer = new VertexDataBufferGL(vd, 3 * ts->nt);
            for (int i=0; i<ts->nt; i++) {
                for (int j=0; j<3; j++) {
                    const int k = ts->tri[i].index[j];
                    vd->vertex.set(3*i + j, ts->vertex[k]);
                    vd->normal.set(3*i + j, ts->normal[k]);
                    }
                }
            vertexBuffer->load();
        #else
            vertexBuffer = new VertexDataBufferGL(vd, ts->nv);
            for (int i=0; i<ts->nv; i++) {
                vd->vertex.set(i, ts->vertex[i]);
                vd->normal.set(i, ts->normal[i]);
                }
            vertexBuffer->load();
            
            //indexBuffer = new IndexBufferUI(3 * ts->nt);
            const int ni = 3 * ts->nt;
            if (ni <= 0x100)   indexBuffer = new IndexBufferUB(ni); else
            if (ni <= 0x10000) indexBuffer = new IndexBufferUS(ni); else
                               indexBuffer = new IndexBufferUI(ni);
            
            for (int i=0; i<ts->nt; i++) {
                indexBuffer->set(3*i + 0, ts->tri[i].index[0]);
                indexBuffer->set(3*i + 1, ts->tri[i].index[1]);
                indexBuffer->set(3*i + 2, ts->tri[i].index[2]);
                }
            indexBuffer->load();
        #endif
    #else
        displist = new DispListGL();
        displist->begin();
            glBegin(GL_TRIANGLES);
            for (int i=0; i<ts->nt; i++) {
                //glColor3fv(ts->tri[i].color);
                for (int j=0; j<3; j++) {
                    const int k = ts->tri[i].index[j];
                    glNormal3fv(ts->normal[k]);
                    glVertex3fv(ts->vertex[k]);
                    }
                }
            glEnd();
        displist->end();
    #endif
    }

TriSoupRenderer::~TriSoupRenderer() {
    #ifdef USE_VBO
        delete vertexBuffer;
        delete indexBuffer;
    #else
        delete displist;
    #endif
    }
        
void TriSoupRenderer::renderRaw() const {
    //if (cullBack) {
    //    glEnable(GL_CULL_FACE);
    //    glCullFace(GL_BACK);
    //    }
    
    #ifdef USE_VBO
        #ifdef NO_ELEMENT_ARRAY
            vertexBuffer->render(GL_TRIANGLES);
        #else
            vertexBuffer->render(GL_TRIANGLES, indexBuffer);
        #endif
    #else
        displist->exec();
    #endif
    }

/*********************************************************************************************/
// <ShaderTSR> implementation:

ShaderTSR::ShaderTSR(const TriSoup *ts, QString shaderName) : TriSoupRenderer(ts) {
    shader.loadDir(shaderName);
    }

void ShaderTSR::begin() const { shader.activate(); }
void ShaderTSR::end()   const { shader.deactivate(); }

/*********************************************************************************************/
// <PrimitiveColorTSR> implementation:

void PrimitiveColorTSR::renderRaw() const {
    glDisable(GL_COLOR_MATERIAL);
    //glDisable(GL_LIGHTING);
    glBegin(GL_TRIANGLES);
    for (int i=0; i<ts->nt; i++) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Vec4(debug ? ts->tri[i].debugColor : ts->tri[i].color, 1.0f));
        //glColor3fv(debug ? ts->tri[i].debugColor : ts->tri[i].color);
        for (int j=0; j<3; j++) {
            const int k = ts->tri[i].index[j];
            glNormal3fv(ts->normal[k]);
            glVertex3fv(ts->vertex[k]);
            }
        }
    glEnd();
    }

