#include "viewer_sv.h"

#include <Params.h>
#include <Message.h>
#include <ColorTypes.h>
#include <Message.h>
// trisoup/
#include <scene3d/Tri.h>
//#include <TriSoup.h>
//#include "TriSoup_AOV.h"
#include <VertexBuffer.h>
#include <FrameBuffer.h>
//
#include <ColorTypes.h>
#include <Image1D.h>
#include <Texture.h>
//
#include <bitset>   // url: http://stackoverflow.com/questions/7349689/c-how-to-print-using-cout-the-way-a-number-is-stored-in-memory


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

    solid_voxelisation = new SolidVoxelisation(
                PARAM(int, texture.sv.width),
                PARAM(int, texture.sv.height),
                128
                );
    solid_voxelisation->setCamera(qgl_cam_light_mf);
    setManipulatedFrame( solid_voxelisation->camera().frame() );

    b_is_init = true;
}

void Viewer::initTextures( bool bDestroy )
{
    if ( bDestroy ) {
        destroyTextures();
    }

    MSG_CHECK_GL;
}

void Viewer::initFrameBuffers( bool bDestroy )
{
    if ( bDestroy ) {
        destroyFrameBuffers();
    }

    MSG_CHECK_GL;
}

void Viewer::initShaders( bool bDestroy )
{
    if ( bDestroy ) {
        destroyShaders();
    }

    LOADDIRSHADER( PARAM(QString, shaderDir.vbo), prog_vbo );
    LOADDIRSHADER( PARAM(QString, shaderDir.vbo_wire), prog_vbo_wire );

    LOADDIRSHADER( PARAM(QString, shaderDir.draw_texture), prog_draw_texture );

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

    ts = TriSoup2::load(PARAM(QString, model.default));

    ts->fitToUnitSphere();

    MSG_CHECK_GL;
}

void Viewer::initVBO( bool bDestroy )
{
    if ( bDestroy ) {
        destroyVBO();
    }

    {
        // definition VBO avec classes génériques (bablib v2)
        // pour une définition "intÃ¨gre" OpenGL, il faut les customs attribs (génériques attribs) soient présents dans les shaders
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

