#ifndef SOLIDVOXELISATION_DEBUG_H
#define SOLIDVOXELISATION_DEBUG_H

#include "SolidVoxelisation.h"
//
#include <Image2D.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/gl.h>
#include <QGLViewer/camera.h>
//
#include <bitset>   // url: http://stackoverflow.com/questions/7349689/c-how-to-print-using-cout-the-way-a-number-is-stored-in-memory
#include <iostream>
#include <iomanip>  // url: http://stackoverflow.com/questions/2650244/formatting-an-integer-output-using-ostream


typedef Image2D<UInt4> Image2DUInt4;
class FrameBuffer;
class Vec3;


class SolidVoxelisation_Debug
{
public:
    static void debug_text_mode_1_column(const Image2DUInt4& _img_sv);
    static void debug_text_mode_full_image(const Image2DUInt4& _img_sv);
    static void debug_graphic_mode_full_image(const Image2DUInt4& _img_sv,  const FrameBuffer& _fb, const qglviewer::Camera& _cam);
    static void debug_graphic_mode_full_image(const SolidVoxelisation& _sv) { debug_graphic_mode_full_image(_sv.getImage(), _sv.framebuffer(), _sv.camera()); }
    static void debug_graphic_mode_full_image(const SolidVoxelisation* _sv) { debug_graphic_mode_full_image(*_sv); }
    static void debug_drawbox(Vec3 _min, Vec3 _max);

    template <typename F0, typename F1>
    static void parse_grid(
            const Image2DUInt4& _img_sv,
            const FrameBuffer& _fb,
            const qglviewer::Camera& _cam,
            F0 _func_pass,
            F1 _draw_for_voxel,
            const GLuint _sparse_coef = 1);
};

#include "SolidVoxelisation_debug_impl.h"

#endif // SOLIDVOXELISATION_DEBUG_H
