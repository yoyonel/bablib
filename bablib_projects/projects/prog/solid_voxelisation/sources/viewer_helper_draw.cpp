#include "viewer_sv.h"

#include <Message.h>
#include <TransfoGL.h>
#include <Params.h>

//
void Viewer::drawScene() {
    //
    if (b_is_init) {
        // -- récupération des dimensions des textures
        ViewportGL viewport;
        viewport.getGL();

        const Vec2 v2_screen_size( viewport.w, viewport.h );

        glLightfv( GL_LIGHT0, GL_POSITION, v4_light_position_in_world );
        const float f_coef_ambient_light = 1.f - PARAM(float, shadow_volume.coef_alpha);
        Vec4 v4_color_ambient_light = Vec4( f_coef_ambient_light );
        glLightfv( GL_LIGHT0, GL_AMBIENT, v4_color_ambient_light );
        Vec4 v4_color_diffuse_light = Vec4( 1.f );
        glLightfv( GL_LIGHT0, GL_DIFFUSE, v4_color_diffuse_light );

        glMatrixMode(GL_MODELVIEW);

        if (PARAM(bool, plane.render)) {
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            pt_object_to_world_plane.glMultModelView();
            drawPlane();
            glPopMatrix();
        }

        glPushMatrix();
        glMultMatrixd(qgl_mf_vbo.worldMatrix());

        glPushAttrib(GL_ALL_ATTRIB_BITS);
        PARAM(bool, vbo.enable_cull_face) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        if ( PARAM(bool, vbo.render_caster) ) {
            //
            vbo->setProg(prog_vbo);
            prog_vbo.activate();
            prog_vbo.setUniformVec3( "u_v3_light_pos_in_object", v4_light_position_in_object_vbo, false);
            prog_vbo.setUniform( "u_f_coef_ambient_lighting", f_coef_ambient_light, false );
            vbo->render(GL_TRIANGLES, indexBuffer);
            prog_vbo.deactivate();
        }

        if (PARAM(bool, vbo.enable_polygon_offset)) {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset( PARAM(float, vbo.polygon_offset_scale), PARAM(float, vbo.polygon_offset_bias) );
        }

        glPopAttrib();
        glPopMatrix();
    }
}

void Viewer::drawPlane() {
    const int i_nb_quad_patch = PARAM(int, plane.nb_quad_patch);
    float f_x, f_y, f_incr_x, f_incr_y;

    f_x = f_y = 0.f;
    f_incr_x = 1.f/(float)(i_nb_quad_patch);
    f_incr_y = 1.f/(float)(i_nb_quad_patch);

    // RENDER
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glDisable(GL_CULL_FACE);
    //
    glColor3f(1, 1, 1);
    for(int i=0; i<i_nb_quad_patch; i++) {
        f_x = 0.f;
        for(int j=0; j<i_nb_quad_patch; j++) {
            glBegin(GL_QUADS);
            glNormal3f(0, 0, 1);	glVertex3f(f_x, f_y, 0);
            glNormal3f(0, 0, 1);	glVertex3f(f_x + f_incr_x, f_y, 0);
            glNormal3f(0, 0, 1);	glVertex3f(f_x + f_incr_x, f_y + f_incr_y, 0);
            glNormal3f(0, 0, 1);	glVertex3f(f_x, f_y + f_incr_y, 0);
            glEnd();
            f_x += f_incr_x;
        }
        f_y += f_incr_y;
    }
    glPopAttrib();
}

void Viewer::setOpenGLStates() {
    // Activation du Depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    // Desactivation du Blending (on sait jamais :p)
    glDisable(GL_BLEND);

    // Activation du backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void Viewer::drawTexture(Texture tex, int x, int y, int w, int h) const {
    ViewportGL view, subView(x, y, w, h);   // plus general : taille texture
    TransfoGL::pushAndInit();
    view.setUnitSquare(subView);
    tex.activate();
    glBegin(GL_QUADS);
    glTexCoord2f(1,0); glVertex2f(1,0);
    glTexCoord2f(1,1); glVertex2f(1,1);
    glTexCoord2f(0,1); glVertex2f(0,1);
    glTexCoord2f(0,0); glVertex2f(0,0);
    glEnd();
    tex.deactivate();
    TransfoGL::pop();
    view.setGL();
}

void Viewer::drawTextures() {
    const int i_screen_width = (*camera()).screenWidth();
    const int i_screen_height = (*camera()).screenHeight();
    const int i_texture_debug_width = int((float)(i_screen_width) / 6.f);
    const int i_texture_debug_height = int((float)(i_screen_height) / 6.f);

    prog_draw_texture.activate();
    drawTexture( tex_sv, 0, 0, i_texture_debug_width, i_texture_debug_height );
    prog_draw_texture.deactivate();

    MSG_CHECK_GL;
}

void Viewer::drawLightCamera( float _fCoef_Alpha ) {
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    // Disable Culling
    glDisable(GL_CULL_FACE);
    // Enable semi-transparent culling planes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_LIGHTING);
    glDepthMask(GL_FALSE);
    glLineWidth(4.0);
    glColor4f(1.0, 1.0, 1.0, _fCoef_Alpha );
    // Draws the light's camera
    qgl_cam_light_mf.draw();
    // restore OpenGL attributes
    glPopAttrib();
}
