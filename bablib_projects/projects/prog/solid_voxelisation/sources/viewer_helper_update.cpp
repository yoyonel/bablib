#include "viewer_sv.h"
//
#include <Vec3.h>
#include <Params.h>
#include <Image2D.h>
//
#include <bitset>   // url: http://stackoverflow.com/questions/7349689/c-how-to-print-using-cout-the-way-a-number-is-stored-in-memory
#include <iostream>
#include <iomanip>  // url: http://stackoverflow.com/questions/2650244/formatting-an-integer-output-using-ostream


void debug_solid_voxelisation(const Image2DUInt4& _img_sv);


void Viewer::updateFrameBuffers() {
    update_matrix();
    MSG_CHECK_GL;

    // - FRAME_BUFFERS: Maj des FBs et Render-Textures associées
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

        // -------

        // Set light's camera (OpenGL 1.x style)
        // Sauvegarde des matrices de transformations
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        pt_light_to_projective.glLoadProjection();

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        pt_world_to_light.glLoadModelView();

        // Draw scene occluders
        // Activate shader for rendering
        vbo->setProg( prog_vbo_sv );
        prog_vbo_sv.activate();
        // Send camera light informations: Near&Far distances
        prog_vbo_sv.setUniform("nearClipPlane", qgl_cam_light_mf.zNear());
        prog_vbo_sv.setUniform("farClipPlane", qgl_cam_light_mf.zFar());
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
    debug_solid_voxelisation(img_sv);

    MSG_CHECK_GL;
}

void debug_solid_voxelisation(const Image2DUInt4& _img_sv) {
    //UInt4 sample = _img_sv.texel(tex_sv.getWidth()/2, tex_sv.getHeight()/2);
    /**
    std::cout << "bits field: sample.r = " << std::bitset<32>(sample.r)  << std::endl;
    std::cout << "bits field: sample.g = " << std::bitset<32>(sample.g)  << std::endl;
    std::cout << "bits field: sample.b = " << std::bitset<32>(sample.b)  << std::endl;
    std::cout << "bits field: sample.a = " << std::bitset<32>(sample.a)  << std::endl;
    /**/
    //
    /**
    std::cout << "bits field: sample = " << \
                 std::bitset<32>(sample.r) << \
                 std::bitset<32>(sample.g) << \
                 std::bitset<32>(sample.b) << \
                 std::bitset<32>(sample.a) << \
                 std::endl;
    /**/
    const UInt4* data = _img_sv.mem();
    { // dump voxels
        const unsigned int resX = _img_sv.width();
        const unsigned int resY = _img_sv.height();

        for( int y = 0; y < resY; y++ ) {
            for( int x = 0; x < resX; x++ ) {

                unsigned int nb_voxels_in_column = 0;
                for( int i = 3; i >= 0; i-- ) {
                    const unsigned int &col = data[x + y * resX].c[i];

                    if ( col == 0 ) continue;

                    bool last_test_voxel = false;
                    for( int z = 0; z < 32; z++ ) { // unpack color data

                        const bool is_activ_voxel = ( col & ( 1 << z ) ) != 0;
                        if ( is_activ_voxel ) { // if the z-th bit is set, create a voxel
                            /**
                            MPoint bbMin( bounds.min().x + x * deltaX, bounds.min().y + y * deltaY, bounds.max().z - ( 32 * ( 3 - i ) + z ) * deltaZ );
                            MPoint bbMax( bbMin.x + deltaX, bbMin.y + deltaY, bbMin.z + deltaZ );
                            bbMin += halfVoxel;
                            bbMax += halfVoxel;
                            voxels.append( bbMin );
                            voxels.append( bbMax );
                            /**/
                        }
                        //nb_voxels_in_column += (last_test_voxel != is_activ_voxel);
                        //last_test_voxel = is_activ_voxel;
                        nb_voxels_in_column += is_activ_voxel;
                    }
                }
               std::cout << setfill('0') << setw(2) << nb_voxels_in_column << "_";
            }
            std::cout << std::endl;
        }
    }
}
