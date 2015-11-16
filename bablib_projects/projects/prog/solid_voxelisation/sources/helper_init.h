#ifndef __HELPER_VIEWER_INIT_H__
#define __HELPER_VIEWER_INIT_H__

#include <ColorTypes.h>
#include <Message.h>

// trisoup/
#include <scene3d/Tri.h>
//#include <TriSoup.h>
//#include "TriSoup_AOV.h"
#include <VertexBuffer.h>
#include <FrameBuffer.h>

void Viewer::initAll( bool bDestroy )
{
    // - TEXTURES
    initTextures( bDestroy );
    // - FRAMEBUFFERS
    initFrameBuffers( bDestroy );
    // - SHADERS
    initShaders( bDestroy );
    // -
    initTimerQueryNV( bDestroy );
    // -
    initTriSoup( bDestroy );
    // -
    initVBO( bDestroy );

    f_scale_vbo = 1.f;

    b_is_init = true;
}

void Viewer::initTextures( bool bDestroy )
{
    if ( bDestroy ) {
        destroyTextures();
    }

    //
    tex_sv = Texture::createTex2DInteger(
                PARAM(int, texture.sv.width),
                PARAM(int, texture.sv.height),
                PARAM(GLenum, texture.sv.internalformat),
                PARAM(GLenum, texture.sv.filter),
                PARAM(GLenum, texture.sv.wrapmode)
                );

    MSG_CHECK_GL;
}

void Viewer::initFrameBuffers( bool bDestroy )
{
    if ( bDestroy ) {
        destroyFrameBuffers();
    }

    fb_sv = FrameBuffer::create_Tex2D_Z(&tex_sv, PARAM(GLenum, texture.sv.depth_format));
    //
    fb_sv.checkCompleteness(true);

    MSG_CHECK_GL;
}

void Viewer::initShaders( bool bDestroy )
{
    if ( bDestroy ) {
        destroyShaders();
    }

    LOADDIRSHADER( PARAM(QString, shaderDir.vbo), prog_vbo );
    LOADDIRSHADER( PARAM(QString, shaderDir.vbo_sv), prog_vbo_sv );

    MSG_CHECK_GL;
}

void Viewer::initTimerQueryNV( bool bDestroy ) {
    if ( bDestroy ) {
        destroyTimerQueryNV();
    }

    MSG_CHECK_GL;
}

void Viewer::initTriSoup( bool bDestroy )
{
    if ( bDestroy ) {
        destroyTriSoup();
    }

    ts = TriSoup::load(PARAM(QString, model.default));

    //ts->fitToUnitSphere();

    MSG_CHECK_GL;
}

void Viewer::initVBO( bool bDestroy )
{
    if ( bDestroy ) {
        destroyVBO();
    }

    {
        // definition VBO avec classes génériques (bablib v2)
        // pour une définition "intègre" OpenGL, il faut les customs attribs (génériques attribs) soient présents dans les shaders
        // sinon il n'est pas possible d'attribuer un id pour les attributs (prog.getAttribID(#name) renvoie -1)
        VERTEX_DATA_SUBCLASS(VDataDef,
                             DEF_ATTRIB(Vertex, float, 3)
                             DEF_ATTRIB(Normal, float, 3)
                             DEF_ATTRIB(Color,  float, 4)
                             );

        VDataDef *vd = new VDataDef();

        indexBuffer = new IndexBufferUI(3 * ts->nt);

        vbo = new VertexDataBufferGL(vd, ts->nv);

        for (int i=0; i<ts->nv; i++) {
            vd->vertex.set(i, ts->vertex[i]);
            vd->normal.set(i, ts->normal[i]);
            Vec4 c = Vec4::random(0,1);
            vd->color.set(i, c);
        }

        // remplissage et load de l'index buffer :
        for (int i=0; i < ts->nt; i++) {
            const int	indice_vertex0 = ts->tri[i].index[0],
                    indice_vertex1 = ts->tri[i].index[1],
                    indice_vertex2 = ts->tri[i].index[2];
            //
            indexBuffer->set(3*i + 0, indice_vertex0);
            indexBuffer->set(3*i + 1, indice_vertex1);
            indexBuffer->set(3*i + 2, indice_vertex2);
        }

        vbo->load();
        indexBuffer->load();
    }

    MSG_CHECK_GL;
}

#endif
