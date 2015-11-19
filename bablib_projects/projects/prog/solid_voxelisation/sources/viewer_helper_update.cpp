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
void debug_drawbox(Vec3 _min, Vec3 _max);
void draw_objects_for_solid_voxelisation(ProgGLSL _prog);

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
    /**
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
        prog_vbo_sv.activate(); {
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
        } prog_vbo_sv.deactivate();

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
    //debug_solid_voxelisation_graphic_mode_full_image(img_sv, fb_sv, qgl_cam_light_mf);

    /**/

    // utilisation d'une lambda fonction pour l'affichage de la scene a voxeliser
    // pour le '[&]' -> url: http://stackoverflow.com/questions/4940259/lambdas-require-capturing-this-to-call-static-member-function
    // Penser a placer le corps d'implémentation du template dictement accessible via le .h du prototype
    // => mettre en place des fichiers _impl.h comme dans la bablib
    solid_voxelisation->update( [&](ProgGLSL _prog) {
        // Draw scene occluders
        // Activate shader for rendering
        vbo->setProg( _prog );
        // RENDER
        if ( !PARAM(bool, vbo.enable_cull_face) )
            glDisable(GL_CULL_FACE);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        // MODEL_SPACE -> WORLD_SPACE
        pt_object_to_world_vbo.glMultModelView();
        vbo->render(GL_TRIANGLES, indexBuffer);
        glPopMatrix();
    });

    // gather the resulting texture data
    solid_voxelisation->texture().bind();
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    img_sv.readTexture(solid_voxelisation->ptrTexture());

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

        // desactivation du culling face
        glDisable(GL_CULL_FACE);

        glDepthMask(GL_TRUE);

        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_NOTEQUAL, 0);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable( GL_POINT_SMOOTH );
        glPointSize( 32.0 );
        GLfloat distance_att[] = {1.f, 0.f, 0.f};
        glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, distance_att);

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        //glMultMatrixd(_cam.frame()->worldMatrix());

        // 0 is the upper left coordinates of the near corner, 1 for the far one
        const qglviewer::Vec points[2] = {
            _cam.unprojectedCoordinatesOf(qglviewer::Vec(0, 0, 0), _cam.frame()),
            _cam.unprojectedCoordinatesOf(qglviewer::Vec(0, 0, 1), _cam.frame()),
        };

        const ViewportGL fb_viewport = _fb.viewport();
        const GLuint* viewport = &fb_viewport.x;

        GLdouble modelviewMatrix[16], projectionMatrix[16];
        _cam.getModelViewMatrix(modelviewMatrix);
        _cam.getProjectionMatrix(projectionMatrix);

        const float scale_Z = 1.0f / float(resZ);
        qglviewer::Vec point_in_screen_coordinate;
        GLdouble world_x, world_y, world_z;
        qglviewer::Vec voxel_minmax[2];
        const float voxel_alphas[] = {10.f/256.f, 0.95f};

        if( PARAM(bool, sv.render_voxels_set) ) {
            glBegin(GL_POINTS); {
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
                                if( cur_bit ) {
                                    for( int k=0; k <= 1; k++ ) {
                                        point_in_screen_coordinate = qglviewer::Vec(x+k, y+k, ( ((z+0.5)+k) + 32*(3-i) ) * scale_Z);
                                        // --------------------------------------------------------------------------------------------------------
                                        // url: https://www.opengl.org/wiki/GluProject_and_gluUnProject_code
                                        gluUnProject(point_in_screen_coordinate.x, point_in_screen_coordinate.y, point_in_screen_coordinate.z,
                                                     modelviewMatrix, projectionMatrix,
                                                     viewport,
                                                     &voxel_minmax[k].x,&voxel_minmax[k].y,&voxel_minmax[k].z
                                                     );
                                    }
                                    glColor4f(new_voxel*0.75f, cur_bit*0.5f, 1.f, voxel_alphas[cur_bit]);
                                    glVertex3dv( ((voxel_minmax[0]+voxel_minmax[1])*0.5) );
                                }
                                last_test_voxel = cur_bit;
                            }
                        }
                    }
                }
            } glEnd();
        }

        if( PARAM(bool, sv.render_voxels_unset) ) {
            // set blending mode
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);

            glDepthMask(GL_FALSE);

            {
                for( unsigned int y = 0; y < resY; y+=4 ) {
                    for( unsigned int x = 0; x < resX; x+=4 ) {
                        const UInt4& sample = _img_sv.texel(x, y);
                        for( int i=3; i>=0; i-- ) {
                            bs_sample = std::bitset<32>(sample[i]);
                            for( int z=0; z < 32; z+=4 ) {
                                const bool & cur_bit = bs_sample[z];
                                if( !cur_bit ) {
                                    for( int k=0; k <= 1; k++ ) {
                                        point_in_screen_coordinate = qglviewer::Vec(x+k, y+k, ( ((z+0.5)+k) + 32*(3-i) ) * scale_Z);
                                        // --------------------------------------------------------------------------------------------------------
                                        // url: https://www.opengl.org/wiki/GluProject_and_gluUnProject_code
                                        gluUnProject(point_in_screen_coordinate.x, point_in_screen_coordinate.y, point_in_screen_coordinate.z,
                                                     modelviewMatrix, projectionMatrix,
                                                     viewport,
                                                     &voxel_minmax[k].x,&voxel_minmax[k].y,&voxel_minmax[k].z
                                                     );
                                    }
                                    glColor4f(0.15f, 0.80, 0.1f, voxel_alphas[0]);
                                    debug_drawbox(Vec3(voxel_minmax[0]), Vec3(voxel_minmax[1]));
                                }
                            }
                        }
                    }
                }
            }
        }

        glPopMatrix();
    } glPopAttrib();
}

void debug_drawbox(Vec3 _min, Vec3 _max) {
    const GLfloat &xMin = _min.x;
    const GLfloat &yMin = _min.y;
    const GLfloat &zMin = _min.z;
    const GLfloat &xMax = _max.x;
    const GLfloat &yMax = _max.y;
    const GLfloat &zMax = _max.z;

    // face bas :
    glBegin(GL_LINE_LOOP);
        glVertex3f(xMin, yMin, zMin);
        glVertex3f(xMax, yMin, zMin);
        glVertex3f(xMax, yMax, zMin);
        glVertex3f(xMin, yMax, zMin);
    glEnd();

    // face haut :
    glBegin(GL_LINE_LOOP);
        glVertex3f(xMin, yMin, zMax);
        glVertex3f(xMax, yMin, zMax);
        glVertex3f(xMax, yMax, zMax);
        glVertex3f(xMin, yMax, zMax);
    glEnd();

    // segments verticaux :
    glBegin(GL_LINES);
        glVertex3f(xMin, yMin, zMin); glVertex3f(xMin, yMin, zMax);
        glVertex3f(xMax, yMin, zMin); glVertex3f(xMax, yMin, zMax);
        glVertex3f(xMax, yMax, zMin); glVertex3f(xMax, yMax, zMax);
        glVertex3f(xMin, yMax, zMin); glVertex3f(xMin, yMax, zMax);
    glEnd();
}

void Viewer::update_camera_light() {
    qgl_cam_light_mf.setType( PARAM(bool, sv.use_ortho_camera) ? qglviewer::Camera::ORTHOGRAPHIC : qglviewer::Camera::PERSPECTIVE );
    qgl_cam_light_mf.computeProjectionMatrix();
    qgl_cam_light_mf.computeModelViewMatrix();
}
