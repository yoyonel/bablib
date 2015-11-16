/****************************************************************************

Copyright (C) 2002-2007 Gilles Debunne (Gilles.Debunne@imag.fr)

This file is part of the QGLViewer library.
Version 2.2.6-3, released on August 28, 2007.

http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer

libQGLViewer is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

libQGLViewer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libQGLViewer; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/

#ifndef __TEST_SM__
#define __TEST_SM__

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>

#include <QGLViewer/qglviewer.h>
#include <QGLViewer/manipulatedFrame.h>
#include <QGLViewer/manipulatedCameraFrame.h>

#include <stdio.h>	// # fprintf
using namespace std;

#include <ProgGLSL.h>
#include <Texture.h>
#include <Image1D.h>
#include <Image2D.h>
#include <FrameBuffer.h>
#include <ProjTransfo.h>

#define __DEBUG__
#include "helper_preprocess.h"

#include "TimerQueryNV.h"
//
//#include <scene3d/TriSoup.h>
#include "TriSoup2.h"
//
#include <VertexBuffer.h>


typedef Image1D<UInt4> Image1DUInt4;
typedef Image2D<UInt4> Image2DUInt4;


/**
 * @brief The Viewer class
 */
class Viewer : public QGLViewer {
public:
    Viewer();
    ~Viewer();

protected :
    virtual void preDraw();
    virtual void draw();
    virtual void init();
    virtual void keyPressEvent(QKeyEvent *e);

private :

    // - INIT
    void initAll( bool bDestroy = false );
    //
    //
    void initTextures( bool bDestroy = false );
    void initFrameBuffers( bool bDestroy = false );
    void initShaders( bool bDestroy = false );
    //
    void initTimerQueryNV( bool bDestroy = false );
    //
    void initVBO( bool bDestroy = false );
    void initTriSoup( bool bDestroy = false );

    // - DESTROY
    void destroyAll();
    void destroyTextures();
    void destroyFrameBuffers();
    void destroyShaders();
    //
    void destroyTimerQueryNV();
    //
    void destroyVBO();
    void destroyTriSoup();

    // - UPDATE
    void update_matrix();
    //
    void updateFrameBuffers();
    void updateSolidVoxelisation(FrameBuffer& _framebuffer);
    //
    void resetFrameBuffer( FrameBuffer& _fbo, ProgGLSL& _prog_reset );
    void resetFrameBuffers();
    //
    void setOpenGLStates();

    bool checkFrameBufferStatus();

    // - DRAW
    void drawScene();
    //
    void drawTexture(Texture tex, int x, int y, int w, int h) const;
    void drawTextures();
    //
    void drawPlane();
    //
    void drawLightCamera( float _fCoef_Alpha = 0.5f );
    //

private :
    bool bFirstInit;

    // FRAMEBUFFERS/TEXTURES BabLib
    FrameBuffer fb_sv;
    Texture     tex_sv;
    Image2DUInt4 img_sv;
    Texture     tex_bitmask;

    // SHADERS BabLib
    ProgGLSL prog_draw_texture;
    ProgGLSL prog_vbo;
    ProgGLSL prog_vbo_sv;

    // CAMERAS QGLViewer

    // TIMERS [ EXT: disponible sur NVidia, pas (pas encore chez) ATI/AMD :'( ]
    // - FBDSM: FrameBuffer Depth Shadow Map
    //CTimerQueryNV   tq_update_shadow_map,
    //tq_update_shadow_clip_map;

    // VBO
    // - loader
    TriSoup2 *ts;

    // - VBO OpenGL
    VertexDataBufferGL 	*vbo;
    IndexBufferUI       *indexBuffer;

    // - VBO interface QGLViewer, manipulation de l'object
    qglviewer::ManipulatedFrame qgl_mf_vbo;
    float	f_scale_vbo;

    ProjTransfo pt_world_to_eyes;
    ProjTransfo pt_eyes_to_projective;
    ProjTransfo pt_projective_to_texture;

    ProjTransfo pt_world_to_light;
    ProjTransfo pt_light_to_projective;
    ProjTransfo pt_world_to_projective_light;
    ProjTransfo pt_world_to_texture_light;

    // - Object Matrix
    // -- VBO
    ProjTransfo pt_object_to_world_vbo;
    ProjTransfo pt_world_to_object_vbo;
    ProjTransfo pt_object_to_projective_light_vbo;
    ProjTransfo pt_object_to_texture_light_vbo;
    // -- PLANE
    ProjTransfo pt_object_to_world_plane;
    ProjTransfo pt_world_to_object_plane;

    qglviewer::Camera qgl_cam_light_mf;		//

    // Light Positions
    Vec4    v4_light_position_in_world;
    Vec4    v4_light_position_in_object_vbo;

    bool    b_is_init;

    GLint i_stencil_bits;
    GLint i_init_stencil;
};

#endif
