#include "viewer_sv.h"
//
#include <Vec3.h>
#include <Params.h>
#include <Image2D.h>
//
#include <bitset>   // url: http://stackoverflow.com/questions/7349689/c-how-to-print-using-cout-the-way-a-number-is-stored-in-memory
#include <iostream>
#include <iomanip>  // url: http://stackoverflow.com/questions/2650244/formatting-an-integer-output-using-ostream
//
//#include <groupby.hpp>
//#include <vector>


void debug_solid_voxelisation(const Image2DUInt4& _img_sv);
void debug_solid_voxelisation_text_mode_1_column(const Image2DUInt4& _img_sv);
void debug_solid_voxelisation_text_mode_full_image(const Image2DUInt4& _img_sv);
void debug_solid_voxelisation_graphic_mode_full_image(const Image2DUInt4& _img_sv,  const FrameBuffer& _fb, const qglviewer::Camera& _cam);

void Viewer::updateFrameBuffers() {
    update_matrix();
    MSG_CHECK_GL;

    // - FRAME_BUFFERS: Maj des FBs et Render-Textures associÃ©es
    updateSolidVoxelisation( fb_sv );

    // - TIMER_QUERY
    //
    if ( PARAM(bool, timer_query.print) ) {
    }
}

void Viewer::updateSolidVoxelisation(FrameBuffer& _framebuffer) {
    // ----------
    const GLenum clearBufferBits = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT;
    const GLboolean GL_ALL_BITS = GLboolean(~0x0);
    const GLboolean colorMask[] = {GL_ALL_BITS, GL_ALL_BITS, GL_ALL_BITS, GL_ALL_BITS};

    glPushAttrib( GL_ALL_ATTRIB_BITS ); {
        _framebuffer.activate();
        glViewport(0, 0, _framebuffer.width(), _framebuffer.height());

        //setOpenGLStates();

        glColorMask( colorMask[0], colorMask[1], colorMask[2], colorMask[3] );
        glClearColor( 0x0, 0x0, 0x0, 0x0 );
        glClear( clearBufferBits );

        // desactivation du test de profondeur
        glDisable( GL_DEPTH_TEST );

        // desactivation du culling face
        glDisable(GL_CULL_FACE);

        // set blending mode
        glEnable( GL_BLEND );
        glLogicOp( GL_XOR );
        glEnable( GL_COLOR_LOGIC_OP );

        glDisable(GL_LIGHTING);

        // -------

        // Set light's camera (OpenGL 1.x style)
        // Sauvegarde des matrices de transformations
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        //pt_light_to_projective.glLoadProjection();
        qgl_cam_light_mf.loadProjectionMatrix();

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        //pt_world_to_light.glLoadModelView();
        qgl_cam_light_mf.loadModelViewMatrix();

        // Draw scene occluders
        // Activate shader for rendering
        vbo->setProg( prog_vbo_sv );
        prog_vbo_sv.activate();
        // Send camera light informations: Near&Far distances
        //prog_vbo_sv.setUniform("nearClipPlane", qgl_cam_light_mf.zNear());
        //prog_vbo_sv.setUniform("farClipPlane", qgl_cam_light_mf.zFar());
        // RENDER
        if ( !PARAM(bool, vbo.enable_cull_face) )
            glDisable(GL_CULL_FACE);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        // MODEL_SPACE -> WORLD_SPACE
        pt_object_to_world_vbo.glMultModelView();
        vbo->render(GL_TRIANGLES, indexBuffer);
        glPopMatrix();
        prog_vbo_sv.deactivate();

        // Restore the OpenGL Matrix
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        //
        _framebuffer.deactivate();
    } glPopAttrib();

    // gather the resulting texture data
    tex_sv.bind();
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    img_sv.readTexture(&tex_sv);

    // For debugging (mode text debugging)
    //debug_solid_voxelisation(img_sv);
    //
    debug_solid_voxelisation_graphic_mode_full_image(img_sv, fb_sv, qgl_cam_light_mf);

    MSG_CHECK_GL;
}

void debug_solid_voxelisation(const Image2DUInt4& _img_sv) {
    //debug_solid_voxelisation_text_mode_1_column(_img_sv);
    //debug_solid_voxelisation_text_mode_full_image(_img_sv);
}

void debug_solid_voxelisation_text_mode_1_column(const Image2DUInt4& _img_sv) {
    UInt4 sample = _img_sv.texel(_img_sv.width()/2, _img_sv.height()/2);

    /**
    std::cout << "bits field: sample.r = " << std::bitset<32>(sample.r)  << std::endl;
    std::cout << "bits field: sample.g = " << std::bitset<32>(sample.g)  << std::endl;
    std::cout << "bits field: sample.b = " << std::bitset<32>(sample.b)  << std::endl;
    std::cout << "bits field: sample.a = " << std::bitset<32>(sample.a)  << std::endl;
    /**/

    //
    std::cout << "bits field: sample = " << \
                 std::bitset<32>(sample.r) << \
                 std::bitset<32>(sample.g) << \
                 std::bitset<32>(sample.b) << \
                 std::bitset<32>(sample.a) << \
                 std::endl;
}

