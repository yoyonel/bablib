#ifndef SOLIDVOXELISATION_DEBUG_IMPL_H
#define SOLIDVOXELISATION_DEBUG_IMPL_H

template <typename F0, typename F1>
void SolidVoxelisation_Debug::parse_grid(
        const Image2DUInt4& _img_sv,
        const FrameBuffer& _fb,
        const qglviewer::Camera& _cam,
        F0 _func_pass,
        F1 _draw_for_voxel,
        const GLuint _sparse_coef
        ) {
    const unsigned int resX = _img_sv.width();
    const unsigned int resY = _img_sv.height();
    const unsigned int resZ = 4*32;
    std::bitset<32> bs_sample;
    qglviewer::Vec point_in_screen_coordinate;
    const float scale_Z = 1.0f / float(resZ);
    GLdouble modelviewMatrix[16], projectionMatrix[16];
    _cam.getModelViewMatrix(modelviewMatrix);
    _cam.getProjectionMatrix(projectionMatrix);
    const ViewportGL fb_viewport = _fb.viewport();
    const int* viewport = &fb_viewport.x;
    qglviewer::Vec voxel_minmax[2];

    for( unsigned int y = 0; y < resY; y+=_sparse_coef ) {
        for( unsigned int x = 0; x < resX; x+=_sparse_coef ) {
            const UInt4& sample = _img_sv.texel(x, y);

            GLuint nb_voxels_in_column = 0;
            bool last_test_voxel = false;

            for( int i=3; i>=0; i-- ) {
                bs_sample = std::bitset<32>(sample[i]);
                for( int z=0; z < 32; z+=_sparse_coef ) {
                    const bool & cur_bit = bs_sample[z];
                    const bool new_voxel = (cur_bit) && (!last_test_voxel);
                    nb_voxels_in_column += new_voxel;
                    if (_func_pass(cur_bit)) {
                        for( int k=0; k <= 1; k++ ) {
                            point_in_screen_coordinate = qglviewer::Vec(x+k, y+k, ( ((z+0.5)+k) + 32*(3-i) ) * scale_Z);
                            // --------------------------------------------------------------------------------------------------------
                            // url: https://www.opengl.org/wiki/GluProject_and_gluUnProject_code
                            gluUnProject(point_in_screen_coordinate.x, point_in_screen_coordinate.y, point_in_screen_coordinate.z,
                                         modelviewMatrix, projectionMatrix,
                                         viewport,
                                         &voxel_minmax[k].x, &voxel_minmax[k].y, &voxel_minmax[k].z
                                         );
                        }
                        _draw_for_voxel(voxel_minmax, cur_bit, new_voxel, nb_voxels_in_column);
                        last_test_voxel = cur_bit;
                    }
                }
            }
        }
    }
}

#endif // SOLIDVOXELISATION_DEBUG_IMPL_H

