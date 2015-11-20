#include "SolidVoxelisation_debug.h"
//
#include <Vec3.h>
#include <FrameBuffer.h>
#include <Params.h>
//
#include <bitset>   // url: http://stackoverflow.com/questions/7349689/c-how-to-print-using-cout-the-way-a-number-is-stored-in-memory
#include <iostream>
#include <iomanip>  // url: http://stackoverflow.com/questions/2650244/formatting-an-integer-output-using-ostream


void SolidVoxelisation_Debug::debug_text_mode_1_column(const Image2DUInt4& _img_sv) {
    UInt4 sample = _img_sv.texel(_img_sv.width()/2, _img_sv.height()/2);

    std::cout << "bits field: sample = " << \
                 std::bitset<32>(sample.r) << \
                 std::bitset<32>(sample.g) << \
                 std::bitset<32>(sample.b) << \
                 std::bitset<32>(sample.a) << \
                 std::endl;
}

void SolidVoxelisation_Debug::debug_text_mode_full_image(const Image2DUInt4& _img_sv) {
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
            std::cout << std::setfill('0') << std::setw(2) << nb_voxels_in_column << "_";
        }
        std::cout << std::endl;
    }
}

void SolidVoxelisation_Debug::debug_graphic_mode_full_image(
        const Image2DUInt4& _img_sv,
        const FrameBuffer& _fb,
        const qglviewer::Camera& _cam
        ) {
    glPushAttrib( GL_ALL_ATTRIB_BITS ); {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        GLdouble modelviewMatrix[16], projectionMatrix[16];
        _cam.getModelViewMatrix(modelviewMatrix);
        _cam.getProjectionMatrix(projectionMatrix);

        if( PARAM(bool, sv.render_voxels_set) ) {
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

            glBegin(GL_POINTS); {
                // url: http://stackoverflow.com/questions/7627098/what-is-a-lambda-expression-in-c11
                const auto & lambda_test = [](bool _bit_voxel) { return _bit_voxel; };
                const auto & lambda_draw = [](const qglviewer::Vec voxel_minmax[2], bool set_voxel, bool new_voxel, GLuint){
                    {
                        glColor4f(new_voxel*0.75f, set_voxel*0.5f, 1.f, 0.95f);
                        glVertex3dv( ((voxel_minmax[0]+voxel_minmax[1])*0.5) );
                    }
                };
                //
                SolidVoxelisation_Debug::parse_grid(
                            _img_sv, _fb, _cam,
                            lambda_test,
                            lambda_draw
                );
            } glEnd();
        }

        if( PARAM(bool, sv.render_voxels_unset) ) {
            // set blending mode
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);

            glDepthMask(GL_FALSE);

            {
                const GLuint offset_sparse_voxel = 4; // divide by 4 the resolution of the voxel grid
                ///
                const auto & lambda_test = [](bool _bit_voxel) { return !_bit_voxel; };
                const auto & lambda_draw = [](const qglviewer::Vec voxel_minmax[2], bool, bool, GLuint) {
                    {
                        glColor4f(0.15f, 0.80, 0.1f, 0.05);
                        debug_drawbox(Vec3(voxel_minmax[0]), Vec3(voxel_minmax[1]));
                    }
                };
                //
                SolidVoxelisation_Debug::parse_grid(
                            _img_sv, _fb, _cam,
                            lambda_test,
                            lambda_draw,
                            offset_sparse_voxel
                );
            }
        }

        glPopMatrix();
    } glPopAttrib();
}

void SolidVoxelisation_Debug::debug_drawbox(Vec3 _min, Vec3 _max) {
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