void debug_solid_voxelisation_text_mode_full_image(const Image2DUInt4& _img_sv) {
    //const UInt4* data = _img_sv.mem();
    // dump voxels
    const unsigned int resX = _img_sv.width();
    const unsigned int resY = _img_sv.height();

    std::bitset<32> bs_sample;

    for( unsigned int y = 0; y < resY; y++ ) {
        for( unsigned int x = 0; x < resX; x++ ) {
            const UInt4& sample = _img_sv.texel(x, y);

            unsigned int nb_voxels_in_column = 0;
            bool last_test_voxel = false;

            //url: https://www.daniweb.com/programming/software-development/threads/31958/bitset
            //std::vector<std::bitset<32>> vec_bs(sample.c, sample.c + 4);

            for( int i=3; i>=0; i-- ) {
                bs_sample = std::bitset<32>(sample[i]);
                for( int z =0; z < 32; z++ ) {
                    const bool & cur_bit = bs_sample[z];
                    nb_voxels_in_column += (cur_bit) && (!last_test_voxel);
                    last_test_voxel = cur_bit;
                }
            }
            std::cout << setfill('0') << setw(2) << nb_voxels_in_column << "_";
        }
        std::cout << std::endl;
    }
}

void debug_solid_voxelisation_graphic_mode_full_image(const Image2DUInt4& _img_sv,  const FrameBuffer& _fb, const qglviewer::Camera& _cam) {
    // dump voxels
    const unsigned int resX = _img_sv.width();
    const unsigned int resY = _img_sv.height();
    const unsigned int resZ = 4*32;

    std::bitset<32> bs_sample;

    glPushAttrib( GL_ALL_ATTRIB_BITS ); {

        glDepthMask(GL_FALSE);

        // desactivation du culling face
        glDisable(GL_CULL_FACE);

        // set blending mode
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBlendEquation(GL_FUNC_ADD);

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glMultMatrixd(_cam.frame()->worldMatrix());

        // 0 is the upper left coordinates of the near corner, 1 for the far one
        qglviewer::Vec points[2];
        const float scale = 1.f;
        points[0].z = scale * _cam.zNear();
        points[1].z = scale * _cam.zFar();

        points[0].y = points[0].z * tan(_cam.fieldOfView()/2.0);
        points[0].x = points[0].y * _cam.aspectRatio();

        const float ratio = points[1].z / points[0].z;

        points[1].y = ratio * points[0].y;
        points[1].x = ratio * points[0].x;

        glPointSize(8);

        glBegin(GL_POINTS);

        const bool drawFarPlane = true;
        const int farIndex = drawFarPlane?1:0;

        // Near and (optionally) far plane(s)
        glColor3f(0.0, 1.0, 0.0);
        for (int i=farIndex; i>=0; --i) {
            glVertex3f( points[i].x,  points[i].y, -points[i].z);
            glVertex3f(-points[i].x,  points[i].y, -points[i].z);
            glVertex3f(-points[i].x, -points[i].y, -points[i].z);
            glVertex3f( points[i].x, -points[i].y, -points[i].z);
        }

        const float deltaX = (2.f*points[0].x) / (float)(resX);
        const float deltaY = (2.f*points[0].y) / (float)(resY);
        const float deltaZ = (points[1].z-points[0].z) / (float)(resZ);

        glColor3f(1.0, 0.0, 0.0);

        GLint viewport[4];
        viewport[0] = _fb.viewport().x;
        viewport[1] = _fb.viewport().y;
        viewport[2] = _fb.viewport().w;
        viewport[3] = _fb.viewport().h;
        GLdouble modelviewMatrix[16], projectionMatrix[16];
        _cam.getModelViewMatrix(modelviewMatrix);
        _cam.getProjectionMatrix(projectionMatrix);
        //std::cout << "viewport: " << viewport[0] << viewport[1] << viewport[2] << viewport[3] << std::endl;

        for( unsigned int y = 0; y < resY; y++ ) {
            for( unsigned int x = 0; x < resX; x++ ) {
                const UInt4& sample = _img_sv.texel(x, y);

                unsigned int nb_voxels_in_column = 0;
                bool last_test_voxel = false;

                //url: https://www.daniweb.com/programming/software-development/threads/31958/bitset
                //std::vector<std::bitset<32>> vec_bs(sample.c, sample.c + 4);

                for( int i=3; i>=0; i-- ) {
                    bs_sample = std::bitset<32>(sample[i]);
                    for( int z=0; z < 32; z++ ) {
                        const bool & cur_bit = bs_sample[z];
                        const bool new_voxel = (cur_bit) && (!last_test_voxel);
                        nb_voxels_in_column += new_voxel;
                        //if (new_voxel) {
                        if( cur_bit ) {
                            glColor4f(0.f, 1.0f, 0.0f, 0.95f);
                        }
                        else
                            glColor4f(0.15f, 0.05f, 1.0f, 0.05f);

                        {
                            const float z_cam = ( z + 32*(3-i) ) / float(resZ);
                            qglviewer::Vec point_in_screen_coordinate(x, y, z_cam);
                            // --------------------------------------------------------------------------------------------------------
                            GLdouble x,y,z;
                            // url: https://www.opengl.org/wiki/GluProject_and_gluUnProject_code
                            gluUnProject(point_in_screen_coordinate.x, point_in_screen_coordinate.y, point_in_screen_coordinate.z,
                                         modelviewMatrix, projectionMatrix,
                                         viewport,
                                         &x,&y,&z
                                         );
                            const qglviewer::Vec point = _cam.frame()->coordinatesOf(qglviewer::Vec(x,y,z));
                            // --------------------------------------------------------------------------------------------------------
                            glVertex3dv( &point.x );
                        }

                        last_test_voxel = cur_bit;
                    }
                }
            }
        }
        glEnd();

        glPopMatrix();
    } glPopAttrib();
}

void Viewer::update_camera_light() {
    qgl_cam_light_mf.setType( PARAM(bool, sv.use_ortho_camera) ? qglviewer::Camera::ORTHOGRAPHIC : qglviewer::Camera::PERSPECTIVE );
    qgl_cam_light_mf.computeProjectionMatrix();
    qgl_cam_light_mf.computeModelViewMatrix();
}
